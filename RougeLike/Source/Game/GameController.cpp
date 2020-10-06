#include "pch.h"
#include "GameController.h"

#include <thread>
#include "Graphics/Renderer.h"

#include "Game/Data/LoadingManager.h"
#include "Game/GameSetup.h"

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


GameController::GameController() : quit(false), mGameStateMachine(new NullState)
{
	GameSetup setup;
	setup.initFileSystem();

	Window* window = setup.initSDLWindow();
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
	mGameData.setupObservers();

	std::thread assetLoader(loadGameAssets, &mGameData);
	std::thread loadingscreen(renderLoadingBar);

	SDL_Event events;

	while (loader->isLoadingAssets())
	{
		while (SDL_PollEvent(&events))
		{
			if (events.type == SDL_QUIT)
			{
				DebugPrint(Warning, "Early exitting loading, closing application\n");
				LoadingManager::Get()->earlyExit();
				quitGame();
				break;
			}
		}
	}

	assetLoader.join();
	loadingscreen.join();

	mGameData.uiManager->controller()->popScreen();
}


void loadGameAssets(GameData* data)
{
	data->load();
}


void GameController::run()
{
	// add first game state
	addState(SystemStates::PreGameState);

	mFrameTimer.start();

	SDL_Event event;

	// main game loop
	while (quit == false)
	{
#if FRAMERATE_CAP
		mFrameTimer.resetCapTimer();
#endif

		handleInput(event);
		updateLoops(mFrameTimer.delta());

		stateChanges();
		render();

		mFrameTimer.update();
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

	GameSetup::closeSubSystems();

	DebugPrint(Log, "GameController destroyed\n");
}


void GameController::restartGame()
{
	DebugPrint(Log, "--- Begin game restart ---\n");

	GameSetup::setTutorial("OFF");
	AudioManager::Get()->resetController();

	// Remove all states
	while (mGameStateMachine.size() > 1)
	{
		mGameStateMachine.popState();
		stateChanges();
	}

	mGameData.uiManager->clearScreens();
	mGameData.uiManager->load();

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
		if (event.type == SDL_QUIT)
			quit = true;

		mGameData.inputManager->processInputEvent(event);
	}

	mGameData.uiManager->handleInput(mGameData.inputManager);
	mGameStateMachine.getActiveState().handleInput();

#if DEBUG_CURSOR // show mouse position in screen title
	VectorF pos = mGameData.inputManager->cursorPosition();
	const BasicString cursor = "X: " + BasicString(pos.x) + " Y: " + BasicString(pos.y);
	mGameData.window->setTitle(cursor.c_str());
#endif
}

void GameController::updateLoops(float dt)
{
	AudioManager::Get()->slowUpdate();

	// Fast update runs updateLoopRepeats number of times per frame
	float updateLoopRepeats = 10;
	for (int i = 0; i < updateLoopRepeats; i++)
		mGameStateMachine.getActiveState().fastUpdate(dt / updateLoopRepeats);

	mGameStateMachine.getActiveState().slowUpdate(dt);

	mGameData.uiManager->update();

	AudioManager::Get()->handleEvents();
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
	mFrameTimer.resetAverage();
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


void renderLoadingBar()
{
	DebugPrint(Log, " -------------------------- starting loader thread -------------------------- \n");
	LoadingManager* loading = LoadingManager::Get();
	
	TimerF timer;
	timer.start();
	float renderFPS = 20;


	while (loading->isLoadingAssets())
	{
		if (!loading->shouldEarlyExit())
		{
			loading->update();
		}

		// Dont want to hog the renderer too much as its used for loading textures, fonts etc
		if (timer.getMilliseconds() > (1000 / renderFPS))
		{
			loading->render();
			timer.restart();
		}
	}

	DebugPrint(Log, "\n\nloading time taken: %fs\n", timer.getSeconds());
	DebugPrint(Log, " -------------------------- exiting loader thread -------------------------- \n");
}

