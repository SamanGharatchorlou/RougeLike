#pragma once

class Actor;
class Map;
class MapTile;

class WallCollisionTracker
{
public:
	WallCollisionTracker(Actor* actor) : mActor(actor) { };
	void resolveWallCollisions(const Map* map, float dt);


private:
	bool restrictLeftMovement(const Map* map,  float dt) const;
	bool restrictRightMovement(const Map* map, float dt) const;
	bool restrictTopMovement(const Map* map, float dt) const;
	bool restrictDownMovement(const Map* map,   float dt) const;

	RectF wallScaledRect(VectorF translation) const;

	bool cannotMove(const MapTile* tile) const;


private:
	Actor* mActor;
};