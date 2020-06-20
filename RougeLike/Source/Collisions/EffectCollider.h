#pragma once

#include "Collider.h"


class Effect;


class EffectCollider : public Collider
{
public:
	EffectCollider() : mRequiresUpdate(true) { }

	void addEffect(Effect* effect) { mEffects.push(effect); }
	Effect* popEffect();

	bool hasEffects() const { return mEffects.size() > 0; }
	int effectCount() const { return mEffects.size(); }

private:
	std::queue<Effect*> mEffects;
	bool mRequiresUpdate;
};
