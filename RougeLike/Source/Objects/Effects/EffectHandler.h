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

	void clear();
	
	void returnExhaustedEffects(EffectPool* pool);


private:
	std::vector<Effect*> mEffects; // turn this into a linked list?

	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;

	std::queue<Effect*> mExhausted;
};

