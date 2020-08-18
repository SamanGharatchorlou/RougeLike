#pragma once

#include "State.h"
#include "UI/Screens/Screen.h"

class GameController;
struct GameData;
class PauseScreen;
class SettingsScreen;


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
	void pauseScreenUpdate();
	void settingsScreenUpdate();

	void quitGame();
	void resumeGame();
	void restartGame();

	void selectScreen(Screen::Type screen);

private:
	GameController* mGameController;
	GameData* mGameData;

	PauseScreen* mPauseScreen;
	SettingsScreen* mSettingsScreen;
};