#pragma once

#include "Animations/Animator.h"


class AnimationReader
{
public:
	Animator buildAnimator(const XMLNode animationNode) const;
};
