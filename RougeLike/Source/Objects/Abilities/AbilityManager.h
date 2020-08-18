#pragma once

#include "AbilityClasses/AbilityStates.h"
#include "AbilityActivator.h"
#include "AbilityHotKeys.h"
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
	void slowUpdate(float dt);
	void render();

	void addAbility(const BasicString& name);
	void setState(Ability* ability, AbilityState state);

	bool inSelectionMode() const;

	Ability* get(const BasicString& name) const;

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }


private:
	void sendSetTextColourEvent(Ability* abiliy, Colour colour);

	void handleStates(Ability* ability, float dt);
	void handleEvents(Ability* ability);


private:
	Environment* mEnvironment;

	LocalDispatcher mEvents;
	
	AbilityBuilder mBuilder;
	AbilityActivator mActivator;
	AbilityHotKeys mHotKeys;

	std::vector<Ability*> mAbilities;
};