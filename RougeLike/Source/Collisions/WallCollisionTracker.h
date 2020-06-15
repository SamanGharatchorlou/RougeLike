#pragma once

class Actor;
class Map;

class WallCollisionTracker
{
public:
	WallCollisionTracker(Actor* actor) : mActor(actor) { };
	void resolveWallCollisions(const Map* map, float dt);


private:
	void testLeftCollisions(const Map* map,  float dt);
	void testTopCollisions(const Map* map, float dt);
	void testRightCollisions(const Map* map, float dt);
	void testBottomCollisions(const Map* map,   float dt);

	RectF wallScaledRect(VectorF translation) const;


private:
	Actor* mActor;
};