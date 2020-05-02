#pragma once

class Effect;

class EffectHandler
{
public:
	void addEffect(Effect* effect);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render() { };


private:
	std::vector<Effect*> mEffects;
};