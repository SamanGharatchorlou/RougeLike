#pragma once

#include "Objects/Properties/PropertyBag.h"

class EnemyPropertyBag : public PropertyBag
{
public:
	void readProperties(const std::string& config);

private:
	void fillProperties(ValueMap& valueMap);
};

