#pragma once

// abstract class to be inherited by all game state objects
struct State
{
	// pure virtual functions - require override
	virtual void init() = 0;
	virtual void handleInput() = 0;

	// State changes etc.
	virtual void slowUpdate(float dt) = 0;

	// Movement, collisions etc 
	virtual void fastUpdate(float dt) = 0;

	virtual void render() = 0;
	virtual void exit() = 0;

	// optional functions
	virtual void pause() {  }
	virtual void resume() {  }
};


// The null state does nothing
struct NullState : public State
{
	void init() override {}
	void handleInput() override {}
	void slowUpdate(float /*dt*/) override { /* do nothing */ }
	void fastUpdate(float /*dt*/) override { /* do nothing */ }
	void render() override {}
	void exit() override { 
		DebugPrint(Warning, "Removing the null state\n"); 
	}
};