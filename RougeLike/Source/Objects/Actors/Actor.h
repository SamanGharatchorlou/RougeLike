#pragma once

#include "Physics.h"
#include "Animations/Animator.h"
#include "Objects/Effects/EffectHandler.h"
#include "Events/LocalDispatcher.h"



struct GameData;
class Collider;
class PropertyBag;
class Property;


class Actor
{
public:
	Actor(GameData* gameData);
	virtual ~Actor();

	void init(XMLParser& parser);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	const GameData* getGameData() const { return mGameData; }

	virtual void setVisibility(bool visibility) { mVisibility = visibility; }
	void reset();

	// Event handling
	EventPacket popEvent() { return mEvents.pop(); }
	void pushEvent(EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.hasEvent(); }

	// PropertyBag
	void setPropertyBag(PropertyBag* bag) { mPropertyBag = bag; }
	PropertyBag* propertyBag() const { return mPropertyBag; }
	Property* getProperty(const std::string& property) const;
	float getPropertyValue(const std::string& property) const;
	bool hasProperty(const std::string& property) const;

	// Collider
	void setCollider(Collider* collider) { mCollider = collider; }
	Collider* collider() const { return mCollider; }

	// Systems
	Animator&	animator() { return mAnimator; }
	Physics*	physics()	 { return &mPhysics; }
	const Physics* physics() const { return &mPhysics; }

	// Position
	VectorF		position() const { return mPhysics.rect().Center(); }
	RectF&		rectRef() { return mPhysics.rectRef(); }
	RectF		rect() const { return mPhysics.rect(); } 
	virtual RectF		renderRect() const { return rect(); } // TODO: remove

	// Effects
	void addEffect(Effect* effect);


protected:
	GameData* mGameData;

	PropertyBag* mPropertyBag;

	Collider* mCollider;

	Animator mAnimator;

	Animator2 mAnimator2;

	Physics mPhysics;

	EffectHandler mEffects;

	LocalDispatcher mEvents;

	bool mVisibility;
};