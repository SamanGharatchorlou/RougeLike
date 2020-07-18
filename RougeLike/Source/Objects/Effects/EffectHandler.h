#pragma once

class Effect;

class EffectHandler
{
public:
	EffectHandler() { }

	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();

private:
	std::vector<Effect*> mEffects; // turn this into a linked list?

	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;
};

