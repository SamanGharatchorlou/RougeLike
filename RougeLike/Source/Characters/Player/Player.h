#pragma once
#include "States/PlayerState.h" // TODO: only for enum actionstate type, could move it??

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
	~Player() { }

	// TODO: change all 'std::string' parameters to 'const std::string&'
	void init(const std::string& characterConfig);
	void processStateChanges();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void equiptWeapon(const WeaponData* data);
	void updateWeaponStats(const PlayerPropertyBag* bag);

	// Movement states
	void updateState();

	// Attack states
	void addState(PlayerState::actionState newState);
	void popState();

	RectF&		getRect()		{ return physics.getRect(); }
	Collider&	getCollider()	{ return mCollider; }
	Physics&	getPhysics()	{ return physics; }

	Weapon*		getWeapon()		{ return mWeapon; }

	PlayerPropertyBag* propertyBag() { return mBag; }


private:
	// Animations
	void selectAnimation(PlayerState::actionState state);
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

	PlayerState::actionState mState;
};

