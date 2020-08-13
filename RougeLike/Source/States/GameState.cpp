#include "pch.h"
#include "GameState.h"

#include "PauseState.h"

#include "Game/GameController.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"
#include "Collisions/CollisionManager.h"

#include "Objects/Pools/EffectPool.h"

#include "Game/Environment.h"
#include "Input/Cursor.h"
#include "Game/Camera/Camera.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
#include "Actors/Enemies/EnemyManager.h"

// temp
#include "Items/Collectables/Collectable.h"
#include "Map/Map.h"

GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
{
	// random seed
	srand((unsigned int)time(NULL));
}


void GameState::init()
{
	//mGameData->environment->init();

	//VectorF playerPosition = VectorF(Camera::Get()->rect().Center().x, mGameData->environment->size().y / 2.0f);

	Map* map = mGameData->environment->map(VectorF(0, 0));
	VectorF playerPosition = map->randomFloorTile(50)->rect().Center();

	mGameData->environment->actors()->player()->setPosition(playerPosition);

	initCamera();
	initUI();
	initRendering();


	// Start Audio
	mGameData->audioManager->playMusic("Ludumdum");
	mGameData->audioManager->setMusicVolume(0.08f);
	mGameData->audioManager->setSoundVolume(0.4f);
	mGameData->audioManager->toggleMute();

	//mCollectables.spawnRandomItem(Collectables::MeleeWeapon);

	//Collectable* Armor = new AbilityCollectable("Armor");
	//mCollectables.spawn(Armor, 15);

	//Collectable* Blink = new AbilityCollectable("Blink");
	//mCollectables.spawn(Blink, 20);

	//Collectable* Spikes = new AbilityCollectable("Spikes");
	//mCollectables.spawn(Spikes, 10);

	//Collectable* Heal = new AbilityCollectable("Heal");
	//mCollectables.spawn(Heal, 12);

	//Collectable* weapon = new WeaponCollectable("Mace");
	//mCollectables.spawn(weapon, 12);

	//Collectable* smash = new AbilityCollectable("Smash");
	//mCollectables.spawn(smash, 10);

	//Collectable* charge = new AbilityCollectable("Charge");
	//mCollectables.spawn(charge, 15);
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

	mGameData->environment->handleInput(mGameData->inputManager);
}


void GameState::fastUpdate(float dt)
{
	Camera::Get()->fastUpdate(dt);

	mGameData->collisionManager->fastUpdate();

	mGameData->environment->fastUpdate(dt);
}


void GameState::slowUpdate(float dt)
{
	mGameData->environment->slowUpdate(dt);

	mGameData->scoreManager->slowUpdate();

	Camera::Get()->slowUpdate(dt);

	//// End current level, close old level exit, open new level entrance
	//if (mGameData->environment->canClosePreviousLevel(mGameData->environment->actors()->player()->position()))
	//{
	//	nextLevel();
	//}

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
	mGameData->environment->exit();
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
	Camera* camera = Camera::Get();

	VectorF cameraPosition = VectorF(0.0f, 0.0f);
	camera->setPosition(cameraPosition);

	// TODO: fix these values
	camera->initShakeyCam(150.0f, 100.0f, 50.0f);

	RectF* playerRect = &mGameData->environment->actors()->player()->get()->rectRef();
	camera->follow(playerRect);
}


void GameState::initRendering()
{
	mGameData->renderManager->Set(mGameData->environment);
	//mGameData->renderManager->Set(mGameData->actors);
	mGameData->renderManager->Set(mGameData->uiManager);
}


void GameState::nextLevel()
{
	mGameData->environment->nextLevel();

	// end level
	//mGameData->actors->enemies()->clearAllEnemies();
	//mGameData->actors->enemies()->spawnLevel();

	//mCollectables.spawnRandomItem(Collectables::MeleeWeapon);
}