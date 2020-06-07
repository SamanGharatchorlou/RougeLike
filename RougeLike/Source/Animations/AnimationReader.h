#pragma once

#include "Animations/Animator.h"
#include "Graphics/TileSet.h"

class TextureManager;
class Texture;


class AnimationReader
{
public:
	AnimationReader(const TextureManager* textureManager, XMLParser& parser) : tm(textureManager), mParser(parser) { };
	bool initAnimator(Animator& animator);


private:
	AnimationData readData(xmlNode node);
	Vector2D<int> getXYAttributes(xmlNode node);


private:
	XMLParser& mParser;
	const TextureManager* tm;
};