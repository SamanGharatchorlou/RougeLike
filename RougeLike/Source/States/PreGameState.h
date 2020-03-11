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

	// mandatory functions must be defined
	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override { };
	void render() override;
	void exit() override { };

	void resume() { };

private:
	void selectCharacter();
	void enterGame();

private:
	GameController* mGameController;
	GameData* mGameData;

	CharacterSelectionScreen* selectionScreen;
};