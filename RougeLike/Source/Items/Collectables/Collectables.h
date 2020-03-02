#pragma once

#include "Collectable.h"

struct GameData;

class Collectables
{
public:
	Collectables(GameData* gameData) : mGameData(gameData) { }

	void spawn(Collectable* collectable, VectorF position);
	void slowUpdate();
	void render();

private:
	GameData* mGameData;

	std::vector<Collectable*> mCollectables;
};