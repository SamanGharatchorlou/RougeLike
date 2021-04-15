#pragma once

class Actor;
class Map;
class MapTile;


class WallCollisionTracker
{
public:
	WallCollisionTracker() : mRect(nullptr), mActor(nullptr) { };

	void setActor(const Actor* actor) { mActor = actor; mRect = nullptr; }
	void setRect(const RectF* rect) { mRect = rect; mActor = nullptr; }

	VectorF allowedVelocity(const Map* map, VectorF velocity, float dt);


private:
	bool braodPhaseCollision(const Map* map, VectorF& movement, float dt) const;

	void restrictLeftMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictRightMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictUpMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictDownMovement(const Map* map, VectorF& movement, float dt) const;

	RectF wallScaledRect() const;

	bool cannotMove(const MapTile* tile) const;

private:
	const RectF* mRect;
	const Actor* mActor;

	RectF mWallScaledRect;
};

