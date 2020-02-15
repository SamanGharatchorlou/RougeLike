#pragma once

#include "Animations/Animator.h"
#include "Collisions/Collider.h"

#include "Characters/Attributes/Health.h"
#include "Characters/Attributes/Damage.h"
#include "Characters/Attributes/Movement.h"

#include "States/StateMachine.h"
#include "EnemyEnums.h"

// Testing
#include "Events/Dispatcher.h"

struct GameData;
class EnemyPropertyBag;


class Enemy : public Dispatcher
{
public:
	Enemy(GameData* gameData);
	~Enemy();

	void init(std::string name);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addState(EnemyState state);
	void popState() { mStateMachine.popState(); }
	void replaceState(EnemyState state);

	virtual void resolvePlayerWeaponCollisions() = 0;

	const GameData* getData() { return mGameData; }

	void		setRect(RectF rect) { mRect = rect; }
	RectF&		getRect() { return mRect; }

	StateMachine* getStateMachine() { return &mStateMachine; }
	Animator*	getAnimator() { return &mAnimator; }
	Collider*	getCollider() { return &mCollider; }
	Movement&	getMovement() { return mMovement; }

	void		move(float dt);

	Health&		health() const;
	bool		isDead() const;

	Damage&		damage() const;

	float hurtTime() const;
	float chaseRange() const;
	float sightRange() const;
	float attentionTime() const;
	float movementSpeed() const;
	float tackleSpeed() const;
	float tackleDistance() const;
	float tackleChargeTime() const;
	int score() const;

	const RectF		targetRect() const;

	void spawn(VectorF position);

	virtual const EnemyType type() const = 0;

	Uint8& alpha() { return mAlpha; }

	void setActive(bool active) { mIsActive = active; }
	bool isActive() const { return mIsActive; }


private:
	void initAnimations(std::string name);
	void readAttributes(std::string name);


protected:
	GameData* mGameData;
	
	StateMachine mStateMachine;
	Collider mCollider;
	Animator mAnimator;

	Movement mMovement;
	RectF mRect; // world coords
	SDL_RendererFlip mFlip;

	EnemyPropertyBag* bag;

	Uint8 mAlpha;

	bool mIsActive;
};