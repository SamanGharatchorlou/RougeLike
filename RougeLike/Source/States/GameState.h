#pragma once

#include "State.h"

struct GameData;

class GameState : public State
{
public:
	GameState(GameData* gameData);
	~GameState() { }

	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void handleInput() override;
	void render() override;

	void resume() override;
	void exit() override;


private:
	void initCamera();
	void initRendering();


private:
	GameData* mGameData;
};
