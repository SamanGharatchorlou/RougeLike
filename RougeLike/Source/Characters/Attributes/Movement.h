#pragma once

class Collider;

class Movement
{
public:
	Movement() : mCollider(nullptr), mCanMove(true) {}
	~Movement() {}

	void init(Collider* collider, float speed);
	void fastUpdate(float dt);

	void setDirection(VectorF direction) { mDirection = direction.normalise(); }
	void setPosition(VectorF position) { mPosition = position; }
	void setPreviousPosition() { mPosition = mPreviousPosition; }

	void setSpeed(float speed) { mSpeed = VectorF(speed,speed); }

	void flipDirection() { mDirection.x *= -1; mDirection.y *= -1; }

	// Movement
	inline void up()	{ mDirection.y = -1; }
	inline void down()	{ mDirection.y = +1; }
	inline void left()	{ mDirection.x = -1; }
	inline void right()	{ mDirection.x = +1; }

	inline void stopUp()	{ mDirection.y -= -1; }
	inline void stopDown()	{ mDirection.y -= +1; }
	inline void stopLeft()	{ mDirection.x -= -1; }
	inline void stopRight() { mDirection.x -= +1; }

	inline void resetDirection() { mDirection.zero(); }
	bool isMoving() const { return !mDirection.isZero() && mCanMove; }

	inline VectorF getMovementDistance()	const { return mSpeed * mDirection; }
	inline VectorF getDirection()			const { return mDirection; }
	inline VectorF getPostion()				const { return mPosition; }
	inline VectorF getPreviousPosition()	const { return mPreviousPosition; }

private:
	VectorF mPosition;
	VectorF mPreviousPosition;

	VectorF mSpeed;
	VectorF mDirection;

	bool mCanMove;

	Collider* mCollider;
};

