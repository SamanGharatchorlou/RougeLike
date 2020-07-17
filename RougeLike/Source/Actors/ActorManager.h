#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#include "Player/PlayerManager.h"
#include "Enemies/EnemyManager.h"

struct GameData;
class EffectPool;
class Environment;

class ActorManager : public Observer, public Dispatcher
{
public:
	ActorManager(GameData* gameData);
	~ActorManager() { }

	void init(Environment* environment);
	void load();

	void handleInput();
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();
	void exit();

	PlayerManager* player() { return &mPlayer; }
	EnemyManager* enemies() { return &mEnemies; }

	std::vector<Actor*> getAllEnemies();

	void handleEvent(EventData& data) override;
	void sendEvent(EventPacket event);


private:
	void initPlayer();
	void initEnemies();


private:
	GameData* mGameData;

	PlayerManager mPlayer;
	EnemyManager mEnemies;
};