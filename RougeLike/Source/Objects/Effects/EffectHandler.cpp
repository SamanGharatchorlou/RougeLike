#include "pch.h"
#include "EffectHandler.h"

#include "Effect.h"



void EffectHandler::addEffect(Effect* effect)
{
	effect->init();

	mEffects.push_back(effect);
}


void EffectHandler::fastUpdate(float dt)
{
	for (Effect* effect : mEffects)
	{
		effect->fastUpdate(dt);
	}
}

void EffectHandler::slowUpdate(float dt)
{
	for (std::vector<Effect*>::iterator iter = mEffects.begin(); iter != mEffects.end();)
	{
		Effect* effect = *iter;

		effect->slowUpdate(dt);

		if (effect->shouldExit())
		{
			effect->exit();

			delete effect;
			printf("effect object delted\n");
			iter = mEffects.erase(iter);

		}
		else
		{
			iter++;
		}
	}
}