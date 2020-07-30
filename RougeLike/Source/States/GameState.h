#pragma once

#include "State.h"

class GameController;
struct GameData;

class GameState : public State
{
public:
	GameState(GameData* gameData, GameController* gameController);
	~GameState() { }

	// mandatory functions must be defined
	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void handleInput() override;
	void render() override;

	void resume() override;
	void exit() override;


private:
	void initCamera();
	void initUI();
	void initMap();
	void initRendering();

	void beginGame();
	void nextLevel();

private:
	GameController* mGameController;
	GameData* mGameData;
};
