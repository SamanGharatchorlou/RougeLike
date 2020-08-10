#include "pch.h"
#include "MapDecorator.h"




// -- Decorator -- //
void MapDecorator::addDecor(DecorMap decorMap, Grid<MapTile>& data)
{
	for (DecorMap::const_iterator iter = decorMap.begin(); iter != decorMap.end(); iter++)
	{
		DecorType type = iter->first;

		if (doesTypeContain(type, DecorType::Column))
		{
			addColumns(data, iter->second);
		}

		if (doesTypeContain(type, DecorType::Water))
		{
			addWaterFeatures(data, iter->second);
		}

		if (doesTypeContain(type, DecorType::Torch))
		{
			addTorches(data, iter->second);
		}

		if (doesTypeContain(type, DecorType::Spikes))
		{
			addSpikes(data, iter->second);
		}
	}

}


// Water
void MapDecorator::addWaterFeatures(Grid<MapTile>& data, const StringMap& attributes)
{
	int width = attributes.getInt("width");
	int height = attributes.getInt("height");
	int spacing = attributes.getInt("spacing");
	Vector2D<int> poolSize(width, height);

	int xWaterIndex = -spacing;

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			if (canAddWater(data, index, poolSize))
			{
				// +1 either sides of pool size
				for (int x = index.x; x < index.x + poolSize.x + 2; x++)
				{
					for (int y = index.y; y < index.y + poolSize.y + 2; y++)
					{
						data[Index(x, y)].set(CollisionTile::Water);
						data[Index(x, y)].add(DecorType::Water);
					}
				}

				x += spacing + width;
				break;
			}
		}
	}
}





// Columns
void MapDecorator::addColumns(Grid<MapTile>& data, const StringMap& attributes)
{
	int spacing = attributes.getInt("spacing");

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			if (isLowerWall(data, index) && canAddWallDecor(data, DecorType::Column, index, spacing))
			{
				Index up(index + Index(0, -1));
				Index up2(index + Index(0, -2));
				Index down(index + Index(0, 1));

				if (isValid(up2, data))
					data[up2].add(DecorType::Column); // Wall top

				if (isValid(up, data))
					data[up].add(DecorType::Column); // Upper

				data[index].add(DecorType::Column); // Lower
				data[down].add(DecorType::Column); // Base

				//x += columnWidth - 1;
				break;
			}
		}
	}
}




// Torches
void MapDecorator::addTorches(Grid<MapTile>& data, const StringMap& attributes)
{
	DecorType torchType = stringToType(attributes.getString("type"));
	int spacing = attributes.getInt("spacing");

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			if (isLowerWall(data, index))
			{
				if (canAddWallDecor(data, torchType, index, spacing))
				{
					data[index].add(DecorType::Torch | torchType);
				}
			}

		}
	}
}


// Spikes
void MapDecorator::addSpikes(Grid<MapTile>& data, const StringMap& attributes)
{
	int rate = attributes.getInt("rate");
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			Index up(x, y);

			if (data[index].is(CollisionTile::Floor) && data[index].is(DecorType::None))
			{
				bool addSpike = randomNumberBetween(0, 101) > 100 - rate;
				if (addSpike)
				{
					data[Index(x, y)].add(DecorType::Spikes);
				}
			}
		}
	}
}


bool MapDecorator::canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const
{
	// +1 either sides of pool
	for (int x = index.x; x < index.x + size.x + 2; x++)
	{
		// +3 (two water tiles + one empty tile for column base) on top + 1 on bottom
		for (int y = index.y - 1; y < index.y + size.y + 2; y++)
		{
			if (isValid(Index(x, y), data) && data.get(Index(x, y)).is(CollisionTile::Floor) && data.get(Index(x, y)).is(DecorType::None))
				continue;

			return false;
		}
	}

	return true;
}


bool MapDecorator::isLowerWall(const Grid<MapTile>& data, Index index) const
{
	Index down(index + Index(0, 1));
	bool validIndexes = isValid(index, data) && isValid(down, data);

	return validIndexes && data.get(index).is(CollisionTile::Wall) && data.get(down).is(CollisionTile::Floor);
}


bool MapDecorator::canAddWallDecor(const Grid<MapTile>& data, DecorType decor, Index index, int spacing) const
{
	// tile is already occupied
	if (!data.get(index).is(DecorType::None))
		return false;

	for (int i = index.x - (spacing / 2); i <= index.x + (spacing / 2); i++)
	{
		Index wallIndex(i, index.y);
		bool validIndex = isValid(wallIndex, data);

		// needs to be a facing wall i.e. render::lower or something, although this works well for columns...by fluke
		if (validIndex && data.get(wallIndex).is(CollisionTile::Wall)) 
		{
			bool alreadyContained = data.get(wallIndex).has(decor);
			if (!alreadyContained)
				continue;
		}

		return false;
	}

	return true;
}




bool MapDecorator::doesTypeContain(DecorType baseType, DecorType queryType) const
{
	return (baseType & queryType) != DecorType::None;
}