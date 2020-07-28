#pragma once

#include "AbilityTypes/Ability.h"


class AbilityManager;
class InputManager;
class Map;
class Environment;


class AbilityActivator
{
public:
	void init(Environment* environment) { mEnvironment = environment; }

	bool selected(Ability* ability, const InputManager* input);
	bool released(Ability* ability, const InputManager* input);

	bool activate(Ability* ability, const InputManager* input);

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