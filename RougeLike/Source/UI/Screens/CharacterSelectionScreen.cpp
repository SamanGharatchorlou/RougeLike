#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Game/Data/GameData.h"
#include "UI/UIManager.h"
#include  "Graphics/TextureManager.h"

#include "UI/Elements/UIButton.h"


void CharacterSelectionScreen::enter()
{
	const TextureManager* textures = TextureManager::Get();
	UIBox* character = static_cast<UIBox*>(find("Character"));
	mSelectedCharacter = textures->getTextureName(character->texture());

	Texture* texture = textures->getTexture(mSelectedCharacter + "Icon", FileManager::Image_UI);
	character->setTexture(texture);

	mSelectedWeapon = "Sword";

	linkButton(ScreenItem::Play, "PlayButton");
}
