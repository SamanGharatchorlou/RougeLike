#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#include "Player/PlayerManager.h"
#include "Enemies/EnemyManager.h"


struct GameData;
class EffectPool;
class Environment;
class InputManager;
class RenderManager;
class TextureManager;

class ActorManager : public Observer, public Dispatcher
{
public:
	ActorManager(GameData* gameData);
	~ActorManager() { }

	void init(Environment* environment);
	void load(const XMLParser& parser, const Map* map);

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();
	void exit();

	PlayerManager* player() { return &mPlayer; }
	EnemyManager* enemies() { return &mEnemies; }

	void spawnEnemies(const XMLParser& parser, const Map* map);

	std::vector<Actor*> getAllEnemies();

	void handleEvent(EventData& data) override;
	void sendEvent(EventPacket event);


private:
	TextureManager* mTextures;
	RenderManager* mRendering;

	PlayerManager mPlayer;

	EnemyManager mEnemies;
};