#pragma once

#include "Menu.h"

struct GameData;

class PauseMenu : public Menu
{
public:
	PauseMenu(GameData* gameData);

	void enter() override { }
	void exit() override;
	void update() override;

	bool shouldQuitGame() { return mQuitGame; }
	bool shouldResumeGame() { return mResumeGame; }

private:
	bool mResumeGame;
	bool mQuitGame;
};