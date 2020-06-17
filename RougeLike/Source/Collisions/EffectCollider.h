#pragma once

#include "Collider.h"


class Effect;


class EffectCollider : public Collider
{
public:
	EffectCollider() : mRequiresUpdate(true) { }

	void addEffect(Effect* effect) { mEffects.push(effect); }
	Effect* popEffect();

	bool hasEffects() const { return (bool)mEffects.size(); }
	int effectCount() const { return mEffects.size(); }

private:
	std::queue<Effect*> mEffects;
	bool mRequiresUpdate;
};
