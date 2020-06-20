#pragma once

#include "Physics.h"
#include "Animations/Animator.h"
#include "Collisions/EffectCollider.h"
#include "Objects/Effects/EffectHandler.h"
#include "Events/LocalDispatcher.h"

#include "Objects/Properties/PropertyBag.h"

struct GameData;
class Property;
class Map;


class Actor
{
public:
	Actor(GameData* gameData);
	virtual ~Actor();

	void init(const std::string& config);
	void fastUpdate(float dt);
	virtual void effectLoop() { };
	void slowUpdate(float dt);
	void render();

	virtual void setVisibility(bool visibility) { mVisibility = visibility; }
	void reset();

	// Event handling
	EventPacket popEvent() { return mEvents.pop(); }
	void pushEvent(EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.hasEvent(); }

	// PropertyBag
	Property* getProperty(const std::string& property) const;
	float getPropertyValue(const std::string& property) const;
	bool hasProperty(const std::string& property) const;

	// Collider
	EffectCollider* collider() { return &mCollider; }
	const EffectCollider* collider() const { return &mCollider; }

	// Systems
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

	// Effects
	const EffectPropertyBag* effectProperties() const { return &mEffectProperties; }
	void setEffectProperty(const std::string& name, float value) { mEffectProperties.setProperty(name, value); }

	void addEffect(Effect* effect);


protected:
	void processEffects(EffectCollider* effectCollider);


protected:
	GameData* mGameData;

	// TODO: make this into the actors own property bag
	// Create another property bag where the effects can pull from?
	// See armor ability comments
	PropertyBag mPropertyBag;

	EffectCollider mCollider;

	Animator mAnimator;

	Physics mPhysics;

	EffectHandler mEffects;

	EffectPropertyBag mEffectProperties;

	LocalDispatcher mEvents;

	bool mVisibility;
};