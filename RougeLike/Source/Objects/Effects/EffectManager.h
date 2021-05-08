#pragma once

#include "EffectHandler.h"

class EffectPool;
class Effect;
enum class EffectType;

class EffectManager
{
public:
	EffectManager() : mPool(nullptr) { }
	void init(EffectPool* pool) { mPool = pool; }
	void clear();

	EffectPool* pool() { return mPool; }

	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addEffect(Effect* effect);
	void returnEffect(Effect* effect);

	bool hasEffect(EffectType type) const;

private:
	EffectPool* mPool;
	EffectHandler mHandler;
};