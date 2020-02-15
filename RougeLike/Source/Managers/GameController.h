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

	StateMachine* getStateMachine() { return &mGameStateMachine; }

private:
	GameData mGameData;
	StateMachine mGameStateMachine;

	bool quit;
};