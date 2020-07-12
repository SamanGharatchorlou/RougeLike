#include "pch.h"
#include "ArmorEffect.h"

#include "Objects/Actors/Actor.h"


void ArmorEffect::init()
{
	Armor* armor = static_cast<Armor*>(mReceiver->getProperty("Armor"));
	armor->increase(mArmor);
	endEffect();
}


void ArmorEffect::clearData()
{
	clearBaseData();
	mArmor = Armor(0.0f);
}