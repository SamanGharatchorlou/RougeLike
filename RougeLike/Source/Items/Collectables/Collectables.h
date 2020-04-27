#pragma once

#include "Collectable.h"
#include "Collisions/CollisionTracker.h"

struct GameData;

class Collectables
{
public:
	Collectables(GameData* gameData);

	void slowUpdate(float dt);
	void render();

	void spawn(Collectable* collectable, VectorF position);
	
	void subscribeCollider(Collider* collider);


private:
	GameData* mGameData;

	Timer<float> timer;

	CollisionTracker mCollisionTracker;

	std::vector<Collectable*> mCollectables;
};