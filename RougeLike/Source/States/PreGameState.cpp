 #include "pch.h"
#include "PreGameState.h"

#include "Graphics/Renderer.h"
#include "Game/Data/GameData.h"

#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Graphics/TextureManager.h"
#include "UI/Screens/CharacterselectionScreen.h"

#include "Game/Environment.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/PlayerManager.h"


#include "Networking/NetworkManager.h"


PreGameState::PreGameState(GameData* gameData) : mGameData(gameData) { }


void PreGameState::init()
{	
	UIManager* UI = mGameData->uiManager;

	UI->controller()->clearScreenStack();
	UI->controller()->addScreen(ScreenType::CharacterSelection);

	UI->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));

	AudioManager* audio = AudioManager::Get();
	if(!audio->isPlaying("Menu", nullptr))
		audio->pushEvent(AudioEvent(AudioEvent::FadeIn, "Menu", nullptr, 1000));
}


void PreGameState::slowUpdate(float dt)
{
	Screen* screen = mGameData->uiManager->getActiveScreen();

	if (screen->type() == ScreenType::CharacterSelection)
	{
		CharacterSelectionScreen* selectionScreen = static_cast<CharacterSelectionScreen*>(screen);
		if(selectionScreen->characterSelected())
		{
			mSelectedCharacter = selectionScreen->selectedCharacter();
			mSelectedWeapon = selectionScreen->selectedWeapon();
		}

		if (!mGameData->network->mSetupComplete && mGameData->inputManager->isPressed(Button::Key::E))
		{
			testFunction();
		}
	}
}


void PreGameState::testFunction()
{
	DebugPrint(Log, "Enter connection type ('client' or 'server')\n");


	char connectionType[100];
	gets_s(connectionType, 100);

	if (strcmp(connectionType, "server") == 0)
	{
		Server* server = new Server;
		server->open();

		mGameData->network->mServer = server;
		mGameData->network->mSetupComplete = true;
		//// now need to run this....
		//while (true)
		//{
		//	BasicString message("", 1024);
		//	BasicString senderInfo("", 1024);

		//	server.receiveMessage(message, nullptr);

		//	if (!message.empty())
		//	{
		//		DebugPrint(Log, "%s\n", message.c_str());
		//	}
		//}
	}
	else
	{
		Client* client = new Client;
		client->open();

		mGameData->network->mClient = client;
		mGameData->network->mSetupComplete = true;

		//while (true)
		//{
		//	BasicString message("", 1024);
		//	std::cin.get(message.buffer(), message.bufferLength());

		//	message.calculateLength();
		//	client.sendMessage(message);

		//	std::cin.ignore();
		//}
	}
}


void PreGameState::exit()
{
	PlayerManager* player = mGameData->environment->actors()->player();

	if(!mSelectedCharacter.empty())
		player->selectCharacter(mSelectedCharacter);

	if(!mSelectedWeapon.empty())
		player->selectWeapon(mSelectedWeapon);
}


void PreGameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mGameData->uiManager->render();

	// update window surface
	SDL_RenderPresent(renderer);
}
