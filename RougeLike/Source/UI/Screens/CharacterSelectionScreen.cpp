#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Input/InputManager.h"
#include  "Graphics/TextureManager.h"

#include "Game/GameController.h"
#include "UI/ScreenController.h"

#include "UI/Elements/UIBox.h"
#include "UI/Elements/UIButton.h"


void CharacterSelectionScreen::init()
{
	const TextureManager* textures = TextureManager::Get();
	UIBox* character = static_cast<UIBox*>(find("Character"));
	mSelectedCharacter = textures->getTextureName(character->texture());

	Texture* texture = textures->getTexture(mSelectedCharacter + "Icon", FileManager::Image_UI);
	character->setTexture(texture);

	mSelectedWeapon = "Sword";

	linkButton(ScreenItem::Play, "PlayButton");
}


void CharacterSelectionScreen::handleInput(const InputManager* input)
{
	if (input->isPressed(Button::Esc) || input->isPressed(Button::Quit))
	{
		mController->quitGame();
	}
	else if (input->isPressed(Button::Enter))
	{
		mController->replaceScreen(ScreenType::Game);
		mController->replaceSystemState(SystemStates::GameState);
	}
}

void CharacterSelectionScreen::slowUpdate()
{
	if (selected(ScreenItem::Play))
	{
		mController->replaceScreen(ScreenType::Game);
		mController->replaceSystemState(SystemStates::GameState);
	}
}