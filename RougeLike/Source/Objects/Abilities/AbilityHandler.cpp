#include "pch.h"
#include "AbilityHandler.h"

#include "AbilityActivator.h"
#include "AbilityClasses/Ability.h"



void AbilityHandler::init(EffectPool* effectPool, std::vector<Actor*>* targets)
{
	mActivator.init(effectPool, targets);
}


void AbilityHandler::clear()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		delete mAbilities[i];
		mAbilities[i] = nullptr;
	}

	mAbilities.clear();
}

void AbilityHandler::fastUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		if (ability->state() == AbilityState::Running)
		{
			ability->fastUpdate(dt);

			if (ability->shouldActivateCollisions())
			{
				mActivator.activateCollisions(ability);
			}
		}
	}
}


void AbilityHandler::slowUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		handleState(ability, dt);

		if (ability->state() == AbilityState::Running)
			ability->slowUpdate(dt);
	}
}


void AbilityHandler::render()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		mAbilities[i]->render();
	}
}


void AbilityHandler::handleState(Ability* ability, float dt)
{
	switch (ability->state())
	{
	case AbilityState::Running:
	{
		if (ability->hasCompleted())
		{
			ability->cooldown().begin();
			ability->setState(AbilityState::Cooldown);

		}
		break;
	}
	case AbilityState::Cooldown:
	{
		if (ability->cooldown().hasCompleted())
		{
			ability->setState(AbilityState::Finished);
		}
		break;
	}
	case AbilityState::Finished:
	{
		ability->exit();
		ability->baseExit();
		ability->setState(AbilityState::Idle);
		break;
	}
	case AbilityState::Selected:
	case AbilityState::Idle:
	default:
		break;
	}
}


Ability* AbilityHandler::get(AbilityType type)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		if (mAbilities[i]->type() == type)
		{
			return mAbilities[i];
		}
	}

	BasicString abilityName;
	type >> abilityName;
	DebugPrint(Warning, "Caster has no '%s' ability\n", abilityName.c_str());
	return nullptr;
}


void AbilityHandler::add(Ability* ability)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		if (ability->type() == mAbilities[i]->type())
		{
			DebugPrint(Warning, "Ability handler already has a '%s' ability", ability->name());
			return;
		}
	}

	mAbilities.push_back(ability);
}