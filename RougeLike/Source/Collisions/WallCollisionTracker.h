#pragma once

class Actor;
class Map;
class MapTile;
class Collider;

class WallCollisionTracker
{
public:
	WallCollisionTracker() : mCollider(nullptr) { };

	void setCollider(const Collider* collider) { mCollider = collider; }
	//void setRect(const RectF* rect) { mRect = rect; }

	VectorF allowedVelocity(const Map* map, VectorF velocity, float dt);

	bool doesCollide(RectF rect, const Map* map) const;


private:
	bool braodPhaseCollision(const Map* map, VectorF& movement, float dt) const;

	void restrictLeftMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictRightMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictUpMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictDownMovement(const Map* map, VectorF& movement, float dt) const;

	RectF wallScaledRect() const;

	bool cannotMove(const MapTile* tile) const;

private:
	// NOTE: these have been hacked in, we only need one
	const Collider* mCollider;
	//const RectF* mRect;

	RectF mWallScaledRect;
};

