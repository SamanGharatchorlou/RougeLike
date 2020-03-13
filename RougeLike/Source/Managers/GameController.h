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
};