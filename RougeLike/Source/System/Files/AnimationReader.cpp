#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

// TODO: both the enemy and player use the same init function kind of thing where they setup an animator
// move that logic into here
AnimationReader::AnimationReader(std::string file, TextureManager* textureManager) : 
	mTextureManager(textureManager)
{
	std::string configFilePath = FileManager::Get()->getFilePath(FileManager::Config_Animations, file);
	parser.parseXML(configFilePath);
}


Animations AnimationReader::readAnimationData()
{
	Animations animations;

	xmlNode rootNode = parser.getRootNode();
	xmlNode animationsParentNode = rootNode->first_node("Animations");
	xmlNode animationNode = animationsParentNode->first_node("Animation");

	while (animationNode != nullptr)
	{
		Attributes attributes = parser.getAttributes(animationNode);

		std::string action = attributes.getString("action");
		int startingIndex = attributes.getInt("start");
		int count = attributes.getInt("count");
		float rate = attributes.getFloat("rate");

		animations[action] = Animation(startingIndex, count, rate);;

		animationNode = animationNode->next_sibling("Animation");
	}

	return animations;
}


TilesetData AnimationReader::readTilesetData()
{
	TilesetData data;

	data.texture = readTexture();
	data.tileSize = readTileSize();
	data.tileCount = readTileCount();

	return data;
}


// -- Private Functions -- //
Vector2D<int> AnimationReader::readTileCount()
{
	xmlNode rootNode = parser.getRootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode tileSizeNode = tileSetInfoNode->first_node("TileCount");

	Attributes attributes = parser.getAttributes(tileSizeNode);

	int tileCountX = attributes.getInt("x");
	int tileCountY = attributes.getInt("y");

	return Vector2D<int>(tileCountX, tileCountY);
}


Vector2D<int> AnimationReader::readTileSize()
{
	xmlNode rootNode = parser.getRootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode tileSizeNode = tileSetInfoNode->first_node("TileSize");

	Attributes attributes = parser.getAttributes(tileSizeNode);

	int tileCountX = attributes.getInt("x");
	int tileCountY = attributes.getInt("y");

	return Vector2D<int>(tileCountX, tileCountY);
}


Texture* AnimationReader::readTexture()
{
	xmlNode rootNode = parser.getRootNode();
	std::string textureName = rootNode->first_node("TextureName")->value();

	return mTextureManager->getTexture(textureName);
}
