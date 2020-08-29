#pragma once

#include "AbilityActivator.h"
#include "Events/LocalDispatcher.h"


class Ability;
class EffectPool;
enum class AbilityType;
enum class AbilityState;


class AbilityHandler
{
public:
	AbilityHandler() { }

	void init(EffectPool* effectPool, std::vector<Actor*>* targets);

	const AbilityActivator& activator() const { return mActivator; }

	Ability* get(AbilityType type);
	void add(Ability* ability);

	const std::vector<Ability*>& abilities() const { return mAbilities; }

	void clear();

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();


private:
	void handleState(Ability* ability, float dt);


private:
	LocalDispatcher mEvents;
	AbilityActivator mActivator;

	std::vector<Ability*> mAbilities;
};