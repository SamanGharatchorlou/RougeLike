#pragma once

#include "Animations/Animator.h"
#include "Graphics/TileSet.h"


class TextureManager;
class Texture;

class AnimationReader
{
public:
	AnimationReader(std::string file, TextureManager* textureManager);

	Animations readAnimationData();
	TilesetData readTilesetData();

private:
	Vector2D<int> readTileCount();
	Vector2D<int> readTileSize();
	Texture* readTexture();

private:
	XMLParser parser;
	TextureManager* mTextureManager;
};