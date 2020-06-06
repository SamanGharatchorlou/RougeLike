#pragma once

#include "Events/LocalDispatcher.h"
#include "Objects/Abilities/Ability.h"

class GameData;
class Ability;
class Actor;
class InputManager;
class UIManager;


class AbilityManager
{
public:
	AbilityManager(GameData* gameData) : mGameData(gameData) { }

	void handleInput();
	void slowUpdate(float dt);
	void render();

	void add(const std::string& name, Ability* ability);
	void activate(const std::string& ability);
	void setState(const std::string& ability, Ability::State state);

	void exitSelection();
	bool inSelectionMode() const;
	
	bool hasAbility(const std::string& ability) const { return mAbilities.count(ability) > 0; }

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }
	

private:
	void completeSelection(std::unordered_map<std::string, Ability*>::iterator iter);
	void attemptActivation(Ability* ability);


private:
	GameData* mGameData;
	LocalDispatcher mEvents;

	std::unordered_map<std::string, Ability*> mAbilities;
};