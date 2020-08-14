#pragma once

#include "Screen.h"


class CharacterSelectionScreen : public Screen
{
public:
	CharacterSelectionScreen();

	void enter() override;

	void handleInput(const InputManager* input) { };
	void update(float dt) override;
	void exit() override { };

	const BasicString& selectedCharacter() const { return mSelectedCharacter; }
	const BasicString& selectedWeapon() const { return mSelectedWeapon; }

	bool enterGame() const { return mEnterGame; }

	Type type() override { return Type::CharacterSelection; }


private:
	void selectWeapon();


private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;

	bool mEnterGame;

	UIButton* mPlayButton;
	//UIBox* mButtonBackground;
};