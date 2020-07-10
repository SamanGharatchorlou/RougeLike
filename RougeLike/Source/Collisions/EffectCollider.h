#pragma once

#include "Collider.h"


class Effect;


class EffectCollider : public Collider
{
public:
	EffectCollider() : mRequiresUpdate(true) { }
	EffectCollider(RectF* rect, VectorF relativeColliderSize = VectorF(1.0f, 1.0f)) : Collider(rect, relativeColliderSize), mRequiresUpdate(true) { }

	void addEffect(Effect* effect) { mEffects.push(effect); }
	Effect* popEffect();

	bool hasEffects() const { return mEffects.size() > 0; }
	int effectCount() const { return mEffects.size(); }

private:
	std::queue<Effect*> mEffects;
	bool mRequiresUpdate;
};
