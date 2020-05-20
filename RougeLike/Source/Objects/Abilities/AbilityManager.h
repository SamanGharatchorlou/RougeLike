#pragma once

#include "Events/LocalDispatcher.h"

class GameData;
class Ability;
class Actor;
class InputManager;
class UIManager;
 

class AbilityManager
{
public:
	AbilityManager(GameData* gameData) : mGameData(gameData), mActiveAbility(nullptr) { }

	void handleInput();
	void slowUpdate();

	void add(std::string name, Ability* ability);
	void select(const std::string& ability);

	void activate(Actor* target);

	void endSelectionMode();
	bool inSelectionMode() const { return mActiveAbility != nullptr; }

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }
	
private:
	void activateActiveAbility();


private:
	GameData* mGameData;

	std::unordered_map<std::string, Ability*> mAbilities;
	Ability* mActiveAbility;

	LocalDispatcher mEvents;
};