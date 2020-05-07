#pragma once

/*
class PhysicsSlowdownEventHandler:

	def __init__(self, obj):
		blablabla
	def __call__(event):
		self.obj.physics.speed / event.magnitude


class FooEventHandler:

	def __init__(self, obj):



class Enemy:
	physics = PhysicsObject
	foo = FooObject
	event_registry = {
		'slowdown': PhysicsSlowdownEventHandler,
		'become green': lambda x: x.colour = green
	}

	def subscribe():
		for event_type in event_registry:
			event_messaging.observe(event_type)

	def process_event(event):
		self.event_registry[event.type](self).handle(event)

		PhysicsSlowdownEventHandler(self).handle(event)
*/


class Collider
{
public:
	Collider();
	Collider(RectF* rect, VectorF relativeColliderSize = VectorF(1.0f, 1.0f));
	virtual ~Collider() { }

	void init(RectF* objectRect, VectorF colliderScale = VectorF(1.0f, 1.0f));
	void reset();

	bool doesIntersect(Collider* collider) const;

	void hasCollidedWith(Collider* collider);

	inline void setDidHit(bool collisionStatus) { mDidHit = collisionStatus; }
	inline bool didHit() const { return mDidHit; }

	inline void setGotHit(bool collisionStatus) { mGotHit = collisionStatus; }
	inline bool gotHit() const { return mGotHit; }

	// TEMP
	bool contains(VectorF position);

	const RectF rect() const { return *mRect; }
	RectF scaledRect() const;

	const Collider* getOtherCollider() const { return mOtherCollider; }

#if _DEBUG
	void setName(const std::string& name) { mName = name; }
	std::string name() const { return mName; }
#endif

protected:
	const RectF* mRect;
	VectorF mColliderScale;

	Collider* mOtherCollider;

	bool mDidHit;
	bool mGotHit;

#if _DEBUG
	std::string mName;
#endif
};