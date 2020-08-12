#include "pch.h"
#include "AnimationTilePopulator.h"

#include "Map/Tiles/MapTile.h"
#include "Animations/AnimationReader.h"


void AnimationTilePopulator::addAnimations(Grid<MapTile>& data, const TextureManager* textures)
{
	XMLParser tourchHandleParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "TorchHandle"));
	XMLParser tourchBowlParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "TorchBowl"));
	XMLParser spikeParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "SpikeTrap"));
	XMLParser triggerParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "TriggerTrap"));

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			MapTile& tile = data[Index(x, y)];

			if (tile.has(DecorType::Torch_Handle))
			{
				XMLNode node = tourchHandleParser.rootChild("Animator");
				Animator animator = buildAnimation(node, textures);
				tile.addAnimation(animator);
			}

			if (tile.has(DecorType::Torch_Bowl))
			{
				XMLNode node = tourchBowlParser.rootChild("Animator");
				Animator animator = buildAnimation(node, textures);
				tile.addAnimation(animator);
			}

			if (tile.has(DecorType::Spikes))
			{
				XMLNode node = spikeParser.rootChild("Animator");
				Animator animator = buildAnimation(node, textures);
				tile.addAnimation(animator);
				tile.animation(0).pause();
			}

			if (tile.has(DecorType::Trigger))
			{
				XMLNode node = triggerParser.rootChild("Animator");
				Animator animator = buildAnimation(node, textures);
				tile.addAnimation(animator);
				tile.animation(0).pause();
			}
		}
	}
}




Animator AnimationTilePopulator::buildAnimation(const XMLNode& node, const TextureManager* textures)
{
	AnimationReader reader;
	return reader.buildAnimator(node, textures);
}