#pragma once

#include "Objects/Actors/Player/PlayerPropertyBag.h"

class StatManager
{
public:
	void init(PlayerPropertyBag* propertyBag);

	void gainExp(int exp);

private:
	PlayerPropertyBag* bag;
};