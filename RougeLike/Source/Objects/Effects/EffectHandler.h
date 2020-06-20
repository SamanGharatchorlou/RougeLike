#pragma once

class EffectPool;
class Actor;

#include "Effect.h"


class EffectHandler
{
public:
	EffectHandler(EffectPool* pool, Actor* actor) : mPool(pool), mActor(actor) { };

	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();

	void addAttackingEffect(EffectType type) { mAttackingEffects.push_back(type); }
	const std::vector<EffectType> attackingEffects() const { return mAttackingEffects; }

private:
	EffectPool* mPool;
	Actor* mActor;

	std::vector<EffectType> mAttackingEffects;

	std::vector<Effect*> mEffects;

	// TODO: do i still need this?
	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;

};

