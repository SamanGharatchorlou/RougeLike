#pragma once

#include "Characters/Player/PlayerPropertyBag.h"

class StatManager
{
public:
	void init(PlayerPropertyBag* propertyBag);

	void gainExp(int exp);

private:
	PlayerPropertyBag* bag;
};