#pragma once

class Actor;
class Map;

class WallCollisionTracker
{
public:
	WallCollisionTracker(Actor* actor) : mActor(actor), mProcessCollisions(true) { };

	void resolveWallCollisions(const Map* map, float dt);
	bool doesCollideLeft(const Map* map, const VectorF point, float dt) const;
	bool doesCollideRight(const Map* map, const VectorF point, float dt) const;
	bool doesCollideTop(const Map* map, const VectorF point, float dt) const;
	bool doesCollideBot(const Map* map, const VectorF point, float dt) const;

	void checkCollisions(bool state) { mProcessCollisions = state; }

private:
	Actor* mActor;
	bool mProcessCollisions;
};