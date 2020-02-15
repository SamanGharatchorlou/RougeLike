#pragma once

#include "States/State.h"

struct GameData;
class Texture;
class Player;

class PlayerState : public State
{
public:
	enum actionState
	{
		None,
		Idle,
		Run,
		Attack
	};

public:
	PlayerState() { };

	void setupStateData(GameData* gameData, Player* player);

	Player* getPlayer() { return mPlayer; }

	void addState(actionState newState);
	void popState();
	void changeState(actionState newState);

	actionState getState() { return mState; }

protected:
	GameData* mGameData;
	Player* mPlayer;

	actionState mState;

private:
	Texture* mTexture;
};