#include "pch.h"
#include "GameState.h"

// TODO: clean up these includes
#include "Game/Data/GameData.h"

#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "Managers/ScoreManager.h"

#include "Game/Environment.h"
#include "Map/Map.h"
#include "Game/Camera/Camera.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
#include "Actors/Enemies/EnemyManager.h"



#include "Utilities/Quad2D.h"
#include "Collisions/Colliders/QuadCollider.h"


GameState::GameState(GameData* gameData) : mGameData(gameData) { }


void GameState::init()
{
	Map* map = mGameData->environment->map(VectorF(0, 0));
	VectorF playerPosition = map->randomFloorTile(50)->rect().Center();

	mGameData->environment->actors()->player()->setPosition(playerPosition);

	initCamera();
	initRendering();

	// Start Audio
	AudioManager* audio = AudioManager::Get();
	audio->playMusic("Ludumdum");
	audio->setMusicVolume(0.0f);
	audio->setSoundVolume(0.0f);
}


void GameState::handleInput()
{
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

	//mGameData->scoreManager->slowUpdate();

	Camera::Get()->slowUpdate(dt);

	// End of slow frame
#if !TRACK_COLLISIONS // Moved to end of render function
	mGameData->collisionManager->resetColliders();
#endif
}


void GameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mGameData->renderManager->render();




	// player rect
	//RectF playerRect = mGameData->environment->actors()->player()->get()->rect();
	//Quad2D<float> quad(playerRect);

	//VectorF center = playerRect.Center();

	//quad.rotate(45.0, center);

	//debugDrawQuad(quad, RenderColour::Blue);

	
	//// draw normal
	//VectorF p1 = quad[0];
	//VectorF p2 = quad[1];

	//VectorF midpoint = (p1 + p2) / 2;

	//float dx = p2.x - p1.x;
	//float dy = p2.y - p1.y;

	//VectorF n1 = VectorF(-dy, dx);
	//VectorF n2 = VectorF(dy, -dx);

	//debugDrawLine(center, center + (quad.normal0()*50.0f), RenderColour::Yellow);
	//debugDrawLine(center, center + (quad.normal1()*50.0f), RenderColour::LightGrey);




	//// draw enemy rect
	//std::vector<Actor*> enemies = mGameData->environment->actors()->getAllEnemies();
	//Actor* enemy = nullptr;
	//if (enemies.size() > 0)
	//	enemy = enemies[0];

	//RectF enemyRect = enemy->rect();
	//debugDrawRect(enemyRect, RenderColour::Black);

	//Collider enemyColl(&enemyRect);
	//QuadCollider quadCollider(quad);
	//if (quadCollider.doesIntersect(&enemyColl))
	//{
	//	debugDrawRect(enemyRect, RenderColour::Green);
	//}
	//else
	//{
	//	debugDrawRect(enemyRect, RenderColour::Red);
	//}



	// update window surface
	SDL_RenderPresent(renderer);


#if TRACK_COLLISIONS // Need to keep the hit data until after rendering
	mGameData->collisionManager->resetColliders();
#endif
}


void GameState::resume() { }


void GameState::exit()
{
	mGameData->environment->clear();
	mGameData->scoreManager->reset();
	mGameData->collisionManager->clearColliders();
}


// --- Private Functions --- //

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
	mGameData->renderManager->Set(mGameData->uiManager);
}
