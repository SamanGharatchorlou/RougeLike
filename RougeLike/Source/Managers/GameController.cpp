#include "pch.h"
#include "GameController.h"

#include "States/GameState.h"

#include "Graphics/Renderer.h"

// GameData
#include "System/Window.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Input/inputManager.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"

#include "Map/Map.h"
#include "Game/Camera.h"
#include "Game/Cursor.h"



// init all SDL subsystems
GameController::GameController(const char* gameTitle) : quit(false)
{
	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		DebugPrint(Error, "SDL could not be initialised! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		mGameData.window = new Window(gameTitle, 1024.0f, 768.0f);

		if (mGameData.window->get() == nullptr)
		{
			DebugPrint(Error, "Window could not be created! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// init window renderer
			SDL_Renderer* renderer = mGameData.window->createRenderer();

			Renderer::Get()->create(renderer);
			mGameData.renderer = renderer;

			if (!mGameData.renderer)
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
	// destroy renderer
	SDL_DestroyRenderer(mGameData.renderer);
	mGameData.renderer = nullptr;

	// destory window
	delete mGameData.window;

	// managers
	// ERROR: when there are no textures or audio files in the map gives error... why?
	delete mGameData.textureManager;
	delete mGameData.audioManager;
	delete mGameData.inputManager;
	delete mGameData.renderManager;
	delete mGameData.uiManager;

	delete mGameData.camera;
	delete mGameData.cursor;

	// quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	// finsihed
	DebugPrint(Log, "GameController destroyed\n");
}


void GameController::init()
{
	// Must be before cursor
	mGameData.textureManager = new TextureManager();
	mGameData.textureManager->init(mGameData.renderer);

	mGameData.cursor = new Cursor();
	mGameData.cursor->setSize(25.0f, 25.0f);

	// Must be before UIManager
	mGameData.camera = new Camera();
	mGameData.camera->setViewport(1024, 768);

	mGameData.map = new Map(&mGameData);

	// Audio
	mGameData.audioManager = new AudioManager();
	mGameData.audioManager->init();

	// Input
	mGameData.inputManager = new InputManager(mGameData.cursor);
	mGameData.inputManager->init();

	// UI
	mGameData.uiManager = new UIManager(&mGameData);
	mGameData.uiManager->init();

	// Renderer
	mGameData.renderManager = new RenderManager(&mGameData);

	// Score Manager
	mGameData.scoreManager = new ScoreManager;
}


void GameController::run()
{
	// add first game state
	mGameStateMachine.addState(new GameState(&mGameData, this));

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

		// Slow update runs once per frame
		mGameStateMachine.getActiveState().slowUpdate(dt);

		// Fast update runs updateLoopRepeats number of times per frame
		for (int i = 0; i < updateLoopRepeats; i++)
			mGameStateMachine.getActiveState().fastUpdate(dt / updateLoopRepeats);


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
	}

	DebugPrint(Log, "Quitting Game\n");
}
