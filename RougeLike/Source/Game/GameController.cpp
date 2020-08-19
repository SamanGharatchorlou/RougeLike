#include "pch.h"
#include "GameController.h"

#include <SDL_thread.h>

#include "Game/Data/LoadingManager.h"
#include "Game/GameSetup.h"
#include "Debug/FrameRateController.h"

#include "States/GameState.h"
#include "States/PreGameState.h"

// GameData
#include "System/Window.h"
#include "Audio/AudioManager.h"
#include "Input/inputManager.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Game/Environment.h"


GameController::GameController() : quit(false), mGameStateMachine(new NullState)
{
	GameSetup setup;
	Window* window = setup.initSDL();
	mGameData.setWindow(window);
}


void GameController::load()
{
	LoadingManager* loader = LoadingManager::Get();

	loader->init();

	SDL_Thread* threadID = SDL_CreateThread(renderLoadingBar, "LoadingBar", nullptr);

	loader->CountToBeLoadedFiles();

	mGameData.init();
	mGameData.setupObservers();
	mGameData.load();

	SDL_WaitThread(threadID, nullptr);

	loader->free();

	mAudio = AudioManager::Get();
}



void GameController::run()
{
	// add first game state
	mGameStateMachine.addState(new PreGameState(&mGameData, this));

	FrameRateController frameTimer;
	frameTimer.start();

	SDL_Event event;

	// main game loop
	while (quit == false)
	{
#if FRAMERATE_CAP
		frameTimer.resetCapTimer();
#endif

		stateChanges();
		handleInput(event);
		updateLoops(frameTimer.delta());
		render();

		frameTimer.update();
	}

	endGame();
	DebugPrint(Log, "Game Ended\n");
}

void GameController::endGame()
{
	mGameStateMachine.~StateMachine();
}


void GameController::free()
{
	mGameData.free();

	// delete globals
	FileManager::Get()->free();
	Renderer::Get()->free();

	// quit SDL subsystems
	GameSetup setup;
	setup.closeSubSystems();

	DebugPrint(Log, "GameController destroyed\n");
}


// TODO: correctly implement this, you need to first break everything down,
// then restart it all again
void GameController::restartGame()
{
	PRINT_MEMORY;

	DebugPrint(Log, "Unimplemented\n");

	//// Remove all states
	while (mGameStateMachine.size() > 1)
	{
		mGameStateMachine.popState();
		stateChanges();
	}

	mGameData.uiManager->clearScreens();
	mGameData.uiManager->setupScreens();

	mGameData.collisionManager->init();

	mGameData.environment->init(&mGameData);
	mGameData.environment->load();
	mGameStateMachine.addState(new PreGameState(&mGameData, this));

	PRINT_MEMORY;
}


// --- Private Functions --- //

void GameController::handleInput(SDL_Event& event)
{
	// handle input events
	mGameData.inputManager->resetInputEvents();

	while (SDL_PollEvent(&event))
	{
		mGameData.inputManager->processInputEvent(event);
	}

	mGameData.uiManager->handleInput(mGameData.inputManager);
	mGameStateMachine.getActiveState().handleInput();

#if DEBUG_CURSOR // show mouse position in screen title
	VectorF pos = mGameData.inputManager->cursorPosition();
	BasicString cursor = BasicString("X: ") + BasicString(pos.x) + BasicString(" Y: ") + BasicString(pos.y);
	mGameData.window->setTitle(cursor.c_str());
#endif
}

void GameController::updateLoops(float dt)
{
	// Fast update runs updateLoopRepeats number of times per frame
	float updateLoopRepeats = 10;
	for (int i = 0; i < updateLoopRepeats; i++)
		mGameStateMachine.getActiveState().fastUpdate(dt / updateLoopRepeats);

	mGameStateMachine.getActiveState().slowUpdate(dt);
	mAudio->slowUpdate();
	mGameData.uiManager->update(dt);
}


void GameController::render()
{
	mGameStateMachine.getActiveState().render();
}

void GameController::stateChanges()
{
	State* state = mGameStateMachine.processStateChanges();
	if (state)
		delete state;
}


static int renderLoadingBar(void *ptr)
{
	DebugPrint(Log, " -------------------------- starting loader thread -------------------------- \n");
	LoadingManager* loading = LoadingManager::Get();

	while (!loading->end())
	{
		loading->render();
	}

	DebugPrint(Log, " -------------------------- exiting loader thread -------------------------- \n");
	return 0;
}