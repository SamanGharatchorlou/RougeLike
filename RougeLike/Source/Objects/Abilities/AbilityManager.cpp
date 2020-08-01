#include "pch.h"
#include "AbilityManager.h"

#include "Input/InputManager.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"

#include "Animations/AnimationReader.h"

// TEMP
#include "AbilityCreator.h"
#include "AbilityBuilder.h"


AbilityManager::AbilityManager(TextureManager* textures, Actor* caster, Screen* screen) :
	mBuilder(textures), mHotKeys(textures, screen)
{ }


void AbilityManager::init(Environment* environment)
{
	mActivator.init(environment);
}


void AbilityManager::handleInput(const InputManager* input)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		AbilityType type = ability->type();
		Button::State buttonState = mHotKeys.state(type, input);

		if (mActivator.canSelect(ability) && mActivator.selected(ability, buttonState))
		{
			setState(ability, Ability::Selected);
		}

		if (mActivator.activate(ability, buttonState, input))
		{
			setState(ability, Ability::Activate);
		}

		if (ability->state() == Ability::Selected && mActivator.released(ability, buttonState))
		{
			setState(ability, Ability::Idle);
		}
	}
}


void AbilityManager::handleStates(Ability* ability, float dt)
{
	switch (ability->state())
	{
	case Ability::Activate:
	{
		if (!ability->cooldown().hasStarted())
		{
			ability->cooldown().begin();
			setState(ability, Ability::Running);
		}
		break;
	}
	case Ability::Running:
	{
		ability->fastUpdate(dt);
		ability->slowUpdate(dt);

		if (ability->cooldown().hasCompleted())
		{
			setState(ability, Ability::Finished);
		}

		break;
	}
	case Ability::Finished:
	{
		ability->exit();
		setState(ability, Ability::Idle);
		break;
	}
	case Ability::Idle:
	case Ability::Selected:
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
	while (ability->hasEvent())
	{
		EventPacket event = ability->popEvent();

		if (event.data->eventType == Event::ActivateAbilityOn)
		{
			//mActivator.activateAreaAttack(ability);
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
		if (mAbilities[i]->state() == Ability::Selected)
		{
			return true;
		}
	}

	return false;
}


void AbilityManager::addAbility(const std::string& name, Actor* caster)
{
	AbilityType type = AbilityType::None;
	type << name;

	if (type != AbilityType::None)
	{
		Ability* ability = mBuilder.build(name);
		ability->setCaster(caster);

		AbilityType type = ability->type();
		setState(ability, Ability::Idle);

		mHotKeys.addHotKey(type);
		mAbilities.push_back(ability);
	}
	else
	{
		DebugPrint(Log, "No ability type with the name '%s'. No ability was added.\n", name.c_str());
	}
}


void AbilityManager::setState(Ability* ability, Ability::State state)
{
	ability->setState(state);

	if(state == Ability::Selected)
		sendSetTextColourEvent(ability, Colour::Green);

	else if (state == Ability::Running)
		sendSetTextColourEvent(ability, Colour::Red);

	else if (state == Ability::Idle)
		sendSetTextColourEvent(ability, Colour::White);
}


Ability* AbilityManager::get(const std::string& name) const
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		if (mAbilities[i]->name() == name)
		{
			return mAbilities[i];
		}
	}

	DebugPrint(Warning, "Ability '%s' not in ability manager\n", name.c_str());
	return nullptr;
}



// --- Private Functions --- //
void AbilityManager::sendSetTextColourEvent(Ability* ability, Colour colour)
{
	std::string id = ability->name() + "IconText";
	SetTextColourEvent* event = new SetTextColourEvent(id, SDLColour(colour));
	EventPacket eventPacket(event);
	mEvents.push(eventPacket);
}
