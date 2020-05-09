#pragma once


enum class PropertyType
{
	Level,
	Health,
	AttackDamage,
	AttackSpeed,

	// Physics
	Force,
	MaxVelocity,
};


class PropertyBase
{
public:
	PropertyBase(PropertyType type) : mType(type) { }

	virtual float value() const = 0;
	PropertyType type() const { return mType; }

private:
	PropertyType mType;
};


class PropertyValue : public PropertyBase
{
public:
	PropertyValue(PropertyType type) : PropertyBase(type) { }

	void setValue(float value) { mValue = value; }
	float value() const { return mValue; }


private:
	float mValue;
};