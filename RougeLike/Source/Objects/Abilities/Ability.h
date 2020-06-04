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

	virtual void fillValues(ValueMap& values) = 0;
	virtual void init(Animator animator);
	
	virtual void activate(Actor* target) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render();

	virtual const TargetType targetType() const = 0;

	void setState(State state) { mState = state; }
	State state() const { return mState; }

	bool hasEvent() const { return mEvents.size() > 0; }
	EventPacket popEvent();

	void realiseSize();


protected:
	State mState;
	Animator mAnimator;
	std::queue<EventPacket> mEvents;

	float mMaxDimention;
	RectF mRect;
};



class AreaAbility : public Ability
{
public:
	virtual void activate(VectorF position) = 0;

	bool isValidTarget(VectorF target, Map* map);

	RectF effectArea() const { return mRect; }
	Collider collider();

protected:
	Map* mMap;
};


Ability* createNewAbility(const std::string& ability);