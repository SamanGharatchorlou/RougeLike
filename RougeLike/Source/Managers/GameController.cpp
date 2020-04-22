#include "pch.h"
#include "GameController.h"

#include "States/GameState.h"
// TEMP
#include "States/PreGameState.h"

// GameData
#include "System/Window.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Input/inputManager.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"
#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Map/Map.h"
#include "Game/Camera.h"
#include "Game/Cursor.h"



// init all SDL subsystems
GameController::GameController(const char* gameTitle) : quit(false), mGameStateMachine(new NullState)
{
	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		DebugPrint(Error, "SDL could not be initialised! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		mGameData.window = new Window;
		mGameData.window->init(gameTitle, 1024, 768);

		if (mGameData.window->get() == nullptr)
		{
			DebugPrint(Error, "Window could not be created! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// init window renderer
			SDL_Renderer* sdlRenderer = mGameData.window->createRenderer();
			Renderer* renderer = Renderer::Get()->create(sdlRenderer);

			if (!renderer->sdlRenderer())
			{
				DebugPrint(Error, "Renderer could not be created! SDL Image Error: %s\n", IMG_GetError());
			}
			else
			{
				// init PNG loader
				if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
				{
					DebugPrint(Error, "SDL Image was not initialised! SDL Image Error: %s\n", IMG_GetError());
				}


				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					DebugPrint(Error, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				}

				// init audio mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					DebugPrint(Error, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				}

				SDL_ShowCursor(SDL_DISABLE);
			}
		}
	}

	DebugPrint(Log, "GameController create with title: %s\n", gameTitle);
}


// destory all SDL subsystems
GameController::~GameController()
{
	mGameData.free();

	// delete globals
	Renderer::Get()->free();


	// quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	// finsihed
	DebugPrint(Log, "GameController destroyed\n");
}


void GameController::init()
{
	mGameData.init();
}


void GameController::run()
{
	// add first game state
	mGameStateMachine.addState(new PreGameState(&mGameData, this));

	// frame rate timer
	Timer<float> frameTimer(Timer<float>::START);
	float dt = 0.0f;

	// Run the update loop this many times every frame
	float updateLoopRepeats = 10;

	SDL_Event event;

#if PRINT_FRAMERATE
	float fpsSum = 0.0f;
	int fpsCounter = 0;
	float totalFrameTime = 0;
	int totalFrames = 0;
#endif

	// main game loop
	while (quit == false)
	{
#if FRAMERATE_CAP
		capTimer.start();
#endif

		// change state, init if required
		mGameStateMachine.processStateChanges();

		// process state changes other objects like players, enemies etc.
		mGameStateMachine.getActiveState().preProcess();

		// handle input events
		mGameData.inputManager->update();
		while (SDL_PollEvent(&event))
		{
			mGameData.inputManager->processInputEvent(event);
		}
		
		mGameStateMachine.getActiveState().handleInput();
		mGameData.uiManager->handleInput();

#if _DEBUG // show mouse position in screen title
		VectorF pos = mGameData.cursor->getPosition();
		std::stringstream ss;
		ss << "X: " << pos.x << " Y: " << pos.y;
		mGameData.window->setTitle(ss.str().c_str());
#endif

		// Fast update runs updateLoopRepeats number of times per frame
		for (int i = 0; i < updateLoopRepeats; i++)
			mGameStateMachine.getActiveState().fastUpdate(dt / updateLoopRepeats);

		// Slow update runs once per frame
		mGameStateMachine.getActiveState().slowUpdate(dt);

		mGameData.uiManager->update();

		mGameStateMachine.getActiveState().render();


#if PRINT_FRAMERATE
		fpsSum += frameTimer.getMilliseconds();
		fpsCounter++;
		
		totalFrameTime += frameTimer.getMilliseconds();
		totalFrames++;

		if (fpsCounter == 100)
		{
			fpsSum /= fpsCounter;

			printf("framerate %f | Running fps average %f\n", 1000 / fpsSum, 1000 / (totalFrameTime / totalFrames));
			fpsSum == 0.0;
			fpsCounter = 0;
		}
#endif

		dt = frameTimer.getSeconds();
		frameTimer.restart();

#if FRAMERATE_CAP
		//If frame finished early
		int frameTicks = capTimer.getMilliseconds();
		if (frameTicks < (1000 / FRAMERATE_CAP))
		{
			//Wait remaining time
			SDL_Delay((1000 / FRAMERATE_CAP) - frameTicks);
		}
#endif
	}

	DebugPrint(Log, "Quitting Game\n");
}


// TODO: correctly implement this, you need to first break everything down,
// then restart it all again
void GameController::restartGame()
{
	//// Remove all states
	//while (mGameStateMachine.size() > 1)
	//{
	//	mGameStateMachine.popState();
	//	mGameStateMachine.processStateChanges();
	//}

	//mGameStateMachine.addState(new GameState(&mGameData, this));
	DebugPrint(Log, "Restart game unimplemented\n");
}