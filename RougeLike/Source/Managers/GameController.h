#pragma once

#include "Game/GameData.h"
#include "States/State.h"
#include "States/StateMachine.h"

// TODO: REMOVE
#include "Debug/DebugDraw.h"


class GameController
{
public:
	GameController();
	~GameController() { }

	void free();

	void init() { };
	void load();
	void run();
	void quitGame() { quit = true; }

	StateMachine<State>* getStateMachine() { return &mGameStateMachine; }
	
	void restartGame();

private:
	void stateChanges();
	void handleInput(SDL_Event& event);
	void updateLoops(float dt);
	void render();

	void endGame();

private:
	GameData mGameData;

	StateMachine<State> mGameStateMachine;

	bool quit;
	bool restart;

#if FRAMERATE_CAP || PRINT_FRAMERATE_EVERY
	Timer<float> fpsTimer;
	Timer<float> capTimer;
#endif
};

static int renderLoadingBar(void *ptr);