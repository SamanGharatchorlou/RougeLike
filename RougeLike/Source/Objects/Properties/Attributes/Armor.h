#pragma once


#include "Objects/Properties/Attributes/Attribute.h"
#include "Damage.h"


class Armor : public Attribute
{
public:
	Armor() : mArmor(0), mMaxArmor(0) { }
	Armor(float armor) : mMaxArmor(armor), mArmor(armor) { }
	Armor(float armor, float maxArmor) : mMaxArmor(maxArmor), mArmor(armor) { }

	void init(float armor) override { mMaxArmor = armor; mArmor = armor; }

	void increase(Armor armor) { mArmor = clamp(mArmor + armor.value(), 0.0f, mMaxArmor); }
	void reduce(Damage damage) { mArmor = clamp(mArmor - damage.value(), 0.0f, mMaxArmor); }

	void setValue(float armor) { mArmor = armor; }
	const float value() const override { return mArmor; }

	float getPercentage() const { return mArmor / mMaxArmor; }

	float getMax() const { return mMaxArmor; }
	void increaseMax(float armor) { mArmor += armor; mMaxArmor += armor; }

	bool hasArmor() const { return mArmor > 0; }

	void setFullArmor() { mArmor = mMaxArmor; }

private:
	float mArmor;
	float mMaxArmor;
};