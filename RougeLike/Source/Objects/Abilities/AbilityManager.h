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

	void add(Ability* ability);
	bool canActivate(const std::string& name);
	void activate(const std::string& name);
	void setState(const std::string& name, Ability::State state);

	void exitSelection();
	bool inSelectionMode() const;

	Ability* get(const std::string& name) const;
	bool hasAbility(const std::string& name) const;

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }

	Ability* createNewAbility(const std::string& name);

private:
	void completeSelection(Ability* ability);
	void attemptActivation(Ability* ability);


private:
	GameData* mGameData;
	LocalDispatcher mEvents;

	std::vector<Ability*> mAbilities;
};