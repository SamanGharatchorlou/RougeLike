#pragma once

#include "Events/LocalDispatcher.h"
#include "AbilityHotKeys.h"
#include "Ability.h"

class GameData;


class AbilityManager
{
public:
	AbilityManager(GameData* gameData);

	void handleInput();
	void slowUpdate(float dt);
	void render();

	void add(Ability* ability);
	void setState(const std::string& name, Ability::State state);
	void setState(Ability* ability, Ability::State state);

	void exitSelection();
	void exitSelection(Ability* ability);
	bool inSelectionMode() const;

	Ability* get(const std::string& name) const;

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }

	Ability* createNewAbility(const std::string& name);

	void sendSetTextColourEvent(Ability* abiliy, Colour colour);


private:
	void handleStates(Ability* ability, float dt);
	void handleEvents(Ability* ability);

	void completeSelection(Ability* ability);

	void attemptActivation(Ability* ability);
	void attemptActivationOnSelf(Ability* ability);
	void attemptActivationOnSingleEnemy(Ability* ability);
	void attemptActivationOnArea(Ability* ability);
	void attemptActivationOnPoint(Ability* ability);

	void activateOnArea(AreaAbility* areaAbility);


private:
	GameData* mGameData;
	LocalDispatcher mEvents;

	AbilityHotKeys mHotKeys;

	std::vector<Ability*> mAbilities;
};