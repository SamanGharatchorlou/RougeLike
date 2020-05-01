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

private:
	GameData* mGameData;

	Timer<float> timer;

	std::vector<Collectable*> mCollectables;
};