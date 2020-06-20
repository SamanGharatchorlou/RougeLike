#include "pch.h"
#include "ArmorEffect.h"

#include "Objects/Actors/Actor.h"


void ArmorEffect::init()
{
	Armor* armor = static_cast<Armor*>(mReceiver->getProperty("Armor"));
	armor->increase(mArmor);
	endEffect();
}


void ArmorEffect::fillData(const EffectPropertyBag* properties)
{
	if (properties->contains("Armor"))
	{
		Property* property = properties->get("Armor");
		mArmor = *static_cast<Armor*>(property);
	}
}

void ArmorEffect::clearData()
{
	clearBaseData();
	mArmor = Armor(0.0f);
}