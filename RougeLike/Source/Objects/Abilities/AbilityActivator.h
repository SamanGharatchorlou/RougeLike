#pragma once

#include "Input/Button.h"

class InputManager;
class Map;
class Environment;
class Ability;
class TargetSelfAbility;
class TargetActorAbility;
class TargetPositionAbility;


class AbilityActivator
{
public:
	void init(Environment* environment) { mEnvironment = environment; }
	void clear() { mEnvironment = nullptr; }

	bool selected(Ability* ability, Button::State buttonState) const;
	bool deselected(Ability* ability, Button::State buttonState) const;

	bool activate(Ability* ability, Button::State buttonState, const InputManager* input) const;

	void activateCollisions(Ability* ability) const;


private:
	VectorF cursorWorldPosition(VectorF cursorPosition) const;
	const Map* map(VectorF position) const;
	

private:
	Environment* mEnvironment;
};