#pragma once


class Effect
{

public:
	Effect() { }
	virtual ~Effect() { }

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }

private:
	bool mShouldExit = false;
};