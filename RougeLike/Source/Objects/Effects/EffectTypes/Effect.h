#pragma once

class Actor;
class Map;

#include "EffectTypes.h"


class Effect
{

public:
	Effect() : mReceiver(nullptr), mShouldExit(false) { }
	virtual ~Effect() { }

	void setReceiver(Actor* receiver) { mReceiver = receiver; }
	
	virtual void clearData() { };
	virtual void fill(ValueMap& valueMap) { } // = 0;

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }

	virtual EffectType type() const = 0;


protected:
	void clearBaseData() { mReceiver = nullptr; mShouldExit = false; }


protected:
	Actor* mReceiver;
	bool mShouldExit;
};


bool canMove(const Actor* actor, VectorF velocity, float dt);
bool isValidFloor(const Map* map, VectorF point);