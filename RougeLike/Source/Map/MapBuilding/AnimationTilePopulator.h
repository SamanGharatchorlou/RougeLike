#pragma once

#include "Animations/Animator.h"

class MapTile;
class TextureManager;

class AnimationTilePopulator
{
public:
	void addAnimations(Grid<MapTile>& data, const TextureManager* textures);

private:
	Animator buildAnimation(const XMLNode& node, const TextureManager* textures);
};