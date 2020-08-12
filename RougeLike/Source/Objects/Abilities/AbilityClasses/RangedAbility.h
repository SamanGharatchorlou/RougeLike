#pragma once


#include "Ability.h"
#include "Collisions/Collider.h"

class Map;

class RangedAbility : public Ability
{
public:
	RangedAbility() : mRange(0.0f), mRangeCircle(nullptr) { }
	virtual ~RangedAbility() { }

	void fillBaseValues(const PropertyMap& properties) override;
	void setRangeCircle(Texture* rangeCircle) { mRangeCircle = rangeCircle; }

	void renderRangeCircle();

	bool isValidTarget(VectorF target, const Map* map);

	RectF effectArea() const { return mRect; }
	Collider collider();

	virtual bool isRanged() const { return true; }

protected:
	float mRange;
	Texture* mRangeCircle;
};