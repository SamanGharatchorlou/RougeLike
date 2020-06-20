#pragma once

class Property
{
public:
	Property() { }
	virtual ~Property() { };
	
	virtual void init(float value) { setValue(value); }
	virtual void setValue(float value) = 0;
	virtual const float value() const = 0;
};


class PropertyValue : public Property
{
public:
	PropertyValue() : mValue(0.0f) { }
	PropertyValue(float value) : mValue(value) { }

	void setValue(float value) override { mValue = value; }
	const float value() const override { return mValue; }

private:
	float mValue;
};

