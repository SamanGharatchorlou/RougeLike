#include "pch.h"
#include "AbilityManager.h"

#include "AbilityClasses/Ability.h"

AbilityManager::AbilityManager() : mEnvironment(nullptr) { }



AbilityManager::~AbilityManager()
{
	clear();
}


void AbilityManager::init(Environment* environment, Actor* caster, Screen* screen)
{
	mActivator.init(environment);
	mBuilder.init(caster);
	mHotKeys.init(screen);
}

void AbilityManager::clear()
{
	mEnvironment = nullptr;
	mEvents.clear();
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
		AbilityType type = ability->type();
		Button::State buttonState = mHotKeys.state(ability, input);

		if (buttonState == Button::State::Pressed)
			int a = 4;


		if (mActivator.canSelect(ability) && mActivator.selected(ability, buttonState))
		{
			setState(ability, AbilityState::Selected);
		}

		if (mActivator.activate(ability, buttonState, input))
		{
			setState(ability, AbilityState::Activate);
		}

		if (ability->state() == AbilityState::Selected && mActivator.released(ability, buttonState))
		{
			setState(ability, AbilityState::Idle);
		}
	}
}


void AbilityManager::handleStates(Ability* ability, float dt)
{
	switch (ability->state())
	{
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
		ability->fastUpdate(dt);
		ability->slowUpdate(dt);

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
		setState(ability, AbilityState::Idle);
		break;
	}
	case AbilityState::Idle:
	case AbilityState::Selected:
	default:
		break;
	}
}


void AbilityManager::slowUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		handleStates(mAbilities[i], dt);
		handleEvents(mAbilities[i]);
	}
}


void AbilityManager::handleEvents(Ability* ability)
{
	while (ability->events().hasEvent())
	{
		EventPacket event = ability->events().pop();

		if (event.data->eventType == Event::ActivateAbilityOn)
		{
			mActivator.activateAreaAttack(ability);
			event.free();
		}
		else
		{
			mEvents.push(event);
		}

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
		AbilityType type = ability->type();

#if _DEBUG
		if (type == AbilityType::None)
			DebugPrint(Warning, "Ability '%s' has no type defined. Has its type() override function been defined?\n", name);
#endif


		mHotKeys.addHotKey(ability);
		setState(ability, AbilityState::Idle);

		mAbilities.push_back(ability);
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

