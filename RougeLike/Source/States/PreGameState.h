#pragma once

#include "State.h"

struct GameData;
class GameController;
class CharacterSelectionScreen;

class PreGameState : public State
{
public:
	PreGameState(GameData* gameData, GameController* gameController);
	~PreGameState() { }

	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override { };
	void render() override;
	void exit() override { };

	void resume() { };


private:
	GameController* mGameController;
	GameData* mGameData;
};