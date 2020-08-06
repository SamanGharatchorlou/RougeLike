#pragma once

#include "Physics.h"
#include "Animations/Animator.h"
#include "Collisions/EffectCollider.h"
#include "Objects/Effects/EffectHandler.h"
#include "Events/LocalDispatcher.h"

#include "Objects/Properties/PropertyBag.h"

// temp
#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Effects/EffectManager.h"


class Property;
class Map;
class Environment;
class TextureManager;


class Actor
{
public:
	Actor();
	virtual ~Actor() { };

	void set(Environment* environment);

	void setCharacter(const XMLParser& parser, const TextureManager* textureManager);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	virtual void setVisibility(bool visibility) { mVisibility = visibility; }
	void reset();

	// PropertyBag
	Property* getProperty(const BasicString& property) const;
	float getPropertyValue(const BasicString& property) const;
	bool hasProperty(const BasicString& property) const;

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

	PropertyBag mPropertyBag;

	Physics mPhysics;
	EffectCollider mCollider;

	Animator mAnimator;

	LocalDispatcher mEvents;

	bool mVisibility;
};