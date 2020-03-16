#pragma once

struct GameData;
class MapLevel;
class PlayerManager;
class EnemyManager;
class UIManager;
class Collectables;

// TODO: implement
//#include "Renderable.h"

class RenderManager
{
public:
	RenderManager(GameData* gameData);

	void Set(MapLevel* level) { mLevel = level; }
	void Set(PlayerManager* player) { mPlayer = player; }
	void Set(EnemyManager* enemies) { mEnemies = enemies; }
	void Set(UIManager* UI) { mUIManager = UI; }
	void Set(Collectables* collectables) { mCollectables = collectables; }

	//void add(Renderable* renderable);

	void render();

private:
	GameData* mGameData;

	MapLevel* mLevel;
	PlayerManager* mPlayer;;
	EnemyManager* mEnemies;
	UIManager* mUIManager;
	Collectables* mCollectables;

	//std::vector<Renderable*> renderables;
};
