#pragma once


#include "Ability.h"

class Map;

class RangedAbility : public Ability
{
public:
	RangedAbility() : mRangeCircle(nullptr), mOnlyDirectional(false) { }
	virtual ~RangedAbility() { }

	void setRangeCircle(Texture* rangeCircle) { mRangeCircle = rangeCircle; }
	void setIsOnlyDirectional(bool isOnlyDirectional) { mOnlyDirectional = isOnlyDirectional; }

	void renderRangeCircle();

	virtual bool isValidTarget(VectorF target, const Map* map) const;

	RectF effectArea() const { return mRect; }
	Collider collider() { return mCollider; }

	virtual bool isRanged() const { return true; }

protected:
	Texture* mRangeCircle;
	bool mOnlyDirectional;

};

