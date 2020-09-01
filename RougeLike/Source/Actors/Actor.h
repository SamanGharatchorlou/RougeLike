#pragma once

#include "Physics.h"
#include "Animations/Animator.h"
#include "Collisions/Colliders/Collider.h"
#include "Objects/Effects/EffectManager.h"
#include "Events/LocalDispatcher.h"
#include "Utilities/Maps/AttributeMap.h"

class Map;
class Environment;


class Actor
{
public:
	Actor();
	virtual ~Actor() { clear(); };

	// init
	void set(Environment* environment);
	void setCharacter(const XMLNode node);
	void clear();

	// core loops
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render(VectorF offset);
	void render();


	// Attributes
	Attribute* getAttribute(AttributeType type) const;
	float getAttributeValue(AttributeType type) const;
	bool hasAttribute(AttributeType type) const;

	// Collider
	Collider* collider() { return &mCollider; }
	const Collider* collider() const { return &mCollider; }

	// Systems
	LocalDispatcher& events() { return mEvents; }
	Animator&	animator() { return mAnimator; }
	Physics*	physics() { return &mPhysics; }
	EffectManager& effects() { return mEffects; }
	const Physics* physics() const { return &mPhysics; }
	void updatePhysicsStats();

	// Position
	const Map*	currentMap() const;

	VectorF		position() const { return mPhysics.rect().Center(); }
	RectF&		rectRef() { return mPhysics.rectRef(); }
	RectF		rect() const { return mPhysics.rect(); }
	RectF		scaledRect() const;

	// Attacking
	virtual void attack() = 0;
	virtual bool isAttacking() const = 0;
	virtual Collider* attackingCollider() = 0;

	virtual void resetColliders() = 0;


	void addEffect(Effect* effect);
	void setVisibility(bool visibility) { mVisibility = visibility; }


protected:
	Environment* mEnvironment;

	EffectManager mEffects;

	AttributeMap mAttributeBag;

	Physics mPhysics;
	Collider mCollider;

	VectorF mRenderOffset;
	Animator mAnimator;

	LocalDispatcher mEvents;

	bool mVisibility;
};