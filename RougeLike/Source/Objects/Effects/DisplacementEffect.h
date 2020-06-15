#pragma once

#include "Effect.h"


class DisplacementEffect : public Effect
{
public:
	DisplacementEffect(VectorF source, float distance, float force);
	virtual ~DisplacementEffect() { }

	void setSource(VectorF source) { mSource = source; }

	virtual void init() override { };
	virtual void fastUpdate(float dt) override;
	virtual void slowUpdate(float dt) override { };
	void render() override { };
	void exit() override { };


protected:
	virtual bool canMove(VectorF velocity, float dt) const;


protected:
	VectorF mSource;

	float mDistance;
	float mForce;

	float mDistanceTravelled;
};
