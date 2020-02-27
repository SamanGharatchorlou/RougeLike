#pragma once

#include "Collectable.h"

struct GameData;

class Collectables
{
public:
	Collectables(GameData* gameData) : mGameData(gameData) { }

	void update();

	void render();

private:
	GameData* mGameData;

	std::vector<Collectable> mCollectables;
};