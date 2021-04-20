#include "pch.h"
#include "WeaponData.h"

#include "Graphics/TextureManager.h"
#include "Animations/AnimationReader.h"

/* 
ERROR
Why do i need this function???
Copying the contents of this function into the constructor for WeaponData causes a linker error
But calling it from another function does not...?

SOLUTION
Do not call pure virtual functions from constructors/destructors the object may not have been
created yet so cannot call its derived members version of the function.
*/

// --- Base Weapon Data --- //
void WeaponData::fillData(const WeaponRawData& data)
{
	populateBaseProperties(data.properties);
	fillProperties(data.properties);
	effectData = data.effectData;
	audio = data.audio;
}


void WeaponData::copyBaseData(const WeaponData* data)
{
	maxDimention = data->maxDimention;
	audio = data->audio;
	effectData = data->effectData;
}


// --- Melee Weapon Data --- //
void MeleeWeaponData::copy(const WeaponData* data)
{
	copyBaseData(data);
	
	const MeleeWeaponData* meleeData = static_cast<const MeleeWeaponData*>(data);
	texture = meleeData->texture;
	offset = meleeData->offset;

	swingAngle = meleeData->swingAngle;
	swingSpeed = meleeData->swingSpeed;
}


void WeaponData::populateBaseProperties(const StringMap& properties)
{
	maxDimention = properties.getFloat("MaxSize");
}


void MeleeWeaponData::fillProperties(const StringMap& properties)
{
	texture = TextureManager::Get()->getTexture(properties.at("Texture"), FileManager::Image_Weapons);
	offset = properties.getVector("OffsetX", "OffsetY");

	swingSpeed = properties.getFloat("SwingSpeed");
	swingAngle = properties.getFloat("SwingAngle");
}


// --- Magic Weapon Data --- //
void MagicWeaponData::copy(const WeaponData* data)
{
	copyBaseData(data);

	const MagicWeaponData* magicData = static_cast<const MagicWeaponData*>(data);
	animator = magicData->animator;
}


void MagicWeaponData::fillProperties(const StringMap& properties)
{
	XMLParser animationParser(FileManager::Get()->findFile(FileManager::Config_Abilities, properties.at("Animation")));
	XMLNode animationNode = animationParser.rootChild("Animator");

	AnimationReader reader;
	animator = reader.buildAnimator(animationNode);
}

