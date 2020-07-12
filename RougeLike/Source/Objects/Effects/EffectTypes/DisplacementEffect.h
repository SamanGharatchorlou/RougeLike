#pragma once

#include "Effect.h"


class DisplacementEffect : public Effect
{
public:
	DisplacementEffect();
	virtual ~DisplacementEffect() { }

	void set(VectorF source, float force, float distance);
	void update(VectorF source) { mSource = source; }
/*
	virtual void fillData(const EffectPropertyBag* properties) override;*/
	void clearData() override;

	virtual void init() override { };
	virtual void fastUpdate(float dt) override;
	virtual void slowUpdate(float dt) override { };
	virtual void exit() { }
	void render() override { };

	EffectType type() const override { return EffectType::Displacement; }


protected:
	virtual bool canMove(VectorF velocity, float dt) const;


protected:
	VectorF mSource;

	float mDistance;
	float mForce;

	float mDistanceTravelled;
};
