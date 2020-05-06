#pragma once

#include "Screen.h"

struct GameData;

class PauseScreen : public Screen
{
public:
	PauseScreen(GameData* gameData);

	void update() override;
	
	void enter() override { }
	void exit() override;

	bool quitGame() const { return mQuitGame; }
	bool resumeGame() const { return mResumeGame; }
	bool restartGame() const { return mRestartGame; }
	bool openSettings() const { return mOpenSettings; }

	Type type() override { return Type::Pause; }


private:
	void updateBoxTexture(std::string buttonId, std::string boxId);


private:
	bool mResumeGame;
	bool mQuitGame;
	bool mRestartGame;
	bool mOpenSettings;
};