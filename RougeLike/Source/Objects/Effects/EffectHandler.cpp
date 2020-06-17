#include "pch.h"
#include "EffectHandler.h"
#include "EffectPool.h"

#include "Objects/Actors/Actor.h"
#include "Effect.h"


#include "DamageEffect.h"


void EffectHandler::addEffect(Effect* effect)
{
	if (!mDelayedAdd)
	{
		effect->setReceiver(mActor);
		printf("adding new effect %p with actor %p\n", effect, mActor);
		counter++;
		effect->init();
		mEffects.push_back(effect);
	}
	else
	{
		mEffectsToAdd.push(effect);
	}
}


//void EffectHandler::addEffect(const EffectData* data)
//w{
//	if (!mDelayedAdd)
//	{
//		Effect* effect = mPool->getEffect(data->type);
//
//		effect->setParentActor(mActor);
//		effect->fillData(data);
//		effect->init();
//
//		mEffects.push_back(effect);
//	}
//	else
//	{
//		//mEffectsToAdd.push(effect);
//	}
//}


void EffectHandler::fastUpdate(float dt)
{
	if (counter == 2)
		printf("pause\n");

	for (Effect* effect : mEffects)
	{
		effect->fastUpdate(dt);
	}
}


void EffectHandler::slowUpdate(float dt)
{
	mDelayedAdd = true;

	for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	{
		Effect* effect = *iter;

		effect->slowUpdate(dt);

		if (effect->shouldExit())
		{
			effect->exit();

			//delete effect;
			iter = mEffects.erase(iter);


			mPool->returnEffect(effect);
			printf("returning effect %p back to pool\n", effect);
			counter--;
			//printf("remove effect\n");
		}
		else
		{
			iter++;
		}
	}

	mDelayedAdd = false;

	// Effects added to mEffects during the for loop can invalidate the iterator
	while (mEffectsToAdd.size() > 0)
	{
		Effect* effect = mEffectsToAdd.front();
		mEffectsToAdd.pop();

		addEffect(effect);
	}
}

void EffectHandler::render()
{
	for (Effect* effect : mEffects)
	{
		effect->render();
	}
}


void EffectHandler::clear()
{
	for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	{
		Effect* effect = *iter;

		effect->exit();

		delete effect;
		iter = mEffects.erase(iter);
	}
}