#pragma once

#include "Objects/Properties/PropertyBag.h"


class PlayerPropertyBag : public PropertyBag
{

private:
	void fillProperties(ValueMap& valueMap);
};


