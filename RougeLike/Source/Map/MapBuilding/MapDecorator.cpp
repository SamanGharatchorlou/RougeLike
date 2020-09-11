#include "pch.h"
#include "MapDecorator.h"

#include "Map/Map.h"


void MapDecorator::addDecor(Map* map, const DecorMap& decorMap)
{
	Grid<MapTile>& data = map->getData();

	addColumns(data, decorMap);
	addWaterFeatures(data, decorMap);
	addTorches(data, decorMap);
	addSpikes(data, decorMap);
	addTiggers(data, decorMap);
	addGrating(data, decorMap);
}


void MapDecorator::addGrating(Grid<MapTile>& data, const DecorMap& decorMap)
{
	if (decorMap.contains(DecorType::Grating))
	{
		for (int x = 0; x < data.xCount() - 1; x++)
		{
			for (int y = 0; y < data.yCount(); y++)
			{
				Index index(x, y);
				Index down = index + Index(0, 1);

				if (data[index].is(CollisionTile::Floor))
				{
					data[down].add(DecorType::Grating);
					if (data[index].is(DecorType::None))
					{
						data[index].add(DecorType::Grating);
					}
				}
			}
		}
	}

}


// Water
void MapDecorator::addWaterFeatures(Grid<MapTile>& data, const DecorMap& decorMap)
{
	DecorType water = DecorType::Water;
	if (decorMap.contains(water))
	{
		const StringMap& attributes = decorMap.at(water);
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
					while (canAddWater(data, index, poolSize + Index(1, 0)))
					{
						poolSize.x++;
					}
					while (canAddWater(data, index, poolSize + Index(0, 1)))
					{
						poolSize.y++;
					}

					// +1 either sides of pool size
					for (int x = index.x; x < index.x + poolSize.x + 2; x++)
					{
						for (int y = index.y; y < index.y + poolSize.y + 2; y++)
						{
							data[Index(x, y)].set(CollisionTile::Water);
							data[Index(x, y)].add(water);
						}
					}

					x += spacing + width;
					break;
				}
			}
		}
	}
}



// Columns
void MapDecorator::addColumns(Grid<MapTile>& data, const DecorMap& decorMap)
{
	DecorType column = DecorType::Column;
	if (decorMap.contains(column))
	{
		const StringMap& attributes = decorMap.at(column);
		int spacing = attributes.getInt("spacing");

		for (int x = 0; x < data.xCount(); x++)
		{
			for (int y = 0; y < data.yCount(); y++)
			{
				Index index(x, y);

				if (isLowerWall(data, index) && canAddWallDecor(data, column, index, spacing))
				{
					Index up(index + Index(0, -1));
					Index up2(index + Index(0, -2));
					Index down(index + Index(0, 1));

					if (isValid(up2, data))
						data[up2].add(column); // Wall top

					if (isValid(up, data))
						data[up].add(column); // Upper

					data[index].add(column); // Lower
					data[down].add(column); // Base

					break;
				}
			}
		}
	}
}



// Torches
void MapDecorator::addTorches(Grid<MapTile>& data, const DecorMap& decorMap)
{
	DecorType torch = DecorType::Torch;
	if (decorMap.contains(torch))
	{
		const StringMap& attributes = decorMap.at(torch);
		DecorType torchType = stringToDecorType(attributes.at("type"));
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
						data[index].add(torch | torchType);
					}
				}

			}
		}
	}
}


// Spikes
void MapDecorator::addSpikes(Grid<MapTile>& data, const DecorMap& decorMap)
{
	DecorType spikes = DecorType::Spikes;
	if (decorMap.contains(spikes))
	{
		const StringMap& attributes = decorMap.at(spikes);
		int rate = attributes.getInt("rate");
		for (int x = 0; x < data.xCount(); x++)
		{
			for (int y = 0; y < data.yCount(); y++)
			{
				Index index(x, y);
				Index up(x, y);

				if (data[index].is(CollisionTile::Floor) && data[index].is(DecorType::None))
				{
					if (data[index].has(DecorType::Water))
					{
						bool test = data[index].is(DecorType::None);
					}
					bool addSpike = randomNumberBetween(0, 101) > 100 - rate;
					if (addSpike)
					{
						data[Index(x, y)].add(spikes);
					}
				}
			}
		}
	}
}

// Spikes
void MapDecorator::addTiggers(Grid<MapTile>& data, const DecorMap& decorMap)
{
	DecorType trigger = DecorType::GratingTrigger;
	if (decorMap.contains(trigger))
	{
		Index upperTrigger(13, 6);
		Index lowerTirgger(13, 7);

		data[upperTrigger].set(trigger);
		data[lowerTirgger].set(trigger);
	}
}


bool MapDecorator::canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const
{
	// +1 spacing for either sides of pool so we can always walk around it
	for (int x = index.x; x < index.x + size.x + 2; x++)
	{
		// +1 either sides of pool (spacing like for x), but also...
		// +2 on top for water top edge parralax + one empty tile in case of a column base
		// +1 on bottom to make sure there's a floor gap between water and wall, for the correct wall render type calculation
		for (int y = index.y - 1; y < index.y + size.y + 3; y++)
		{
			if (isValid(Index(x, y), data))
			{
				const MapTile& tile = data.get(Index(x, y));
				if (tile.is(CollisionTile::Floor))
					continue;
			}

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

		if (!isValid(wallIndex, data))
			continue;

		// needs to be a facing wall i.e. render::lower or something, although this works well for columns...by fluke
		if (data.get(wallIndex).is(CollisionTile::Wall)) 
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