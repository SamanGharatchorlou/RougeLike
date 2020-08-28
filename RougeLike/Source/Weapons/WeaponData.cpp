#include "pch.h"
#include "WeaponData.h"

/* 
ERROR
Why do i need this function???
Copying the contents of this function into the constructor for WeaponData causes a linker error
But calling it from another function does not...?

SOLUTION
Do not call pure virtual functions from constructors/destructors the object may not have been
created yet so cannot call its derived members version of the function.
*/
void WeaponData::fillData(const WeaponRawData& data)
{
	fillProperties(data.properties);
	effects = data.effects;
	audio = data.audio;
	texture = data.texture;
}


void WeaponData::copy(const WeaponData* data)
{
	texture = data->texture;
	cooldown = data->cooldown;
	maxDimention = data->maxDimention;
	offset = data->offset;
	audio = data->audio;
	effects = data->effects;
}


void MeleeWeaponData::copy(const WeaponData* data)
{
	WeaponData::copy(data);
	
	const MeleeWeaponData* meleeData = static_cast<const MeleeWeaponData*>(data);
	swingAngle = meleeData->swingAngle;
	swingSpeed = meleeData->swingSpeed;
}


void MeleeWeaponData::fillProperties(const StringMap& properties)
{
	// Cooldown
	cooldown = Cooldown(properties.getFloat("Cooldown"));

	// Size and offset	
	maxDimention = properties.getFloat("MaxSize");
	offset = properties.getVector("OffsetX", "OffsetY");

	swingSpeed = properties.getFloat("SwingSpeed");
	swingAngle = properties.getFloat("SwingAngle");
}

