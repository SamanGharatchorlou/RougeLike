#pragma once

#include "EffectHandler.h"
#include "Objects/Bags/EffectBag.h"

class EffectPool;
class Effect;

class EffectManager
{
public:
	void init(EffectPool* pool) { mPool = pool; }
	void clear();

	void fillEffectBag(XMLNode effectNode);
	void setEffectBag(EffectBag effectBag) { mBag = effectBag; }

	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addReceivedEffect(Effect* effect);
	std::queue<Effect*> getAttackingEffects();

	void returnEffect(Effect* effect);


private:
	EffectPool* mPool;

	EffectHandler mHandler;
	EffectBag mBag;
};