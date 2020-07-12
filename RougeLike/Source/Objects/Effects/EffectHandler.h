#pragma once

class EffectPool;
class Actor;

#include "EffectTypes/Effect.h"


class EffectHandler
{
public:
	EffectHandler(EffectPool* pool, Actor* actor) : mPool(pool), mActor(actor) { };

	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();

private:
	EffectPool* mPool;
	Actor* mActor;

	std::vector<Effect*> mEffects;

	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;
};

