#include "pch.h"
#include "AbilityManager.h"

#include "AbilityClasses/Ability.h"

AbilityManager::AbilityManager(TextureManager* textures, Actor* caster, Screen* screen) :
	mBuilder(textures, caster), mHotKeys(textures, screen)
{ }



AbilityManager::~AbilityManager()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		delete ability;
	}
}


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


void AbilityManager::addAbility(const BasicString& name)
{
	AbilityType type = AbilityType::None;
	type << name;

	if (type != AbilityType::None)
	{
		Ability* ability = mBuilder.build(name);
		AbilityType type = ability->type();

#if _DEBUG
		if (type == AbilityType::None)
			DebugPrint(Warning, "Ability '%s' has no type defined. Has its type() override function been defined?\n", name);
#endif

		setState(ability, AbilityState::Idle);

		mHotKeys.addHotKey(type);
		mAbilities.push_back(ability);
		ability->enter();
	}
	else
	{
		DebugPrint(Log, "No ability type with the name '%s'. No ability was added.\n", name.c_str());
	}
}


void AbilityManager::setState(Ability* ability, AbilityState state)
{
	ability->setState(state);

	if(state == AbilityState::Selected)
		sendSetTextColourEvent(ability, Colour::Green);

	else if (state == AbilityState::Running)
		sendSetTextColourEvent(ability, Colour::Red);

	else if (state == AbilityState::Idle)
		sendSetTextColourEvent(ability, Colour::White);
}


Ability* AbilityManager::get(const BasicString& name) const
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
	BasicString id = ability->name() + "IconText";
	SetTextColourEvent* event = new SetTextColourEvent(id, SDLColour(colour));
	EventPacket eventPacket(event);
	mEvents.push(eventPacket);
}
