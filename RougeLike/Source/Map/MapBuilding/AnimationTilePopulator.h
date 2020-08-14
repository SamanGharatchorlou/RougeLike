#pragma once

#include "Animations/Animator.h"

class MapTile;

class AnimationTilePopulator
{
public:
	void addAnimations(Grid<MapTile>& data);

private:
	Animator buildAnimation(const XMLNode& node);
};