#pragma once

#include "AbilityBuilder.h"
#include "AbilityHotKeyManager.h"
#include "AbilityHandler.h"
#include "Events/LocalDispatcher.h"


class Actor;
class InputManager;
class Screen;
class Cursor;
enum class AbilityState;


class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();

	void init(Actor* caster, Screen* screen, EffectPool* effectPool, std::vector<Actor*>* targets);
	void clear();

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void addAbility(AbilityType type);
	Ability* get(AbilityType type);

	bool inSelectionMode() const;

	LocalDispatcher& events() { return mEvents; }


private:
	void handleBasicAbility(const InputManager* input);
	void updateCursor();


private:
	LocalDispatcher mEvents;

	Actor* mCaster;
	AbilityBuilder mBuilder;
	AbilityHotKeyManager mHotKeys;
	AbilityHandler mHandler;

	bool mUpdateCursor;
};