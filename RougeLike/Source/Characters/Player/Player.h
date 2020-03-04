#pragma once

#include "Characters/Attributes/Physics.h"
#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"

struct GameData;
class Weapon;
struct WeaponData;
class PlayerPropertyBag;


class Player
{
public:
	Player(GameData* gameData);
	~Player();

	// TODO: change all 'std::string' parameters to 'const std::string&'
	void init(const std::string& characterConfig);
	void processStateChanges() { }
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	// Weapon
	void equiptWeapon(Weapon* weapon);
	void updateWeaponStats(const PlayerPropertyBag* bag);

	// Movement states
	void updateState();

	RectF&		getRect()		{ return physics.getRect(); }
	Collider&	getCollider()	{ return mCollider; }
	Physics&	getPhysics()	{ return physics; }

	Weapon*		getWeapon()		{ return mWeapon; }

	PlayerPropertyBag* propertyBag() { return mBag; }


private:
	// Animations
	void selectAnimation();
	void initAnimations(const std::string& config);


private:
	GameData* mGameData;

	PlayerPropertyBag* mBag;

	StateMachine stateMachine;
	Animator mAnimator;
	Collider mCollider;
	Physics physics;

	Weapon* mWeapon;

	SDL_RendererFlip mFlip;

	bool mMoving;
};

