#pragma once

#include "EffectHandler.h"
#include "AttackingEffects.h"

class EffectPool;
class Effect;
enum class EffectType;


class EffectManager
{
public:
	void init(EffectPool* pool) { mPool = pool; }
	void clear();

	void fillAttackingEffectData(XMLNode effectNode) { mAttackingEffects.fill(effectNode); }
	UniqueQueue<Effect*> getNewAttackingEffects();

	void setAttackingEffectdata(AttackingEffects attackingData) { mAttackingEffects = attackingData; }
	PropertyMap& attackingData() { return mAttackingEffects.mData; }

	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addReceivedEffect(Effect* effect);

	void returnEffect(Effect* effect);


private:
	EffectPool* mPool;
	EffectHandler mHandler;

	AttackingEffects mAttackingEffects;
};