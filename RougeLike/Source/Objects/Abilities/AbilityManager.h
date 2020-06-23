#pragma once

#include "Events/LocalDispatcher.h"
#include "AbilityHotKeys.h"
#include "AbilityTypes/Ability.h"

class GameData;


class AbilityManager
{
	friend class AbilityActivator;

public:
	AbilityManager(GameData* gameData, Actor* parent);

	void handleInput();
	void slowUpdate(float dt);
	void render();

	void add(Ability* ability);
	void setState(Ability* ability, Ability::State state);

	void exitSelection();
	void exitSelection(Ability* ability);
	bool inSelectionMode() const;

	Ability* get(const std::string& name) const;

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }

	void sendSetTextColourEvent(Ability* abiliy, Colour colour);


private:
	void handleStates(Ability* ability, float dt);
	void handleEvents(Ability* ability);

	void completeSelection(Ability* ability);


private:
	GameData* mGameData;
	Actor* mCaster;

	LocalDispatcher mEvents;

	AbilityHotKeys mHotKeys;

	std::vector<Ability*> mAbilities;
};