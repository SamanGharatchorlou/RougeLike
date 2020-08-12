#pragma once

#include "EffectHandler.h"
#include "Utilities/Maps/EffectMap.h"

class EffectPool;
class Effect;

class EffectManager
{
public:
	void init(EffectPool* pool) { mPool = pool; }
	void clear();

	void fillEffectBag(XMLNode effectNode);
	void setEffectBag(EffectMap EffectMap) { mBag = EffectMap; }

	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addReceivedEffect(Effect* effect);
	std::queue<Effect*> getAttackingEffects();

	void returnEffect(Effect* effect);


private:
	EffectPool* mPool;

	EffectHandler mHandler;
	EffectMap mBag;
};