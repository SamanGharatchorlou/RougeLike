#pragma once

#include "Utilities/Maps/EffectMap.h"
#include "Animations/Animator.h"

class Texture;

enum class WeaponType
{
	None,
	Melee,
	Ranged,
	Magic
};


struct WeaponRawData
{
	BasicString type;
	Texture* texture = nullptr;
	StringMap properties;
	StringMap audio;
	PropertyMap effectData;
};


class WeaponData
{
public:
	WeaponData() : type(WeaponType::None), maxDimention(0.0f) { }
	virtual ~WeaponData() { }

	void fillData(const WeaponRawData& data);
	void copyBaseData(const WeaponData* data);
	virtual void copy(const WeaponData* data) = 0;

	const BasicString& audioHit() const { return audio.at("Hit"); }
	const BasicString& audioMiss() const { return audio.at("Miss"); }


private:
	void populateBaseProperties(const StringMap& properties);
	virtual void fillProperties(const StringMap& properties) = 0;


public:
	float maxDimention;

	WeaponType type;
	StringMap audio;
	PropertyMap effectData;
};



class MeleeWeaponData : public WeaponData
{
public:
	MeleeWeaponData() : texture(nullptr), swingAngle(0.0f), swingSpeed(0.0f) { type = WeaponType::Melee; }
	void copy(const WeaponData* data) override;

public:
	Texture* texture;
	VectorF offset;

	float swingAngle;
	float swingSpeed;


private:
	void fillProperties(const StringMap& properties) override;
};


class MagicWeaponData : public WeaponData
{
public:
	MagicWeaponData() { type = WeaponType::Magic; }
	void copy(const WeaponData* data) override;

private:
	void fillProperties(const StringMap& properties) override;

public:
	Animator animator;
	VectorF projectileColliderScale;
};