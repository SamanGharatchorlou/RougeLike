#include "pch.h"
#include "ArmorEffect.h"

#include "Actors/Actor.h"


void ArmorEffect::init()
{
	Armor* armor = static_cast<Armor*>(mReceiver->getAttribute(PropertyType::Armor));
	armor->increase(mArmor);
	endEffect();
}


void ArmorEffect::fill(const PropertyMap& valueBag)
{
	setProperty(PropertyType::Armor, mArmor, valueBag);
}


void ArmorEffect::clearData()
{
	clearBaseData();
	mArmor = Armor(0.0f);
}