#pragma once

#include "AbilityTypes/Ability.h"
#include "Input/Button.h"

class AbilityManager;
class InputManager;
class Map;
class Environment;


class AbilityActivator
{
public:
	void init(Environment* environment) { mEnvironment = environment; }

	bool canSelect(Ability* ability) const;

	bool selected(Ability* ability, Button::State buttonState) const;
	bool released(Ability* ability, Button::State buttonState) const;

	bool activate(Ability* ability, Button::State buttonState, const InputManager* input);

	void activateAreaAttack(Ability* ability);


private:
	VectorF cursorWorldPosition(VectorF cursorPosition) const;
	const Map* map(VectorF position) const;

	bool activateOnSelf(TargetSelfAbility* ability);
	bool activateOnActor(TargetActorAbility* ability, const InputManager* input);
	bool activateOnPosition(TargetPositionAbility* ability, const InputManager* input);
	

private:
	Environment* mEnvironment;
};