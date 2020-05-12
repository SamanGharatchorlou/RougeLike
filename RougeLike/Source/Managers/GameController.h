#pragma once

#include "Game/GameData.h"
#include "States/State.h"
#include "States/StateMachine.h"


class GameController
{
public:
	GameController(const char* gameTitle);
	~GameController();

	void init();
	void run();
	void quitGame() { quit = true; }

	StateMachine<State>* getStateMachine() { return &mGameStateMachine; }
	
	void restartGame();

private:
	GameData mGameData;

	StateMachine<State> mGameStateMachine;

	bool quit;
	bool restart;

#if FRAMERATE_CAP
	Timer<float> fpsTimer;
	Timer<float> capTimer;
#endif
};