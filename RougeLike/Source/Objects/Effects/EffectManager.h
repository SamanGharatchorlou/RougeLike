#pragma once

#include "EffectHandler.h"

class EffectPool;
class Effect;


class EffectManager
{
public:
	void init(EffectPool* pool) { mPool = pool; }
	void clear();

	EffectPool* pool() { return mPool; }

	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addEffect(Effect* effect);
	void returnEffect(Effect* effect);


private:
	EffectPool* mPool;
	EffectHandler mHandler;
};