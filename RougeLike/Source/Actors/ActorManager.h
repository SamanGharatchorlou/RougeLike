#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#include "Collisions/PlayerCollisions.h"
#include "Weapons/WeaponStash.h"

struct GameData;
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
	void slowUpdate(float dt);
	void render();
	void exit();

	Player* player() { return mPlayer; }
	EnemyManager* enemies() { return mEnemies; }

	std::vector<Actor*> getAllEnemies();
	std::vector<Actor*> getAllActors();

	void handleEvent(EventData& data) override;
	void sendEvent(EventPacket event);

	// temp
	void selectWeapon(const std::string& weaponName);

private:
	void initPlayer();
	void initEnemies();


private:
	GameData* mGameData;
	EffectPool* mEffectPool;

	// Player stuff
	PlayerCollisions mPlayerCollisions;
	WeaponStash weaponStash;

	Player* mPlayer;
	EnemyManager* mEnemies;
};