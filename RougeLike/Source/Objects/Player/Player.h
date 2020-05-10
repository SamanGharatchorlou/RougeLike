#pragma once

#include "Objects/Actor.h"
#include "States/StateMachine.h"

class Weapon;
class PlayerPropertyBag;


class Player : public Actor
{
public:
	Player(GameData* gameData);
	~Player();

	void reset();

	void init(const std::string& characterConfig);
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	// Weapon
	void equiptWeapon(Weapon* weapon);

	// Movement states
	void updateState();

	Weapon*		weapon()		{ return mWeapon; }

private:
	// Animations
	void selectAnimation();

	RectF renderRect() const; // TODO: move this into physics, enemy also has this but isn't using physics yet


private:
	StateMachine<State> mStateMachine;

	Weapon* mWeapon;

	bool mMoving;
};

