#pragma once

struct GameData;
class Map;
class Player;
class EnemyManager;
class UIManager;


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
	RenderManager(GameData* gameData);

	void Set(Map* map) { mMap = map; }
	void Set(Player* player) { mPlayer = player; }
	void Set(EnemyManager* enemies) { mEnemies = enemies; }
	void Set(UIManager* UI) { mUIManager = UI; }

	void render();

private:
	GameData* mGameData;

	Map* mMap;
	Player* mPlayer;;
	EnemyManager* mEnemies;
	UIManager* mUIManager;
};
