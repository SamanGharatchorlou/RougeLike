#pragma once

#include "Events/LocalDispatcher.h"
#include "AbilityActivator.h"
#include "AbilityHotKeys.h"
#include "AbilityTypes/Ability.h"


struct GameData;
class GameScreen;
class Actor;
class TextureManager;
class InputManager;
class EffectPool;
class Environment;
class Screen;


class AbilityManager
{
public:
	AbilityManager(TextureManager* textures, Actor* caster, Screen* screen);

	void init(Environment* environment);


	void handleInput(const InputManager* input);
	void slowUpdate(float dt);
	void render();

	void add(const std::string& name);
	void setState(Ability* ability, Ability::State state);

	bool inSelectionMode() const;

	Ability* get(const std::string& name) const;

	bool hasEvent() const { return mEvents.hasEvent(); }
	EventPacket popEvent() { return mEvents.pop(); }


private:
	void addAbility(Ability* ability);

	void sendSetTextColourEvent(Ability* abiliy, Colour colour);

	void handleStates(Ability* ability, float dt);
	void handleEvents(Ability* ability);


private:
	Actor* mCaster;

	Environment* mEnvironment;
	TextureManager* mTextures;

	LocalDispatcher mEvents;

	AbilityActivator mActivator;
	AbilityHotKeys mHotKeys;

	std::vector<Ability*> mAbilities;
};