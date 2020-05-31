#pragma once

#include "Game/GameData.h"
#include "States/State.h"
#include "States/StateMachine.h"

// TODO: REMOVE
#include "Debug/DebugDraw.h"


class GameController
{
public:
	GameController(const char* gameTitle);
	~GameController();

	void init() { };
	void load();
	void run();
	void quitGame() { quit = true; }

	StateMachine<State>* getStateMachine() { return &mGameStateMachine; }
	
	void restartGame();

	int threadFunction(void* data);

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