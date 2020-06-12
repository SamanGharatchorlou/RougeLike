#pragma once

class Effect;
class Actor;

class EffectHandler
{
public:
	EffectHandler(Actor* actor) : mActor(actor) { };

	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();


private:
	Actor* mActor;
	std::vector<Effect*> mEffects;

	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;
};