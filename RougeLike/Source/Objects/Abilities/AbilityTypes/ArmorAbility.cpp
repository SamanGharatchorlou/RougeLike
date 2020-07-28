#include "pch.h"
#include "ArmorAbility.h"

#include "Actors/Player/Player.h"
#include "Objects/Effects/EffectTypes/ArmorEffect.h"
#include "Objects/Effects/EffectPool.h"


//void ArmorAbility::fillValues(ValueMap& values)
//{
//	mArmor = Armor(values["Armor"]);
//}


void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mCaster->position());

	if (mAnimator.loops() > 4)
		mAnimator.stop();
}

void ArmorAbility::activate(EffectPool* pool)
{
	mAnimator.startAnimation(Action::Active);

	applyEffects(pool);
	updateUI();
}


void ArmorAbility::applyEffects(EffectPool* pool)
{
	//Effect* effect = pool->getObject(EffectType::Armor);
	//ArmorEffect* armorEffect = static_cast<ArmorEffect*>(effect);
	//armorEffect->set(mArmor);
	//mCaster->addEffect(effect);
}


void ArmorAbility::updateUI()
{
	Player* player = dynamic_cast<Player*>(mCaster);

	if (mCaster)
	{
		Armor* armor = static_cast<Armor*>(mCaster->getProperty("Armor"));
		SetArmorBarEvent* armorPtr = new SetArmorBarEvent(*armor);
		pushEvent(EventPacket(armorPtr));
	}
}