#include "pch.h"
#include "GameState.h"
#include "PauseState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"

#include "Map/Map.h"
#include "Game/Cursor.h"
#include "Game/Camera.h"

#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/EnemyManager.h"

// TEMp
#include "Items/Spawner.h"
#include "Characters/Player/Player.h"


GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
	, collectables(gameData)
{
	// random seed
	srand((unsigned int)time(NULL));
}


void GameState::init()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));

	// Map
	mGameData->map->setScale(3.0f);
	mGameData->map->generateRandomTunnel(20, 100);

	// Camera
	mGameData->camera->setupCamera(mGameData->map);

	// UI
	mGameData->uiManager->selectScreen(Screen::Game);

	// Player
	PlayerManager* player = mGameData->playerManager;
	EnemyManager* enemies = mGameData->enemies;

	player->init();

	// TODO: splitting this out has created the dependancy that a character must be selected before anything else as the rect isnt set
	// this should happen on the screen before this one?
	player->selectCharacter("Soldier.xml");

	mGameData->camera->follow(player->getRectRef());

	// Enemies
	enemies->addEnemiesToPool(EnemyType::Imp, 10);
	enemies->subscribe(player->getWeaponColliders());
	enemies->setTarget(player->getRectRef());

	// Collectables 
	// TODO?: change this to items, item manager (its not really a manager)
	collectables.subscrbeCollider(&player->get()->getCollider());

	// rendering
	mGameData->renderManager->Set(mGameData->map);
	mGameData->renderManager->Set(player);
	mGameData->renderManager->Set(enemies);
	mGameData->renderManager->Set(mGameData->uiManager);
	mGameData->renderManager->Set(&collectables);


	// Test spawning
	enemies->spawn(EnemyType::Imp, 5);
	//enemies->spawn(EnemyType::Imp, 10.0f);
	//enemies->spawn(EnemyType::Imp, 15.0f);
	//enemies->spawn(EnemyType::Imp, 20.0f);
	//enemies->spawn(EnemyType::Imp, 22.0f);
	//enemies->spawn(EnemyType::Imp, 25.0f);
	//enemies->spawn(EnemyType::Imp, 28.0f);
	//enemies->spawn(EnemyType::Imp, 35.0f);
	//enemies->spawn(EnemyType::Imp, 40.0f);
	//enemies->spawn(EnemyType::Imp, 42.0f);

	Spawner itemSpawner;
	VectorF position = itemSpawner.findSpawnPoint(mGameData->map, 10);

	std::string weaponName = "weapon_big_hammer";

	WeaponCollectable* weaponPickup = new WeaponCollectable(weaponName, mGameData->textureManager->getTexture(weaponName));

	collectables.spawn(weaponPickup, position);
}



void GameState::preProcess()
{
	mGameData->playerManager->preProcess();
}


void GameState::handleInput()
{
	if (mGameData->inputManager->isPressed(Button::Esc))
	{
		mGameController->quitGame();
	}
	
	if (mGameData->inputManager->isPressed(Button::Pause))
	{
		mGameController->getStateMachine()->addState(new PauseState(mGameData, mGameController));
	}

	mGameData->playerManager->handleInput();
	mGameData->uiManager->handleInput();
}
 

void GameState::slowUpdate(float dt)
{
	mGameData->playerManager->slowUpdate(dt);
	mGameData->enemies->slowUpdate(dt);

	mGameData->scoreManager->slowUpdate();

	collectables.slowUpdate();
}


void GameState::fastUpdate(float dt)
{
	mGameData->playerManager->fastUpdate(dt);
	mGameData->enemies->fastUpdate(dt);

	mGameData->camera->fastUpdate();
}


void GameState::render()
{
	// clear screen
	SDL_SetRenderDrawColor(mGameData->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mGameData->renderer);

	mGameData->renderManager->render();

	// update window surface
	SDL_RenderPresent(mGameData->renderer);
}


void GameState::resume()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));
}