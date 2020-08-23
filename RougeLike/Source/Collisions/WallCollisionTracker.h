#pragma once

class Actor;
class Map;
class MapTile;


class WallCollisionTracker
{
public:
	WallCollisionTracker() { };
	~WallCollisionTracker() { mActor = nullptr; }

	void setActor(Actor* actor) { mActor = actor; }

	VectorF allowedMovement(const Map* map, VectorF movement);
	VectorF allowedVelocity(const Map* map, VectorF velocity, float dt);


private:
	void restrictLeftMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictRightMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictTopMovement(const Map* map, VectorF& movement, float dt) const;
	void restrictDownMovement(const Map* map, VectorF& movement, float dt) const;

	RectF wallScaledRect() const;

	bool cannotMove(const MapTile* tile) const;


private:
	//const RectF* mRect;
	Actor* mActor;
};

