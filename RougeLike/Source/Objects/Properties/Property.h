#pragma once

class Property
{
public:
	Property() { }
	virtual ~Property() { };
	
	virtual void setValue(float value) = 0;
	virtual const float value() const = 0;
};



class PropertyValue : public Property
{
public:
	PropertyValue(float value) : mValue(value) { }

	void setValue(float value) { mValue = value; }
	const float value() const { return mValue; }

private:
	float mValue;
};
