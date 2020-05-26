#pragma once

#include "Events/Events.h"
#include "Objects/Effects/Effect.h"

#include "Animations/Animator.h"
#include "Collisions/Collider.h"

class Actor;
class Map;

class Ability
{
public:
	enum TargetType
	{
		Self,
		Single_Enemy,
		Area_Attack,
		Area_Point
	};

	enum State
	{
		None,
		Idle,
		Selected,
		Activating,
		Running,
		Finished
	};


public:
	Ability() : mState(None) { }
	virtual ~Ability() { }

	virtual void init(Animator animator) { mAnimator = animator; }
	
	virtual void activate(Actor* target) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0 ;

	virtual const TargetType targetType() const = 0;

	void setState(State state) { mState = state; }
	State state() const { return mState; }

	bool hasEvent() const { return mEvents.size() > 0; }
	EventPacket popEvent();


protected:
	State mState;
	Animator mAnimator;
	std::queue<EventPacket> mEvents;
};



class AreaAbility : public Ability
{
public:
	virtual void activate(VectorF position) = 0;

	bool isValidTarget(VectorF target);

	RectF effectArea() const { return mRect; }
	Collider collider();

protected:
	RectF mRect;
	Map* mMap;
};