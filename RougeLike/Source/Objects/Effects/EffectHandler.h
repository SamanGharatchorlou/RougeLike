#pragma once

class Effect;
class EffectPool;

class EffectHandler
{
public:
	EffectHandler() { }

	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear(EffectPool* pool);
	
	void returnExhaustedEffects(EffectPool* pool);

private:
	void addQueuedEffects();
	void endEffect(std::vector<Effect*>::iterator& iter);


private:
	std::queue<Effect*> mEffectsToAdd;
	std::queue<Effect*> mExhausted;

	std::vector<Effect*> mEffects;
};

