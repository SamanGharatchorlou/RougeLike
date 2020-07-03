#pragma once

#include "Events/LocalDispatcher.h"
#include "AbilityActivator.h"
#include "AbilityHotKeys.h"
#include "AbilityTypes/Ability.h"


class GameData;
class Actor;


class AbilityManager
{
	friend class AbilityHotKeys;
	friend class AbilityActivator;

public:
	AbilityManager(GameData* gameData, Actor* parent);

	void handleInput();
	void slowUpdate(float dt);
	void render();

	void add(const std::string& name);
	void setState(Ability* ability, Ability::State state);

	bool inSelectionMode() const;

	Ability* get(const std::string& name) const;

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }


private:
	void sendSetTextColourEvent(Ability* abiliy, Colour colour);

	void handleStates(Ability* ability, float dt);
	void handleEvents(Ability* ability);


private:
	GameData* mGameData;
	Actor* mCaster;

	LocalDispatcher mEvents;

	AbilityActivator mActivator;
	AbilityHotKeys mHotKeys;

	std::vector<Ability*> mAbilities;
};