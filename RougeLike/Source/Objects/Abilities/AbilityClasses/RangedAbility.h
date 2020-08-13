#pragma once


#include "Ability.h"
#include "Collisions/Collider.h"

class Map;

class RangedAbility : public Ability
{
public:
	RangedAbility() : mRangeCircle(nullptr) { }
	virtual ~RangedAbility() { }

	void setRangeCircle(Texture* rangeCircle) { mRangeCircle = rangeCircle; }

	void renderRangeCircle();

	bool isValidTarget(VectorF target, const Map* map) const;

	RectF effectArea() const { return mRect; }
	Collider collider();

	virtual bool isRanged() const { return true; }

protected:
	Texture* mRangeCircle;
};