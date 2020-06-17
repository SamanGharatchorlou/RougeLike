#pragma once

#include "Effect.h"


class DisplacementEffect : public Effect
{
public:
	DisplacementEffect();
	DisplacementEffect(VectorF source, float distance, float force);
	virtual ~DisplacementEffect() { }

	virtual void fillData(const Actor* distributer) override;
	void clearData();

	virtual void init() override { };
	virtual void fastUpdate(float dt) override;
	virtual void slowUpdate(float dt) override { };
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
