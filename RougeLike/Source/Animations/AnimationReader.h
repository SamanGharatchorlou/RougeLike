#pragma once

#include "Animations/Animator.h"
#include "Graphics/TileSet.h"

class TextureManager;
class Texture;


class AnimationReader
{
public:
	AnimationReader(const TextureManager* textureManager, XMLParser& parser) : tm(textureManager), mParser(parser) { };
	bool initAnimator(Animator& animator, const std::string& config);

	bool initAnimator2(Animator2& animator);

private:
	Animations readAnimationData() const;
	TilesetData buildTilesetData() const;

	Vector2D<int> readTileCount() const;
	Vector2D<int> readTileSize() const;
	Texture* readTexture(const TextureManager* tm) const;


	Vector2D<int> getXYAttributes(xmlNode node);


private:
	XMLParser& mParser;
	const TextureManager* tm;
};