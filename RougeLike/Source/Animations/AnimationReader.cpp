#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

bool AnimationReader::initAnimator(Animator& animator, const std::string& config)
{
	std::string configFilePath = FileManager::Get()->findFile(FileManager::Configs, config);

	if (!configFilePath.empty())
	{
		parser.parseXML(configFilePath);

		// Setup sprite sheet
		TilesetData data = buildTilesetData();
		Tileset spriteSheet(data);
#if _DEBUG
		spriteSheet.validate();
#endif

		// Setup animations
		Animations animationData = readAnimationData();

		if (animationData.size() >= 1)
		{
			animator.init(spriteSheet, animationData);
			return true;
		}
	}

	return false;
}


TilesetData AnimationReader::buildTilesetData() const
{
	TilesetData data;
	data.texture = readTexture(tm);
	data.tileSize = readTileSize();
	data.tileCount = readTileCount();
	return data;
}


Animations AnimationReader::readAnimationData() const
{
	Animations animations;

	xmlNode rootNode = parser.rootNode();
	xmlNode animationsParentNode = rootNode->first_node("Animations");
	xmlNode animationNode = animationsParentNode->first_node("Animation");

	while (animationNode != nullptr)
	{
		Attributes attributes = parser.attributes(animationNode);

		std::string action = attributes.getString("action");
		int startingIndex = attributes.getInt("start");
		int count = attributes.getInt("count");
		float rate = attributes.getFloat("rate");

		animations[action] = Animation(startingIndex, count, rate);;

		animationNode = animationNode->next_sibling("Animation");
	}

	return animations;
}



// -- Private Functions -- //
Vector2D<int> AnimationReader::readTileCount() const
{
	xmlNode rootNode = parser.rootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode tileCountNode = tileSetInfoNode->first_node("TileCount");

	Attributes attributes = parser.attributes(tileCountNode);

	int x = attributes.getInt("x");
	int y = attributes.getInt("y");

	return Vector2D<int>(x, y);
}


Vector2D<int> AnimationReader::readTileSize() const
{
	xmlNode rootNode = parser.rootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode tileSizeNode = tileSetInfoNode->first_node("TileSize");

	Attributes attributes = parser.attributes(tileSizeNode);

	int x = attributes.getInt("x");
	int y = attributes.getInt("y");

	return Vector2D<int>(x, y);
}


Texture* AnimationReader::readTexture(const TextureManager* tm) const
{
	return tm->getTexture(parser.firstRootNodeValue("Texture"), FileManager::Image_Animations);
}
