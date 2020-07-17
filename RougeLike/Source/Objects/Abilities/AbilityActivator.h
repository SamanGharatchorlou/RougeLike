#pragma once

#include "AbilityTypes/Ability.h"


class AbilityManager;
class InputManager;
class Map;
class Environment;

class AbilityActivator
{
public:
	AbilityActivator(Environment* environment);

	bool activate(Ability* ability);
	void activateAreaAttack(Ability* ability);


private:
	VectorF cursorPosition() const;
	const Map* map(VectorF position) const;

	bool activateOnSelf(TargetSelfAbility* ability);
	bool activateOnActor(TargetActorAbility* ability);
	bool activateOnPosition(TargetPositionAbility* ability);
	

private:
	Environment* mEnvironment;
};