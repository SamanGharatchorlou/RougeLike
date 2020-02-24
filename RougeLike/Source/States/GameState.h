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
	void preProcess() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void handleInput() override;
	void render() override;
	void exit() override { };

	void resume();


private:
	GameController* mGameController;
	GameData* mGameData;
};
