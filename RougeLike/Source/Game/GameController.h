#pragma once

#include "Game/Data/GameData.h"
#include "States/State.h"
#include "States/StateMachine.h"
#include "Debug/FrameRateController.h"

class FrameRateController;
class AudioManager;

enum class SystemStates
{
	None,
	PreGameState,
	GameState,
	PauseState,
};


class GameController
{
public:
	GameController();
	~GameController() { }

	void free();

	void init();
	void preLoad();
	void load();
	void run();
	void quitGame() { quit = true; }

	// System game states
	void addState(SystemStates state);
	void replaceState(SystemStates state);
	void popState();

	StateMachine<State>* getStateMachine() { return &mGameStateMachine; }
	
	void restartGame();

private:
	void stateChanges();
	void handleInput(SDL_Event& event);
	void updateLoops(float dt);
	void render();

	void endGame();

	State* getNewGameState(SystemStates state);

private:
	GameData mGameData;
	AudioManager* mAudio;

	StateMachine<State> mGameStateMachine;

	bool quit;
	bool restart;

	FrameRateController* mFrameRateController;

#if FRAMERATE_CAP || PRINT_FRAMERATE_EVERY
	Timer<float> fpsTimer;
	Timer<float> capTimer;
#endif
};


void loadGameAssets(GameData* data);
void renderLoadingBar();