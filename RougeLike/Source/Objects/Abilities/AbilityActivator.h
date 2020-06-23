#pragma once

#include "AbilityTypes/Ability.h"


class AbilityManager;
class Map;


class AbilityActivator
{
public:
	AbilityActivator(AbilityManager* manager, Ability* ability);


private:
	VectorF cursorPosition() const;
	const Map* map(VectorF position) const;

	void setState(Ability* ability, Ability::State state);

	void attemptActivation(Ability* ability);
	void attemptActivationOnSelf(Ability* ability);
	void attemptActivationOnSingleEnemy(Ability* ability);
	void attemptActivationOnArea(Ability* ability);
	void attemptActivationOnPoint(Ability* ability);

	void activateOnArea(AreaAbility* areaAbility);


private:
	AbilityManager* mManager;

};