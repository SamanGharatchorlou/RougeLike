#include "pch.h"
#include "AbilityManager.h"

#include "Input/InputManager.h"
#include "AbilityClasses/Ability.h"
#include "AbilityClasses/RangedAbility.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"


AbilityManager::AbilityManager() { }
AbilityManager::~AbilityManager() { clear(); }


void AbilityManager::init(Actor* caster, Screen* screen, EffectPool* effectPool, std::vector<Actor*>* targets)
{
	mCaster = caster;

	mHotKeys.init(screen);
	mHandler.init(effectPool, targets);
	
	Ability* ability = mBuilder.build(AbilityType::Attack, mCaster);			
	ability->setState(AbilityState::Idle);
	mHandler.add(ability);
}

void AbilityManager::clear()
{
	mHandler.clear();
	mHotKeys.clear();
}


void AbilityManager::handleInput(const InputManager* input)
{
	const std::vector<Ability*>& abilities = mHandler.abilities();
	const AbilityActivator& activator = mHandler.activator();

	for (int i = 1; i < abilities.size(); i++)
	{
		Ability* ability = abilities[i];
		HotKey* hotKey = mHotKeys.hotKey(ability);

		if (hotKey)
		{
			Button::State buttonState = input->state(hotKey->mKey);

			if (ability->state() == AbilityState::Idle && buttonState == Button::State::Pressed)
			{
				ability->setState(AbilityState::Selected);
			}

			if (ability->state() == AbilityState::Selected)
			{			
				if (activator.activate(ability, buttonState, input))
				{
					ability->setState(AbilityState::Running);
				}
			}

			if (ability->state() == AbilityState::Selected && buttonState == Button::State::Released)
			{
				ability->setState(AbilityState::Idle);
			}
		}
	}

	if (input->isCursorPressed(Cursor::Left))
	{
		Ability* basicAttack = mHandler.get(AbilityType::Attack);
		if (basicAttack->state() == AbilityState::Idle && !inSelectionMode())
		{
			basicAttack->activate(VectorF());
			basicAttack->setState(AbilityState::Running);
		}
	}
}


void AbilityManager::fastUpdate(float dt)
{
	mHandler.fastUpdate(dt);
}


void AbilityManager::slowUpdate(float dt)
{
	mHandler.slowUpdate(dt);

	const std::vector<Ability*>& abilities = mHandler.abilities();
	for (int i = 0; i < abilities.size(); i++)
	{
		Ability* ability = abilities[i];

		if (ability->state() == AbilityState::Selected &&
			ability->targetType() == AbilityTarget::Ranged)
		{			
			RangedAbility* rangedAbility = static_cast<RangedAbility*>(ability);
			mEvents.push(rangedAbility->renderRangeCircleEvent());
		}
	}

	mHotKeys.updateStates();
}


void AbilityManager::render()
{
	mHandler.render();
}


bool AbilityManager::inSelectionMode() const
{
	const std::vector<Ability*>& abilities = mHandler.abilities();
	for (int i = 0; i < abilities.size(); i++)
	{
		if (abilities[i]->state() == AbilityState::Selected)
		{
			return true;
		}
	}

	return false;
}



Ability* AbilityManager::get(AbilityType type)
{
	return mHandler.get(type);
}

void AbilityManager::addAbility(AbilityType abilityType)
{
	if (abilityType != AbilityType::None)
	{
		Ability* ability = mBuilder.build(abilityType, mCaster);
		if (ability)
		{
#if _DEBUG
			if (ability->type() == AbilityType::None)
			{
				BasicString name;
				abilityType >> name;
				DebugPrint(Warning, "Ability '%s' has no type defined. Has its type() override function been defined?\n", name);
			}
#endif

			mHotKeys.addHotKey(ability);
			ability->setState(AbilityState::Idle);

			mHandler.add(ability);
		}
	}
	else
	{
		BasicString name;
		abilityType >> name;
		DebugPrint(Log, "No ability type with the name '%s'. No ability was added.\n", name.c_str());
	}
}

