#pragma once

#include "Collectable.h"
#include "Collisions/CollisionTracker.h"

struct GameData;

class Collectables
{
public:
	Collectables(GameData* gameData);

	void spawn(Collectable* collectable, VectorF position);
	void slowUpdate();
	void render();

	void subscrbeCollider(Collider* collider);

private:
	GameData* mGameData;

	CollisionTracker mCollisionTracker;

	std::vector<Collectable*> mCollectables;
};