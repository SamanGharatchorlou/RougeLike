#pragma once

#include "Objects/Attributes/Physics.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"

struct GameData;
class PropertyBag;

class Actor
{
public:
	Actor(GameData* gameData) : mGameData(gameData) { }

	void init(const std::string& characterConfig);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void setPropertyBag(PropertyBag* bag) { mPropertyBag = bag; }
	PropertyBag* propertyBag() const { return mPropertyBag; }
	float getPropertyValue(const std::string& property) const;

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

	PropertyBag* mPropertyBag;

	Animator mAnimator;
	Collider mCollider;
	Physics mPhysics;
};