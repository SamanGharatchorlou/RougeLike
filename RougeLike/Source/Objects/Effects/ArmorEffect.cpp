#include "pch.h"
#include "ArmorEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/Property.h"
#include "Objects/Attributes/Armor.h"


void ArmorEffect::init()
{
	Armor* armor = static_cast<Armor*>(mReceiver->getProperty("Armor"));
	armor->increase(mArmor);
	endEffect();
}
