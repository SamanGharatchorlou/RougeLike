#pragma once

class Actor;
class Map;
class MapTile;


class WallCollisionTracker
{
public:
	WallCollisionTracker() { };
	//~WallCollisionTracker() { mActor = nullptr; }

	void setActor(const Actor* actor) { mActor = actor; }
	void setRect(const RectF* rect) { mRect = rect; }

	VectorF allowedVelocity(const Map* map, VectorF velocity, float dt) const;


private:
	void restrictLeftMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictRightMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictTopMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictDownMovement(const Map* map, VectorF& movement, float dt) const;

	RectF wallScaledRect() const;

	bool cannotMove(const MapTile* tile) const;


private:
	const RectF* mRect;
	const Actor* mActor;
};

