#pragma once

#include "Effect.h"


class DisplacementEffect : public Effect
{
public:
	DisplacementEffect();
	virtual ~DisplacementEffect() { }

	void fill(const PropertyMap& propertyMap) override;
	//void set(VectorF source, float force, float distance);
	void update(VectorF source) { mSource = source; }

	virtual void init() override { };
	virtual void fastUpdate(float dt) override;
	virtual void slowUpdate(float dt) override { };
	virtual void exit();
	void render() override { };

	EffectType type() const override { return EffectType::Displacement; }


protected:
	VectorF mSource;

	float mDistance;
	float mForce;

	float mDistanceTravelled;
};
 