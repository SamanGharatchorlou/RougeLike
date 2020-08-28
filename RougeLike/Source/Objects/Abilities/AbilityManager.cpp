#include "pch.h"
#include "AbilityManager.h"

#include "AbilityClasses/Ability.h"
#include "AbilityClasses/RangedAbility.h"


AbilityManager::AbilityManager() : mEnvironment(nullptr) { }
AbilityManager::~AbilityManager() { clear(); }


void AbilityManager::init(Environment* environment, Actor* caster, Screen* screen)
{
	mActivator.init(environment);
	mBuilder.init(caster);
	mHotKeys.init(screen);
}

void AbilityManager::clear()
{
	mEnvironment = nullptr;
	mActivator.clear();
	mHotKeys.clear();

	for (int i = 0; i < mAbilities.size(); i++)
	{
		delete mAbilities[i];
		mAbilities[i] = nullptr;
	}

	mAbilities.clear();
}


void AbilityManager::handleInput(const InputManager* input)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		Button::State buttonState = mHotKeys.state(ability, input);

		if (mActivator.selected(ability, buttonState))
		{
			setState(ability, AbilityState::Selected);
		}

		if (mActivator.activate(ability, buttonState, input))
		{
			setState(ability, AbilityState::Activate);
		}

		if (mActivator.deselected(ability, buttonState))
		{
			setState(ability, AbilityState::Idle);
		}
	}
}


void AbilityManager::handleState(Ability* ability, float dt)
{
	switch (ability->state())
	{
	case AbilityState::Selected:
	{
		if (ability->targetType() == AbilityTarget::Ranged)
		{
			RangedAbility* rangedAbility = static_cast<RangedAbility*>(ability);
			mEvents.push(rangedAbility->renderRangeCircleEvent());
		}
		break;
	}
	case AbilityState::Activate:
	{
		if (!ability->cooldown().hasStarted())
		{
			ability->cooldown().begin();
			setState(ability, AbilityState::Running);
		}
		break;
	}
	case AbilityState::Running:
	{
		if (ability->hasCompleted())
		{
			ability->cooldown().begin();
			setState(ability, AbilityState::Cooldown);

		}
		break;
	}
	case AbilityState::Cooldown:
	{
		if (ability->cooldown().hasCompleted())
		{
			setState(ability, AbilityState::Finished);
		}
		break;
	}
	case AbilityState::Finished:
	{
		ability->exit();
		ability->baseExit();
		setState(ability, AbilityState::Idle);
		break;
	}
	case AbilityState::Idle:
	default:
		break;
	}
}


void AbilityManager::fastUpdate(float dt)
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


void AbilityManager::slowUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		handleState(ability, dt);

		if (ability->state() == AbilityState::Running)
			ability->slowUpdate(dt);
	}
}


void AbilityManager::render()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		mAbilities[i]->render();
	}
}


bool AbilityManager::inSelectionMode() const
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		if (mAbilities[i]->state() == AbilityState::Selected)
		{
			return true;
		}
	}

	return false;
}


void AbilityManager::addAbility(AbilityType abilityType)
{
	BasicString name;
	abilityType >> name;

	if (abilityType != AbilityType::None)
	{
		Ability* ability = mBuilder.build(abilityType);
		if (ability)
		{
#if _DEBUG
			if (ability->type() == AbilityType::None)
				DebugPrint(Warning, "Ability '%s' has no type defined. Has its type() override function been defined?\n", name);
#endif

			mHotKeys.addHotKey(ability);
			setState(ability, AbilityState::Idle);
			mAbilities.push_back(ability);
		}
	}
	else
	{
		DebugPrint(Log, "No ability type with the name '%s'. No ability was added.\n", name.c_str());
	}
}


void AbilityManager::setState(Ability* ability, AbilityState state)
{
	ability->setState(state);
	mHotKeys.updateStates();
}

