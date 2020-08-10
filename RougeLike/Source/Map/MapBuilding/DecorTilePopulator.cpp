#include "pch.h"
#include "DecorTilePopulator.h"

#include "Animations/AnimationReader.h"
//#include "Animations/Animator.h"


void DecorTilePopulator::populate(Grid<MapTile>& data)
{
	addWater(data);
	addColumns(data);

	addAnimations(data);
}


// -- Decore Tiles -- //
void DecorTilePopulator::addWater(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			const Index index(x, y);

			if (data[index].is(CollisionTile::Water))
			{
				int width = 0;
				while (data[index + Index(width,0)].is(CollisionTile::Water))
					width++;

				int height = 0;
				while (data[index + Index(0,height)].is(CollisionTile::Water))
					height++;

				// Top floor
				for (int xPool = 1; xPool < width - 1; xPool++)
				{
					data[index + Index(xPool, 0)].set(RenderTile::Floor_Bottom);
				}

				// Top water
				for (int xPool = 1; xPool < width - 1; xPool++)
				{
					if (xPool == 1)
						data[index + Index(xPool, 1)].set(RenderTile::Water_Top_Left);
					else
						data[index + Index(xPool, 1)].set(RenderTile::Water_Top);
				}

				// Fill water center
				for (int xPool = 1; xPool < width - 1; xPool++)
				{
					for (int yPool = 2; yPool < height - 1; yPool++)
					{
						if (xPool == index.x + 1)
							data[index + Index(xPool, yPool)].set(RenderTile::Water_Left);
						else
							data[index + Index(xPool, yPool)].set(RenderTile::Water_Middle);
					}
				}

				// Left
				for (int yPool = 0; yPool < height - 1; yPool++)
				{
					data[index + Index(0, yPool)].set(RenderTile::Floor_Right);
				}

				// Right
				for (int yPool = 0; yPool < height - 1; yPool++)
				{
					data[index + Index(width - 1, yPool)].set(RenderTile::Floor_Left);
				}

				// Bot
				for (int xPool = 1; xPool < width - 1; xPool++)
				{
					data[index + Index(xPool, height - 1)].set(RenderTile::Floor_Top);
				}


				// Corners
				data[index].set(						RenderTile::Floor_Bottom_Right);
				data[index + Index(width - 1, 0)].set(			RenderTile::Floor_Bottom_Left);
				data[index + Index(width - 1, height - 1)].set(	RenderTile::Floor_Top_Left);
				data[index + Index(0, height - 1)].set(				RenderTile::Floor_Top_Right);

				// Move to end of pool + 1
				x = index.x + width + 1;
			}
		}
	}
}

void DecorTilePopulator::addColumns(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);

			if (data[index].has(DecorType::Column))
			{
				if (data[index].has(RenderTile::Bottom))
					data[index].set(RenderTile::Column_Top);

				else if (data[index].has(RenderTile::Bottom_Upper))
					data[index].set(RenderTile::Column_Upper);

				else if (data[index].has(RenderTile::Bottom_Lower))
					data[index].set(RenderTile::Column_Lower);

				else if (data[index].is(CollisionTile::Floor))
					data[index].set(RenderTile::Floor_ColumnBase);
			}
		}
	}
}

void DecorTilePopulator::addAnimations(Grid<MapTile>& data)
{
	XMLParser tourchHandleParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "TorchHandle"));
	XMLParser tourchBowlParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "TorchBowl"));
	XMLParser spikeParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "SpikeTrap"));

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			MapTile& tile = data[Index(x, y)];


			if (tile.has(DecorType::Torch_Handle))
			{
				Animator animator = buildAnimation(tourchHandleParser);
				tile.addAnimation(animator);
			}

			if (tile.has(DecorType::Torch_Bowl))
			{
				Animator animator = buildAnimation(tourchBowlParser);
				tile.addAnimation(animator);
			}

			if (tile.has(DecorType::Spikes))
			{
				Animator animator = buildAnimation(spikeParser);
				tile.addAnimation(animator);
				tile.animation(0).pause();
			}
		}
	}
}




Animator DecorTilePopulator::buildAnimation(const XMLParser& parser)
{
	AnimationReader reader;
	XMLNode node = parser.rootChild("Animator");

	return reader.buildAnimator(node, mTextures);
}


