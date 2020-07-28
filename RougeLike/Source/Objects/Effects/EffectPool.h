#pragma once

#include "Utilities/ObjectPool.h"
#include "EffectTypes/EffectTypes.h"

class Effect;


class EffectPool : public ObjectPool<Effect, EffectType>
{
public:
	EffectPool() { }

	void load() override;

	void returnObject(Effect* effect) override;

private:
	Effect* createNewObject(EffectType type) const override;
};



/*

if (type == EffectType::Damage)
{
	effect = new DamageEffect;
}
else if (type == EffectType::Displacement)
{
	effect = new DisplacementEffect;
}
else if (type == EffectType::KnockbackStun)
{
	Effect* additionalEffect = getEffect(EffectType::Stun);
	StunEffect* stunEffect = static_cast<StunEffect*>(additionalEffect);

	effect = new KnockbackStunEffect(stunEffect);
}
else if (type == EffectType::Armor)
{
	effect = new ArmorEffect;
}
else if (type == EffectType::Blink)
{
	effect = new BlinkEffect;
}
else if (type == EffectType::Heal)
{
	effect = new HealEffect;
}
//else if (type == EffectType::Stun)
//{
//	XMLParser parser;
//	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, "Stun"));

//	AnimationReader reader(mGameData->textureManager, parser);
//	Animator animator;
//	reader.initAnimator(animator);

//	effect = new StunEffect(animator);
//}
*/