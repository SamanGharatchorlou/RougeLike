#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"



bool AnimationReader::initAnimator(Animator& animator)
{
	xmlNode animationNode = mParser.rootNode()->first_node("AnimationInfo");

	float frameTime = atof(mParser.nodeValue(animationNode, "FrameTime").c_str());
	animator.setFrameTime(frameTime);

	BasicString id = mParser.nodeValue(animationNode, "ID");

	xmlNode animations = animationNode->first_node("Animations");
	xmlNode node = animations->first_node();

	while (node != nullptr)
	{
		AnimationData data = readData(node);

		animator.addAnimation(data);
		node = node->next_sibling();
	}

	return (bool)animator.animationCount();
}


// -- Private Functions -- //
AnimationData AnimationReader::readData(xmlNode node)
{
	AnimationData data;
	xmlNode animationNode = mParser.rootNode()->first_node("AnimationInfo");

	// Texture
	BasicString id = mParser.nodeValue(animationNode, "ID");
	BasicString action = node->name();
	BasicString fileName = id + "_" + action;
	Texture* texture = tm->getTexture(fileName, FileManager::Image_Animations);
	data.texture = texture;

	// Size
	xmlNode frameSize = animationNode->first_node("FrameSize");
	Vector2D<int> size = getXYAttributes(frameSize);
	data.tileDimentions = size;

	// Count
	int count = std::stoi(node->value());
	data.frameCount = count;

	data.action = stringToAction(action);

	return data;

}


Vector2D<int> AnimationReader::getXYAttributes(xmlNode node)
{
	Attributes attributes = mParser.attributes(node);
	int x = attributes.getInt("x");
	int y = attributes.getInt("y");
	return Vector2D<int>(x, y);
}