#pragma once

#include "Objects/Properties/PropertyBag.h"


class PlayerPropertyBag : public PropertyBag
{
public:
	void readProperties(XMLParser& parser);

private:
	void fillProperties(ValueMap& valueMap);
};


