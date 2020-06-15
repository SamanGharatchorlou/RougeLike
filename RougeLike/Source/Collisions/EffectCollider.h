#pragma once

#include "Collider.h"

class Effect;


class EffectCollider : public Collider
{
public:
	bool hasEffect() const { return (bool)mEffects.size(); }
	int effectCount() const { return mEffects.size(); }

	void addEffect(Effect* effect) { mEffects.push(effect); }
	Effect* popEffect();



private:
	std::queue<Effect*> mEffects;
};