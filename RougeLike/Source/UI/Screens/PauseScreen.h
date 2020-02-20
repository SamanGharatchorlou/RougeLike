#pragma once

#include "Screen.h"

struct GameData;

class PauseScreen : public Screen
{
public:
	PauseScreen(GameData* gameData, std::vector<UILayer*> layers);

	void enter() override { }
	void exit() override;
	void update() override;

	bool quitGame() const { return mQuitGame; }
	bool resumeGame() const { return mResumeGame; }
	bool restartGame() const { return mRestartGame; }

	Type type() override { return Type::Pause; }

private:
	bool mResumeGame;
	bool mQuitGame;
	bool mRestartGame;
};