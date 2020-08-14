#pragma once

#include "Physics.h"
#include "Animations/Animator.h"
#include "Collisions/EffectCollider.h"
#include "Objects/Effects/EffectManager.h"
#include "Events/LocalDispatcher.h"
#include "Utilities/Maps/AttributeMap.h"

class Map;
class Environment;


class Actor
{
public:
	Actor();
	virtual ~Actor() { };

	void set(Environment* environment);

	void setCharacter(const XMLNode node);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void setVisibility(bool visibility) { mVisibility = visibility; }
	void clear();

	// StringMap
	Attribute* getAttribute(AttributeType type) const;
	float getAttributeValue(AttributeType type) const;
	bool hasAttribute(AttributeType type) const;

	// Collider
	EffectCollider* collider() { return &mCollider; }
	const EffectCollider* collider() const { return &mCollider; }

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


	void addEffect(Effect* effect);



protected:
	void handleEffects(EffectCollider* effectCollider);


protected:
	Environment* mEnvironment;

	EffectManager mEffects;

	AttributeMap mAttributeBag;

	Physics mPhysics;
	EffectCollider mCollider;

	Animator mAnimator;

	LocalDispatcher mEvents;

	bool mVisibility;
};