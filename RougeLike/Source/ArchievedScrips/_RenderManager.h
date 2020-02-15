#pragma once
#include "Environment/MapData.h"

class TextureManager;
class GameData;
class Player;
class EnemyManager;
class UIManager;

struct PlayerRenderer
{
	Player* player;
	bool hasRendered;

	float getYPosition();
	void render();
};

/* TODO create some generic rendering list to render objects
struct Renderable
{
	virtual void render() = 0;

	// order to be rendered 10->first 0->last
	unsigned int priority = 10;
};
*/

// used to render everything here this will decide what's rendered 
// when and where incl. camera, player, map etc
class RenderManager
{
public:
	RenderManager(GameData* gameData) : mGameData(gameData) 
	{
#if _DEBUG // Make sure we only create 1 map data;
		mMap.initCounter();
#endif
	}

	//void add(Renderable* object) { renderObjects.push_back(object); }

	void initMap(std::string mapDataFilePath);
	void Set(Player* player) { mPlayer.player = player; }
	void Set(EnemyManager* enemies) { mEnemies = enemies; }
	void Set(UIManager* UI) { mUIManager = UI; }

	void render();

private:
	GameData* mGameData;
	MapData mMap;
	PlayerRenderer mPlayer;

	EnemyManager* mEnemies;
	UIManager* mUIManager;

	int mapScale = 3.0;

	//std::vector<Renderable*> renderObjects;
};
