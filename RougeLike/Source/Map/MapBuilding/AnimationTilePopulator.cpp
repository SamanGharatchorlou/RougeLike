#include "pch.h"
#include "AnimationTilePopulator.h"

#include "Map/Tiles/MapTile.h"
#include "Animations/AnimationReader.h"


void AnimationTilePopulator::addAnimations(Grid<MapTile>& data)
{
	FileManager* fm = FileManager::Get();
	std::unordered_map<DecorType, XMLParser> parserMap;

	std::vector<BasicString> filePaths = fm->allFilesInFolder(FileManager::Configs_MapObjects);
	for (const BasicString& path : filePaths)
	{
		const BasicString fileName = fm->getItemName(path);
		DecorType type = stringToDecorType(fileName);
		parserMap[type].parseXML(path);
	}


	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			MapTile& tile = data[index];
			DecorType type = tile.decorType();

			if (type > DecorType::ANIMATION)
			{
				if (tile.has(DecorType::Torch))
					type = type &~DecorType::Torch;

				if (parserMap.count(type) > 0)
				{
					XMLParser& parser = parserMap.at(type);
					XMLNode root = parser.rootNode();
					XMLNode node = root.child("Animator");
					Animator animator = buildAnimation(node);

					if (root.child("Start"))
					{
						animator.start();
					}
					else if (root.child("Pause"))
					{
						animator.start();
						animator.pause();
					}
					else
					{
						animator.stop();
					}

					if (root.child("DeferredRender"))
					{
						tile.setDeferredRendering(true);
					}

					tile.addAnimation(animator);
				}
				else
				{
					DebugPrint(Warning, "No parser has been built for decor type %d. Does it exist in %s?\n", (int)type, fm->generatePath(FileManager::Configs_MapObjects));
				}
			}
		}
	}

	// HACK
	if (data[Index(13, 7)].has(DecorType::GratingTrigger))
	{
		data[Index(13, 7)].animations().clear();
	}
}




Animator AnimationTilePopulator::buildAnimation(const XMLNode& node)
{
	AnimationReader reader;
	return reader.buildAnimator(node);
}