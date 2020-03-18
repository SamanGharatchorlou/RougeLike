#pragma once

#include "Animations/Animator.h"
#include "Graphics/TileSet.h"


class TextureManager;
class Texture;

class AnimationReader
{
public:
	AnimationReader(const std::string& file);

	Animations readAnimationData() const;
	TilesetData readTilesetData(const TextureManager* textureManager) const;

private:
	Vector2D<int> readTileCount() const;
	Vector2D<int> readTileSize() const;
	Texture* readTexture(const TextureManager* tm) const;

private:
	XMLParser parser;
};