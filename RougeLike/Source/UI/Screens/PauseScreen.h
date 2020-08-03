#pragma once

#include "Screen.h"

struct GameData;

class PauseScreen : public Screen
{
public:
	PauseScreen(GameData* gameData);

	void update(float dt) override;
	
	void enter() override;
	void exit() override;

	bool quitGame() const { return mQuitGame; }
	bool resumeGame() const { return mResumeGame; }
	bool restartGame() const { return mRestartGame; }
	bool openSettings() const { return mOpenSettings; }

	Type type() override { return Type::Pause; }


private:
	void updateBoxTexture(BasicString buttonId, BasicString boxId);


private:
	bool mResumeGame;
	bool mQuitGame;
	bool mRestartGame;
	bool mOpenSettings;
};