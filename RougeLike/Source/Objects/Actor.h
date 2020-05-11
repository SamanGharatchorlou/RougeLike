#pragma once

#include "Objects/Attributes/Physics.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"

struct GameData;
class PropertyBag;
class Property;

class Actor
{
public:
	Actor(GameData* gameData) : mGameData(gameData) { }

	void init(const std::string& characterConfig);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	// PropertyBag
	void setPropertyBag(PropertyBag* bag) { mPropertyBag = bag; }
	PropertyBag* propertyBag() const { return mPropertyBag; }
	Property* getProperty(const std::string& property) const;
	float getPropertyValue(const std::string& property) const;

	// Collider
	void setCollider(Collider* collider) { mCollider = collider; }
	Collider*	collider() { return mCollider; }

	Animator&	animator() { return mAnimator; }
	Physics*	physics()	 { return &mPhysics; }
	const Physics* physics() const { return &mPhysics; }

	VectorF		position() const { return mPhysics.rect().Center(); }
	RectF&		rectRef() { return mPhysics.rectRef(); }
	RectF		rect() const { return mPhysics.rect(); } 

	virtual RectF		renderRect() const = 0;

protected:
	void initAnimations(const std::string& config);


protected:
	GameData* mGameData;

	PropertyBag* mPropertyBag;

	Collider* mCollider;

	Animator mAnimator;
	Physics mPhysics;
};