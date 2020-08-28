#pragma once

#include "Collider.h"


class Effect;


class EffectCollider : public Collider
{
public:
	EffectCollider() { }
	EffectCollider(RectF* rect, VectorF relativeColliderSize = VectorF(1.0f, 1.0f)) : Collider(rect, relativeColliderSize) { }

	void addEffect(Effect* effect) { mEffects.push(effect); }
	Effect* popEffect();

	void addNewEffects(UniqueQueue<Effect*>& effects);
	void updateEffectData(const PropertyMap& properties);

	bool hasEffects() const { return mEffects.size() > 0; }
	int effectCount() const { return mEffects.size(); }

private:
	UniqueQueue<Effect*> mEffects;
};
