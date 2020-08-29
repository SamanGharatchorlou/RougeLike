#pragma once

#include "Input/Button.h"

class InputManager;
class Ability;
class EffectPool;
class Actor;


class AbilityActivator
{
public:
	AbilityActivator() : mEffectPool(nullptr), mTargets(nullptr) { }

	void init(EffectPool* effectPool, std::vector<Actor*>* targets);
	void clear() { mTargets = nullptr; }

	bool activate(Ability* ability, Button::State buttonState, const InputManager* input) const;
	void activateCollisions(Ability* ability) const;

private:
	VectorF cursorWorldPosition(VectorF cursorPosition) const;
	

private:
	EffectPool* mEffectPool;
	std::vector<Actor*>* mTargets;
};