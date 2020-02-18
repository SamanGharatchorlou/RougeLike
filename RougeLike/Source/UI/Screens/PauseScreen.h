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

	bool shouldQuitGame() { return mQuitGame; }
	bool shouldResumeGame() { return mResumeGame; }

	Type type() override { return Type::Pause; }

private:
	bool mResumeGame;
	bool mQuitGame;
};