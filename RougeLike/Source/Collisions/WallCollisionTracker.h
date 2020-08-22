#pragma once

class Actor;
class Map;
class MapTile;

class WallCollisionTracker
{
public:
	WallCollisionTracker() { };

	void setActor(Actor* actor) { mActor = actor; }

	VectorF allowedMovement(const Map* map, VectorF movement);


private:
	void restrictLeftMovement(const Map* map, VectorF& movement) const;
	void restrictRightMovement(const Map* map, VectorF& movement) const;
	void restrictTopMovement(const Map* map, VectorF& movement) const;
	void restrictDownMovement(const Map* map, VectorF& movement) const;

	RectF wallScaledRect(VectorF translation) const;

	bool cannotMove(const MapTile* tile) const;


private:
	//const RectF* mRect;
	Actor* mActor;
};


//class WallCollisionTracker2
//{
//public:
//	WallCollisionTracker2() { };
//
//	void init(Actor* actor);
//	void setRect(RectF rect) { mRect = rect; }
//	void resolveWallCollisions(const Map* map, VectorF& movement);
//
//
//private:
//	bool restrictLeftMovement(const Map* map, VectorF movement) const;
//	bool restrictRightMovement(const Map* map, VectorF movement) const;
//	bool restrictTopMovement(const Map* map, VectorF movement) const;
//	bool restrictDownMovement(const Map* map, VectorF movement) const;
//
//
//	RectF wallScaledRect(VectorF translation) const;
//
//
//	bool cannotMove(const MapTile* tile) const;
//
//
//private:
//
//	Actor* mActor;
//	RectF mRect;
//};