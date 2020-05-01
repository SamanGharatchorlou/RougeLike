#pragma once

#include "State.h"

// TEMP
#include "Items/Collectables/Collectables.h"

class GameController;
struct GameData;

class GameState : public State
{
public:
	GameState(GameData* gameData, GameController* gameController);
	~GameState() { }

	// mandatory functions must be defined
	void init() override;
	void preProcess() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void handleInput() override;
	void render() override;
	void exit() override { };

	void enter();
	void resume();

	// TEMP
	Collectables collectables;

private:
	void initCamera();
	void initUI();
	void initMap();
	void initPlayer();
	void initEnemies();
	void initCollectables();
	void initRendering();

private:
	GameController* mGameController;
	GameData* mGameData;
};
