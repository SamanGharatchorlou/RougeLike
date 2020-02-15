#pragma once

#include "State.h"

class GameController;
struct GameData;
class PauseMenu;

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
	void exit() override;

private:
	void quitGame();
	void resumeGame();

private:
	GameController* mGameController;
	GameData* mGameData;

	PauseMenu* pauseMenu;
};