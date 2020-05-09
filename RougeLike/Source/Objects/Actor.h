#pragma once

#include "Objects/Attributes/Physics.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"

struct GameData;

class Actor
{
public:
	Actor(GameData* gameData) : mGameData(gameData) { }

	void init(const std::string& characterConfig);
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	Animator&	animator() { return mAnimator; }
	Collider&	collider() { return mCollider; }
	Physics&	physics() { return mPhysics; }

	RectF&		rectRef() { return mPhysics.rectRef(); }
	RectF		rect() const { return mPhysics.rect(); }

	virtual RectF		renderRect() const = 0;

protected:
	void initAnimations(const std::string& config);


protected:
	GameData* mGameData;

	Animator mAnimator;
	Collider mCollider;
	Physics mPhysics;
};