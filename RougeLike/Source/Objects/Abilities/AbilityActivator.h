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

	bool canSelect(Ability* ability) const;

	bool selected(Ability* ability, Button::State buttonState) const;
	bool released(Ability* ability, Button::State buttonState) const;

	bool activate(Ability* ability, Button::State buttonState, const InputManager* input) const;

	void activateAreaAttack(Ability* ability) const;


private:
	VectorF cursorWorldPosition(VectorF cursorPosition) const;
	const Map* map(VectorF position) const;

	bool activateOnSelf(TargetSelfAbility* ability) const;
	bool activateOnActor(TargetActorAbility* ability, const InputManager* input) const;
	bool activateOnPosition(TargetPositionAbility* ability, const InputManager* input) const;
	

private:
	Environment* mEnvironment;
};