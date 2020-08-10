#pragma once

#include "Objects/Abilities/Cooldown.h"
#include "Objects/Bags/EffectBag.h"

class Texture;


struct WeaponRawData
{
	Texture* texture = nullptr;
	StringMap properties;
	StringMap audio;
	EffectBag effects;
};


struct WeaponData
{
public:
	WeaponData() : texture(nullptr), maxDimention(0.0f) { }
	virtual ~WeaponData() { }

	void fillData(const WeaponRawData& data);
	virtual void copy(const WeaponData* data);

	const BasicString& audioHit() const { return audio.at("Hit"); }
	const BasicString& audioMiss() const { return audio.at("Miss"); }


public:
	Texture* texture;

	Cooldown cooldown;

	float maxDimention;
	VectorF offset;

	StringMap audio;
	EffectBag effects;


private:	
	virtual void fillProperties(const StringMap& properties) = 0;
	void fillEffects(EffectBag effectBag) { effects = effectBag; }
	void fillAudio(StringMap audioData) { audio = audioData; }

};



class MeleeWeaponData : public WeaponData
{
public:
	MeleeWeaponData() : swingAngle(0.0f), swingSpeed(0.0f) { }
	void copy(const WeaponData* data) override;

public:
	float swingAngle;
	float swingSpeed;


private:
	void fillProperties(const StringMap& properties) override;
};