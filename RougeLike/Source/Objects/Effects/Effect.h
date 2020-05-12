#pragma once

#include "Objects/Actor.h"

class Effect
{

public:
	Effect() : mShouldExit(false) { }
	virtual ~Effect() { }

	void set(Actor* actor) { mActor = actor; }

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }


protected:
	Actor* mActor;
	bool mShouldExit;
};