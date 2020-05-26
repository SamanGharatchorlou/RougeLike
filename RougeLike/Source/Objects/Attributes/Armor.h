#pragma once

#include "Damage.h"
#include "Objects/Properties/Property.h"
#include "Utilities/Helpers.h"


class Armor : public Property
{
public:
	Armor() : mArmor(0), mMaxArmor(0) { }
	Armor(float armor) : mMaxArmor(armor), mArmor(armor) { }
	Armor(float armor, float maxArmor) : mMaxArmor(maxArmor), mArmor(armor) { }

	void increase(float armor) { mArmor = clamp(mArmor + armor, 0.0f, mMaxArmor); }
	void reduce(Damage damage) { mArmor = clamp(mArmor - damage.value(), 0.0f, mMaxArmor); }

	void setValue(float armor) { mArmor = armor; }
	const float value() const override { return (float)mArmor; }

	float getPercentage() const { return (float)mArmor / (float)mMaxArmor; }

	int getMax() const { return mMaxArmor; }
	void increaseMax(float armor) { mArmor += armor; mMaxArmor += armor; }

	bool hasArmor() const { return mArmor > 0; }

	void setFullArmor() { mArmor = mMaxArmor; }

private:
	float mArmor;
	float mMaxArmor;
};