#pragma once

#include "Animations/Animator.h"


class TextureManager;


class AnimationReader
{
public:
	Animator buildAnimator(const XMLNode animationNode, const TextureManager* textureManager) const;

};
