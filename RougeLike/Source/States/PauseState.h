#pragma once

#include "State.h"

class GameController;
struct GameData;
class PauseScreen;

class PauseState : public State
{
public:
	PauseState(GameData* gameData, GameController* gameController);

	// mandatory functions must be defined
	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override;
	void render() override;
	void exit() override { }

private:
	void quitGame();
	void resumeGame();
	void restartGame();

private:
	GameController* mGameController;
	GameData* mGameData;

	PauseScreen* pauseScreen;
};