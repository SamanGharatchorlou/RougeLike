#include "pch.h"
#include "GameController.h"

#include <SDL_thread.h>

#include "Game/Data/LoadingManager.h"
#include "Game/GameSetup.h"
#include "Debug/FrameRateController.h"

#include "States/PreGameState.h"
#include "States/GameState.h"
#include "States/PauseState.h"


// GameData
#include "System/Window.h"
#include "Audio/AudioManager.h"
#include "Input/inputManager.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Game/Environment.h"
#include "Collisions/CollisionManager.h"


GameController::GameController() : quit(false), mGameStateMachine(new NullState)
{
	GameSetup setup;
	Window* window = setup.initSDL();
	mGameData.setWindow(window);
}


void GameController::init()
{
	mGameData.init(this);
}


void GameController::preLoad()
{
	mGameData.preLoad();
}


void GameController::load()
{
	LoadingManager* loader = LoadingManager::Get();

	loader->init(mGameData.uiManager);

	SDL_Thread* threadID = SDL_CreateThread(renderLoadingBar, "LoadingBar", nullptr);

	//mGameData.init(this);
	mGameData.setupObservers();
	mGameData.load();

	mGameData.uiManager->controller()->popScreen();

	SDL_WaitThread(threadID, nullptr);

	mAudio = AudioManager::Get();
}


void GameController::run()
{
	// add first game state
	addState(SystemStates::PreGameState);

	FrameRateController frameTimer;
	frameTimer.start();

	SDL_Event event;

	// main game loop
	while (quit == false)
	{
#if FRAMERATE_CAP
		frameTimer.resetCapTimer();
#endif

		handleInput(event);
		updateLoops(frameTimer.delta());
		stateChanges();
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
	//GameSetup setup;
	//setup.closeSubSystems();

	GameSetup::closeSubSystems();

	DebugPrint(Log, "GameController destroyed\n");
}


void GameController::restartGame()
{
	DebugPrint(Log, "--- Begin game restart ---\n");

	GameSetup::setTutorial("OFF");

	// Remove all states
	while (mGameStateMachine.size() > 1)
	{
		mGameStateMachine.popState();
		stateChanges();
	}

	mGameData.uiManager->clearScreens();
	mGameData.uiManager->load();

	mGameData.collisionManager->init();

	mGameData.environment->init(&mGameData);
	mGameData.environment->load();
	addState(SystemStates::PreGameState);
	DebugPrint(Log, "--- End game restart ---\n");
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
	mGameData.uiManager->update();
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


void GameController::addState(SystemStates state)
{
	mGameStateMachine.addState(getNewGameState(state));
}


void GameController::replaceState(SystemStates state)
{

	mGameStateMachine.replaceState(getNewGameState(state));
}


void GameController::popState()
{
	mGameStateMachine.popState();
}


State* GameController::getNewGameState(SystemStates state)
{
	State* systemState = nullptr;

	switch (state)
	{
	case SystemStates::PreGameState:
		systemState = new PreGameState(&mGameData);
		break;
	case SystemStates::GameState:
		systemState = new GameState(&mGameData);
		break;
	case SystemStates::PauseState:
		systemState = new PauseState(&mGameData);
		break;
	case SystemStates::None:
	default:
		DebugPrint(Warning, "No valid system state with type %d\n", state);
		break;
	}

	return systemState;
}




static int renderLoadingBar(void *ptr)
{
	DebugPrint(Log, " -------------------------- starting loader thread -------------------------- \n");
	LoadingManager* loading = LoadingManager::Get();

	while (!loading->end())
	{
		loading->update();
		loading->render();
	}

	DebugPrint(Log, " -------------------------- exiting loader thread -------------------------- \n");
	return 0;
}