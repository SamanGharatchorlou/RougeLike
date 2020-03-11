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
	VectorF cameraPosition = VectorF(0.0f, mGameData->map->size().y / 2.0f);
	Camera::Get()->setPosition(cameraPosition);
	Camera::Get()->setMapBoundaries(mGameData->map->size());

	// UI
	mGameData->uiManager->selectScreen(Screen::Game);

	// Player
	VectorF playerPosition = VectorF(mGameData->map->getTileSize().x, mGameData->map->size().y / 2.0f);
	mGameData->playerManager->getRectRef()->SetLeftCenter(playerPosition);

	// Camera
	Camera::Get()->follow(mGameData->playerManager->getRectRef());

	// Enemies
	EnemyManager* enemies = mGameData->enemies;
	enemies->addEnemiesToPool(EnemyType::Imp, 10);
	enemies->setTarget(mGameData->playerManager->getRectRef());

	// Collectables 
	// TODO?: change this to items, item manager (its not really a manager)
	collectables.subscrbeCollider(&mGameData->playerManager->get()->getCollider());

	// rendering
	mGameData->renderManager->Set(mGameData->map);
	mGameData->renderManager->Set(mGameData->playerManager);
	mGameData->renderManager->Set(enemies);
	mGameData->renderManager->Set(mGameData->uiManager);
	mGameData->renderManager->Set(&collectables);


	// Test spawning
	enemies->spawn(EnemyType::Imp, 5);

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

	if (mGameData->inputManager->isPressed(Button::E))
	{
		mGameData->map->generateRandomTunnel(30, 150);
	}

	mGameData->playerManager->handleInput();
	mGameData->uiManager->handleInput();
}


void GameState::fastUpdate(float dt)
{
	mGameData->playerManager->fastUpdate(dt);
	mGameData->enemies->fastUpdate(dt);

	Camera::Get()->fastUpdate(dt);
}


void GameState::slowUpdate(float dt)
{
	mGameData->playerManager->slowUpdate(dt);
	mGameData->enemies->slowUpdate(dt);

	mGameData->scoreManager->slowUpdate();

	collectables.slowUpdate(dt);

	Camera::Get()->slowUpdate(dt);

	// Update weapon collider list
	mGameData->enemies->clearSubscriptions();
	mGameData->enemies->subscribe(mGameData->playerManager->getWeaponColliders());
}


void GameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mGameData->renderManager->render();

	// update window surface
	SDL_RenderPresent(renderer);
}


void GameState::enter()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));
}


void GameState::resume()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));
}