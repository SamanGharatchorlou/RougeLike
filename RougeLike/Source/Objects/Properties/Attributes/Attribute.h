#pragma once

class Attribute
{
public:
	Attribute() { }
	virtual ~Attribute() { };
	
	virtual void init(float value) { setValue(value); }
	virtual void setValue(float value) = 0;
	virtual const float value() const = 0;
};


class AttributeValue : public Attribute
{
public:
	AttributeValue() : mValue(0.0f) { }
	AttributeValue(float value) : mValue(value) { }

	void setValue(float value) override { mValue = value; }
	const float value() const override { return mValue; }

private:
	float mValue;
};

