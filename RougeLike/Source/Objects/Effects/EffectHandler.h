#pragma once

class Effect;
class EffectPool;

class EffectHandler
{
	friend class EffectManager;

public:
	EffectHandler() { }

	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear(EffectPool* pool);
	
	Queue<Effect*>& exhaustedEffects() { return mExhausted; }

private:
	void addQueuedEffects();
	void endEffect(std::vector<Effect*>::iterator& iter);


private:
	Queue<Effect*> mEffectsToAdd;
	Queue<Effect*> mExhausted;

	std::vector<Effect*> mEffects;
};

