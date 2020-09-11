#pragma once

#include "States/State.h"

class InputManager;
class Player;

class PlayerState : public State
{
public:
	virtual ~PlayerState() { }

	virtual void init() override = 0;
	virtual void handleInput() override { }
	virtual void fastUpdate(float dt) override = 0;
	virtual void slowUpdate(float dt) override =0;
	virtual void render() override = 0;
	virtual void exit() override = 0;

	virtual bool finished() { return false; }
};


// The null state does nothing
struct PlayerNullState : public PlayerState
{
	void init() override { }
	void handleInput() override { }
	void handleInput(const InputManager* input) { };
	void slowUpdate(float /*dt*/) override { /* do nothing */ }
	void fastUpdate(float /*dt*/) override { /* do nothing */ }
	void render() override { }
	void exit() override { DebugPrint(Warning, "Removing the null state\n"); }
};