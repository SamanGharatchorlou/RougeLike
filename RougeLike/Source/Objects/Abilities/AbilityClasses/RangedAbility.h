#pragma once


#include "Ability.h"

class Map;

class RangedAbility : public Ability
{
public:
	RangedAbility() : mRangeCircle(nullptr), mOnlyDirectional(false) { }
	virtual ~RangedAbility() { }

	void init(Actor* caster, const PropertyMap& properties, Animator animator) override;

	void setIsOnlyDirectional(bool isOnlyDirectional) { mOnlyDirectional = isOnlyDirectional; }

	void renderRangeCircle();

	virtual bool isValidTarget(VectorF target, const Map* map) const;

	RectF effectArea() const { return mRect; }


protected:
	Texture* mRangeCircle;
	bool mOnlyDirectional;

};

