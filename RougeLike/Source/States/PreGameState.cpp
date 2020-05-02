#include "pch.h"
#include "PreGameState.h"

#include "GameState.h"

#include "Managers/GameController.h"

#include "Game/Cursor.h"
#include "UI/UIManager.h"
#include "Collisions/CollisionManager.h"
#include "Graphics/RenderManager.h"

#include "Graphics/TextureManager.h"
#include "UI/Screens/CharacterselectionScreen.h"

#include "Objects/Player/PlayerManager.h"


PreGameState::PreGameState(GameData* gameData, GameController* gameController) :
	mGameData(gameData), mGameController(gameController)
{

}


void PreGameState::init()
{	
	// Setup collision tracking (before setting up any Objects)
	initCollisions();

	mGameData->uiManager->selectScreen(Screen::CharacterSelection);
	selectionScreen = static_cast<CharacterSelectionScreen*>(mGameData->uiManager->getActiveScreen());
	
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("UICursor"));

	// Player
	mGameData->playerManager->init();
}


void PreGameState::slowUpdate(float dt)
{
	if (selectionScreen->enterGame())
	{
		selectCharacter();
		enterGame();
	}
}





void PreGameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);


	mGameData->uiManager->render();

	mGameData->cursor->render();

	// update window surface
	SDL_RenderPresent(renderer);
}


// --- Private Functions --- //
void PreGameState::selectCharacter()
{
	mGameData->playerManager->selectCharacter(selectionScreen->selectCharacter());
}

void PreGameState::enterGame()
{
	mGameController->getStateMachine()->replaceState(new GameState(mGameData, mGameController));
	mGameData->uiManager->selectScreen(Screen::Game);
}

void PreGameState::initCollisions()
{
	CollisionManager* collisions = mGameData->collisionManager;
	collisions->addNewCollisionTracker(CollisionManager::PlayerWeapon_Hit_Enemy);
	collisions->addNewCollisionTracker(CollisionManager::Enemy_Hit_Player);
	collisions->addNewCollisionTracker(CollisionManager::Player_Hit_Collectable);
}