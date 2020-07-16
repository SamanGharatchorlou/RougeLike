#include "pch.h"
#include "AbilityManager.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"

#include "Animations/AnimationReader.h"

// TEMP
#include "AbilityCreator.h"


AbilityManager::AbilityManager(GameData* gameData, Actor* parent) 
	: mGameData(gameData), mActivator(this), mHotKeys(this), mCaster(parent)
{ }


void AbilityManager::handleInput()
{
	mHotKeys.handleInput(mGameData->inputManager);
}


void AbilityManager::handleStates(Ability* ability, float dt)
{
	switch (ability->state())
	{
	case Ability::Selected:
	{
		if (mActivator.shouldActivate(ability, mGameData->inputManager))
			setState(ability, Ability::Activate);

		break;
	}
	case Ability::Activate:
	{
		if (mActivator.activate(ability))
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

		if (ability->cooldown().completed())
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

	// Handle hot key events
	while (mHotKeys.events.hasEvent())
	{
		mEvents.push(mHotKeys.events.pop());
	}
}


void AbilityManager::handleEvents(Ability* ability)
{
	while (ability->hasEvent())
	{
		EventPacket event = ability->popEvent();

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
		if (mAbilities[i]->state() == Ability::Selected)
		{
			return true;
		}
	}

	return false;
}


void AbilityManager::add(const std::string& name)
{
	Ability* ability = createNewAbility(name, mGameData->textureManager);

	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, ability->name()));

	AnimationReader reader(mGameData->textureManager, parser);
	Animator animator;

	if (reader.initAnimator(animator))
	{
		ability->init(animator, mCaster);
		mHotKeys.addHotKey(ability);
	}
	else
		DebugPrint(Warning, "Animator setup failed for '%s' ability\n", ability->name().c_str());

	setState(ability, Ability::Idle);
	mAbilities.push_back(ability);
}


void AbilityManager::setState(Ability* ability, Ability::State state)
{
	if (state == Ability::Idle)
	{
		// None OR Finished --> Idle
		if (ability->state() == Ability::None || ability->state() == Ability::Finished)
		{
			ability->setState(state);
		}
	}
	else if (state == Ability::Selected)
	{
		// Idle --> Selected
		if (ability->state() == Ability::Idle)
		{
			sendSetTextColourEvent(ability, Colour::Green);
			ability->setState(state);
		}
	}
	else if (state == Ability::Activate)
	{
		// Idle OR Selected --> Activate
		if (ability->state() == Ability::Idle || ability->state() == Ability::Selected)
		{
			sendSetTextColourEvent(ability, Colour::Red);
			ability->setState(state);
		}
	}
	else if (state == Ability::Running)
	{
		if (ability->state() == Ability::Activate)
		{
			ability->setState(state);
		}
	}
	else if (state == Ability::Finished)
	{
		if (ability->state() == Ability::Running)
		{
			sendSetTextColourEvent(ability, Colour::White);
			ability->setState(state);
		}
	}
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
