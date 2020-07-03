#pragma once

#include "AbilityTypes/Ability.h"


class AbilityManager;
class InputManager;
class Map;


class AbilityActivator
{
public:
	AbilityActivator(AbilityManager* manager);

	bool shouldActivate(Ability* ability, InputManager* input);

	bool activate(Ability* ability);
	void activateAreaAttack(Ability* ability);


private:
	VectorF cursorPosition() const;
	const Map* map(VectorF position) const;

	void setState(Ability* ability, Ability::State state);

	bool activateOnSelf(TargetSelfAbility* ability);
	bool activateOnActor(TargetActorAbility* ability);
	bool activateOnPosition(TargetPositionAbility* ability);
	

private:
	AbilityManager* mManager;
	InputManager* mInput;

};