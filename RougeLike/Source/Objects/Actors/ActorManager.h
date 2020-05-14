#pragma once

#include "Events/Observer.h"

class GameData;
class Player;
class EnemyManager;
class Actor;

class ActorManager : public Observer
{
public:
	ActorManager(GameData* gameData);
	~ActorManager();

	void init();
	void handleInput();
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	Player* player() { return mPlayer; }
	Actor* playerActor();

	EnemyManager* enemies() { return mEnemies; }

	std::vector<Actor*> getAllEnemies();
	std::vector<Actor*> getAllActors();

	void handleEvent(const Event event, EventData& data) override;


private:
	void initPlayer();
	void initEnemies();


private:
	GameData* mGameData;
	Player* mPlayer;
	EnemyManager* mEnemies;


};