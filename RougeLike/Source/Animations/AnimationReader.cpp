#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"


bool AnimationReader::initAnimator(Animator& animator, const std::string& config)
{
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

	xmlNode rootNode = mParser.rootNode();
	xmlNode animationsParentNode = rootNode->first_node("Animations");
	xmlNode animationNode = animationsParentNode->first_node("Animation");

	while (animationNode != nullptr)
	{
		Attributes attributes = mParser.attributes(animationNode);

		std::string action = attributes.getString("action");
		int startingIndex = attributes.getInt("start");
		int count = attributes.getInt("count");
		float rate = attributes.getFloat("rate");

		animations[action] = AnimationInfo(startingIndex, count, rate);;

		animationNode = animationNode->next_sibling("Animation");
	}

	return animations;
}



// -- Private Functions -- //
Vector2D<int> AnimationReader::readTileCount() const
{
	xmlNode rootNode = mParser.rootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode tileCountNode = tileSetInfoNode->first_node("TileCount");

	Attributes attributes = mParser.attributes(tileCountNode);

	int x = attributes.getInt("x");
	int y = attributes.getInt("y");

	return Vector2D<int>(x, y);
}


Vector2D<int> AnimationReader::readTileSize() const
{
	xmlNode rootNode = mParser.rootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode tileSizeNode = tileSetInfoNode->first_node("TileSize");

	Attributes attributes = mParser.attributes(tileSizeNode);

	int x = attributes.getInt("x");
	int y = attributes.getInt("y");

	return Vector2D<int>(x, y);
}


Texture* AnimationReader::readTexture(const TextureManager* tm) const
{
	return tm->getTexture(mParser.firstRootNodeValue("Texture"), FileManager::Image_Animations);
}





bool AnimationReader::initAnimator2(Animator2& animator)
{
	float frameSpeed = std::stof(mParser.firstRootNodeValue("FrameSpeed"));
	animator.setFrameSpeed(frameSpeed);

	std::string id = mParser.firstRootNodeValue("ID");

	xmlNode animations = mParser.rootNode()->first_node("Animations");
	xmlNode actionNode = animations->first_node();

	while (actionNode != nullptr)
	{
		AnimationData data;

		std::string action = actionNode->name();
		std::string fileName = id + "_" + action;
		Texture* texture = tm->getTexture(fileName, FileManager::Image_Animations);
		data.texture = texture;

		xmlNode frameSize = actionNode->first_node("FrameSize");
		Vector2D<int> size = getXYAttributes(frameSize);
		data.tileDimentions = size;

		xmlNode frameCount = actionNode->first_node("FrameCount");
		int count = std::stoi(frameCount->value());
		data.frameCount = count;

		data.action = stringToAction(action);

		animator.addAnimation(data);

		actionNode = actionNode->next_sibling();
	}

	return false;
}



Vector2D<int> AnimationReader::getXYAttributes(xmlNode node)
{
	Attributes attributes = mParser.attributes(node);
	int x = attributes.getInt("x");
	int y = attributes.getInt("y");
	return Vector2D<int>(x, y);
}