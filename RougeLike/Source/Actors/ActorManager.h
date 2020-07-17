#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#include "Player/PlayerManager.h"


struct GameData;
class EffectPool;
class EnemyManager;
class Environment;

class ActorManager : public Observer, public Dispatcher
{
public:
	ActorManager(GameData* gameData);
	~ActorManager();

	void init(Environment* environment);
	void load();

	void handleInput();
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();
	void exit();

	PlayerManager* player() { return &mPlayer; }
	EnemyManager* enemies() { return mEnemies; }

	std::vector<Actor*> getAllEnemies();

	void handleEvent(EventData& data) override;
	void sendEvent(EventPacket event);


private:
	void initPlayer();
	void initEnemies();


private:
	GameData* mGameData;

	PlayerManager mPlayer;
	EnemyManager* mEnemies;
};