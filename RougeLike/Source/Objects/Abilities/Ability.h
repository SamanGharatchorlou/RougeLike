#pragma once

#include "Events/Events.h"
#include "Objects/Effects/Effect.h"

#include "Animations/Animator.h"
#include "Collisions/Collider.h"

class Actor;
class Map;
class Player;

class Ability
{
public:
	enum TargetType
	{
		Self,
		Single_Enemy,

		Area,
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
	virtual void init(Animator animator, Player* player);
	
	virtual void activate(Actor* target) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render();

	virtual const TargetType targetType() const = 0;

	void setName(const std::string& name) { mName = name; }
	std::string name() const { return mName; }

	void setState(State state) { mState = state; }
	State state() const { return mState; }

	bool hasEvent() const { return mEvents.size() > 0; }
	EventPacket popEvent();

	void realiseSize();


protected:
	std::string mName;

	State mState;
	Animator mAnimator;
	std::queue<EventPacket> mEvents;

	Player* mPlayer;
	float mMaxDimention;
	RectF mRect;
};



class AreaAbility : public Ability
{
public:
	
	void setRangeCircle(Texture* rangeCircle) { mRangeCircle = rangeCircle; }

	virtual void activate(VectorF position) = 0;
	virtual void render() override;

	bool isValidTarget(VectorF target, Map* map);

	RectF effectArea() const { return mRect; }
	Collider collider();


protected:
	Map* mMap;

	float mRange;
	Texture* mRangeCircle;
};


Ability* createNewAbility(const std::string& ability);