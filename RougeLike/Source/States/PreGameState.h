#pragma once

#include "State.h"

struct GameData;

class PreGameState : public State
{
public:
	PreGameState(GameData* gameData);
	~PreGameState() { }

	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override { };
	void render() override;
	void exit() override;

	void resume() { };


private:
	GameData* mGameData;	
	
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;
};