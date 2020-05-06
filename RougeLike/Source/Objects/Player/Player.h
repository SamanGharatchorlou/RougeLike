#pragma once

#include "Objects/Attributes/Physics.h"
#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"

struct GameData;
class Weapon;
class PlayerPropertyBag;


class Player
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
	void updateWeaponStats(const PlayerPropertyBag* bag);

	// Movement states
	void updateState();

	RectF&		rectRef()	{ return mPhysics.rectRef(); }
	RectF		rect() const { return mPhysics.rect(); }

	Collider&	collider()	{ return mCollider; }
	Physics&	physics()	{ return mPhysics; }

	Weapon*		weapon()		{ return mWeapon; }

	PlayerPropertyBag* propertyBag() { return mBag; }


private:
	// Animations
	void selectAnimation();
	void initAnimations(const std::string& config);

	RectF renderRect() const; // TODO: move this into physics, enemy also has this but isn't using physics yet

private:
	GameData* mGameData;

	PlayerPropertyBag* mBag; // TODO: does this need to be a pointer?

	StateMachine<State> mStateMachine;
	Animator mAnimator;
	Collider mCollider;
	Physics mPhysics;

	Weapon* mWeapon;

	SDL_RendererFlip mFlip;

	bool mMoving;
};

