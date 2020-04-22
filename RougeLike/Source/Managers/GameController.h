#pragma once
#include "States/StateMachine.h"
#include "Game/GameData.h"


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

#if FRAMERATE_CAP
	//The frames per second timer
	Timer<float> fpsTimer;

	//The frames per second cap timer
	Timer<float> capTimer;
#endif
};