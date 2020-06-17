#pragma once

class EffectPool;
class Actor;

#include "Effect.h"


class EffectHandler
{
public:
	EffectHandler(EffectPool* pool, Actor* actor) : mPool(pool), mActor(actor) { };

	void addEffect(Effect* effect);
	//void addEffect(const EffectData* data);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();

	void addAttackingEffect(EffectType type) { mAttackingEffects.push_back(type); }
	const std::vector<EffectType> attackingEffects() const { return mAttackingEffects; }


	// temp
	int counter = 0;

private:
	EffectPool* mPool;
	Actor* mActor;

	std::vector<Effect*> mEffects;

	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;

	std::vector<EffectType> mAttackingEffects;
};

