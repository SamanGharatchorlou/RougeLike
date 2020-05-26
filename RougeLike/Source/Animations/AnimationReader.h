#pragma once

#include "Animations/Animator.h"
#include "Graphics/TileSet.h"

class TextureManager;
class Texture;


class AnimationReader
{
public:
	AnimationReader(const TextureManager* textureManager) : tm(textureManager) { };
	bool initAnimator(Animator& animator, const std::string& config);


private:
	Animations readAnimationData() const;
	TilesetData buildTilesetData() const;

	Vector2D<int> readTileCount() const;
	Vector2D<int> readTileSize() const;
	Texture* readTexture(const TextureManager* tm) const;


private:
	XMLParser parser;
	const TextureManager* tm;
};