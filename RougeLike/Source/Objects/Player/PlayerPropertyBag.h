#pragma once

#include "Objects/Attributes/Property.h"
#include "Objects/Properties/PropertyBag.h"


class PlayerPropertyBag : public PropertyBag
{
public:
	PlayerPropertyBag() { }
	void readAttributes(const std::string& name);

	void resetAttributes();

	void fillProperties(ValueMap& valueMap);

public:
	WeaponProperty pWeapon;

private:
	std::string attributeName;
};


