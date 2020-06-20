#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

class GameData;
class EffectPool;
class Player;
class EnemyManager;
class Actor;

class ActorManager : public Observer, public Dispatcher
{
public:
	ActorManager(GameData* gameData);
	~ActorManager();

	void init();
	void handleInput();
	void fastUpdate(float dt);
	void effectLoop();
	void slowUpdate(float dt);
	void render();

	Player* player() { return mPlayer; }
	EnemyManager* enemies() { return mEnemies; }

	std::vector<Actor*> getAllEnemies();
	std::vector<Actor*> getAllActors();

	void handleEvent(EventData& data) override;
	void sendEvent(EventPacket event);


private:
	void initEffectPool();
	void initPlayer();
	void initEnemies();


private:
	GameData* mGameData;
	EffectPool* mEffectPool;
	Player* mPlayer;
	EnemyManager* mEnemies;
};