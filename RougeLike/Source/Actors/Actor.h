#pragma once

#include "Physics.h"
#include "Animations/Animator.h"
#include "Collisions/EffectCollider.h"
#include "Objects/Effects/EffectHandler.h"
#include "Events/LocalDispatcher.h"

#include "Objects/Properties/PropertyBag.h"

// temp
#include "Objects/Effects/EffectTypes/Effect.h"

struct GameData;
class Property;
class Map;
class EffectPool;
class Environment;
class TextureManager;


class Actor
{
public:
	Actor();
	virtual ~Actor() { };

	void set(Environment* environment) { mEnvironment = environment; }

	void setCharacter(const XMLParser& parser, TextureManager* textureManager);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	virtual void setVisibility(bool visibility) { mVisibility = visibility; }
	void reset();

	// PropertyBag
	Property* getProperty(const std::string& property) const;
	float getPropertyValue(const std::string& property) const;
	bool hasProperty(const std::string& property) const;

	// Collider
	EffectCollider* collider() { return &mCollider; }
	const EffectCollider* collider() const { return &mCollider; }

	// Systems
	LocalDispatcher& events() { return mEvents; }
	Animator&	animator() { return mAnimator; }
	Physics*	physics() { return &mPhysics; }
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

	EffectHandler mEffects;

	PropertyBag mPropertyBag;

	Physics mPhysics;
	EffectCollider mCollider;

	Animator mAnimator;

	LocalDispatcher mEvents;

	bool mVisibility;
};