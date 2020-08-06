#pragma once

#include "Screen.h"

struct GameData;

class PauseScreen : public Screen
{
public:
	PauseScreen(const TextureManager* textures);

	void enter() override;
	void handleInput(const InputManager* input) { }
	void update(float dt) override;
	void exit() override;

	bool quitGame() const { return mStates.at("QuitButton"); }
	bool resumeGame() const { return mStates.at("ResumeButton"); }
	bool restartGame() const { return mStates.at("RestartButton"); }
	bool openSettings() const { return mStates.at("SettingsButton"); }

	Type type() override { return Type::Pause; }


private:
	void resetButtonStates();
	void updateBoxTexture(BasicString buttonId, BasicString boxId);
	void setButtonsAndStates();

	void addButtonAndState(const BasicString& label);

private:
	std::unordered_map<BasicString, UIButton*> mButtons;
	std::unordered_map<BasicString, bool> mStates;
};