#include "pch.h"
#include "AbilityManager.h"

#include "Input/InputManager.h"
#include "Input/Button.h"
#include "AbilityClasses/Ability.h"
#include "AbilityClasses/RangedAbility.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"

#include "Audio/AudioManager.h"
#include "Graphics/TextureManager.h"


AbilityManager::AbilityManager() { }
AbilityManager::~AbilityManager() { clear(); }


void AbilityManager::init(Actor* caster, Screen* screen, EffectPool* effectPool, std::vector<Actor*>* targets)
{
	mCaster = caster;
	mHotKeys.init(screen);
	mHandler.init(effectPool, targets);
}

void AbilityManager::clear()
{
	mHandler.clear();
	mHotKeys.clear();
}


void AbilityManager::handleInput(const InputManager* input)
{
	const std::vector<Ability*>& abilities = mHandler.abilities();

	for (int i = 1; i < abilities.size(); i++)
	{
		Ability* ability = abilities[i];
		HotKey* hotKey = mHotKeys.hotKey(ability);

		if (hotKey)
		{
			const Button& button = input->getButton(hotKey->mKey);

			if (button.isPressed())
			{
				if (ability->state() == AbilityState::Idle)
				{
					ability->setState(AbilityState::Selected);
				}
				else
				{
					AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "InvalidAbility", mCaster));
				}
			}

			if ( ability->state() == AbilityState::Selected && (input->isCursorReleased(Cursor::Left) || !button.isHeld()) )
			{
				ability->setState(AbilityState::Activate);
			}
		}
	}

	mHandler.handleInput(input);

	Cursor::Mode mode = input->getCursor()->mode();
	if ((inSelectionMode() && mode == Cursor::Game_Red) || (!inSelectionMode() && mode == Cursor::Game_Green))
	{
		mUpdateCursor = true;
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
	if (mUpdateCursor)
	{
		updateCursor();
		mUpdateCursor = false;
	}
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
#if DEBUG_CHECK
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
		DebugPrint(Warning, "No ability type with the name '%s'. No ability was added.\n", name.c_str());
	}
}

// -- Private Functions --

void AbilityManager::updateCursor()
{
	TextureManager* texture = TextureManager::Get();
	if (inSelectionMode())
	{
		mHotKeys.setCursorTexture(texture->getTexture("GameCursorGreen", FileManager::Image_UI));
	}
	else
	{
		mHotKeys.setCursorTexture(texture->getTexture("GameCursor", FileManager::Image_UI));
	}
}