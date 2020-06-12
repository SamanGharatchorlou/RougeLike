#include "pch.h"
#include "GameState.h"

#include "PauseState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"
#include "Collisions/CollisionManager.h"

#include "Map/Environment.h"
#include "Game/Cursor.h"
#include "Game/Camera.h"

#include "Objects/Actors/ActorManager.h"
#include "Objects/Actors/Player/Player.h"
#include "Objects/Actors/Enemies/EnemyManager.h"

// temp
#include "Items/Collectables/Collectable.h"

GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
	, mCollectables(gameData)
{
	// random seed
	srand((unsigned int)time(NULL));
}


void GameState::init()
{
	initMap();
	initCamera();
	initUI();
	mGameData->actors->init();
	initRendering();

	// Start Audio
	mGameData->audioManager->playMusic("Ludumdum");
	mGameData->audioManager->setMusicVolume(0.08f);
	mGameData->audioManager->setSoundVolume(0.4f);
	mGameData->audioManager->toggleMute();

	//mCollectables.spawnRandomItem(Collectables::MeleeWeapon);

	Collectable* Armor = new AbilityCollectable("Armor");
	mCollectables.spawn(Armor, 15);

	Collectable* Blink = new AbilityCollectable("Blink");
	mCollectables.spawn(Blink, 20);

	//Collectable* Spikes = new AbilityCollectable("Spikes");
	//mCollectables.spawn(Spikes, 10);

	//Collectable* Heal = new AbilityCollectable("Heal");
	//mCollectables.spawn(Heal, 12);

	Collectable* weapon = new WeaponCollectable("Mace");
	mCollectables.spawn(weapon, 12);

	Collectable* smash = new AbilityCollectable("Smash");
	mCollectables.spawn(smash, 10);
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

	mGameData->actors->handleInput();
}


void GameState::fastUpdate(float dt)
{
	mGameData->actors->fastUpdate(dt);

	Camera::Get()->fastUpdate(dt);

	mGameData->collisionManager->fastUpdate();
}


void GameState::slowUpdate(float dt)
{
	mGameData->actors->slowUpdate(dt);

	mGameData->scoreManager->slowUpdate();

	mCollectables.slowUpdate(dt);

	Camera::Get()->slowUpdate(dt);

	// End current level, close old level exit, open new level entrance
	if (mGameData->environment->canClosePreviousLevel(mGameData->actors->player()->position()))
	{
		nextLevel();
	}

	// End of slow frame
	mGameData->collisionManager->resetColliders();
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


void GameState::resume()
{
	mGameData->uiManager->setCursorTexture(mGameData->textureManager->getTexture("GameCursor", FileManager::Image_UI));
}


void GameState::exit()
{
	mGameData->actors->enemies()->clear();
	mGameData->actors->player()->reset();
	mGameData->environment->restart();
	mGameData->scoreManager->reset();
	mGameData->collisionManager->clearColliders();
}


// --- Private Functions --- //

void GameState::initUI()
{
	mGameData->uiManager->setCursorTexture(mGameData->textureManager->getTexture("GameCursor", FileManager::Image_UI));
	mGameData->uiManager->selectScreen(Screen::Game);
}

void GameState::initCamera()
{
	VectorF cameraPosition = VectorF(0.0f, mGameData->environment->size().y / 2.0f);
	Camera::Get()->setPosition(cameraPosition);

	// TODO: fix these values
	Camera::Get()->initShakeyCam(150.0f, 100.0f, 50.0f);
}

void GameState::initMap()
{
	mGameData->environment->init();
}

void GameState::initRendering()
{
	mGameData->renderManager->Set(mGameData->environment);
	mGameData->renderManager->Set(mGameData->actors);
	mGameData->renderManager->Set(mGameData->uiManager);
	mGameData->renderManager->Set(&mCollectables);
}


void GameState::nextLevel()
{
	mGameData->environment->nextLevel();

	// end level
	mGameData->actors->enemies()->clearAllEnemies();
	mGameData->actors->enemies()->spawnLevel();

	//mCollectables.spawnRandomItem(Collectables::MeleeWeapon);
}