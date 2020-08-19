#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#include "Player/PlayerManager.h"
#include "Enemies/EnemyManager.h"


struct GameData;
class Environment;
class InputManager;

class ActorManager : public Observer, public Dispatcher
{
public:
	void init(GameData* gameData);
	void loadPools();

	void clear();

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	PlayerManager* player() { return &mPlayer; }
	EnemyManager* enemies() { return &mEnemies; }

	void spawnEnemies(const XMLNode spawnNode, const Map* map);

	std::vector<Actor*> getAllEnemies();

	void handleEvent(EventData& data) override;
	void sendEvent(EventPacket event);


private:
	PlayerManager mPlayer;
	EnemyManager mEnemies;
};