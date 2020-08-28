#pragma once

#include "AbilityClasses/AbilityStates.h"
#include "AbilityActivator.h"
#include "AbilityHotKeyManager.h"
#include "AbilityBuilder.h"
#include "Events/LocalDispatcher.h"


class Actor;
class InputManager;
class Screen;


class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();

	void init(Environment* environment, Actor* caster, Screen* screen);
	void clear();

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void addAbility(AbilityType abilityType);

	void setState(Ability* ability, AbilityState state);

	bool inSelectionMode() const;

	LocalDispatcher& events() { return mEvents; }


private:
	void handleState(Ability* ability, float dt);


private:
	Environment* mEnvironment;

	LocalDispatcher mEvents;

	AbilityBuilder mBuilder;
	AbilityActivator mActivator;
	AbilityHotKeyManager mHotKeys;

	std::vector<Ability*> mAbilities;
};