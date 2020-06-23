#include "pch.h"
#include "AbilityManager.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "UI/UIManager.h"
#include "UI/Elements/UIButton.h"

#include "Objects/Actors/Player/Player.h"
#include "Objects/Actors/Actor.h"
#include "Objects/Actors/ActorManager.h"

#include "Collisions/Collider.h"
#include "Map/Environment.h"
#include "Map/Map.h"

#include "Animations/AnimationReader.h"

// TEMP
#include "AbilityActivator.h"


AbilityManager::AbilityManager(GameData* gameData, Actor* parent) 
	: mGameData(gameData), mHotKeys(gameData, this), mCaster(parent)
{ }


void AbilityManager::handleInput()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		UIButton* button = mGameData->uiManager->findButton(mAbilities[i]->name());

		// Enter selection mode
		if (button && button->isReleased())
		{
			exitSelection();
			mAbilities[i]->setState(Ability::Selected);
		}
	}

	mHotKeys.handleInput();
}


void AbilityManager::exitSelection()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		exitSelection(mAbilities[i]);
	}
}


void AbilityManager::exitSelection(Ability* ability)
{
	//if (ability->state() == Ability::Selected)
	//	setState(ability, Ability::Idle);

	UIButton* button = mGameData->uiManager->findButton(ability->name());
	if (button)
		button->setActive(false);
}


void AbilityManager::handleStates(Ability* ability, float dt)
{
	switch (ability->state())
	{
		// Ready for/attemping activation
	case Ability::Selected:
	{
		//attemptActivation(ability);
		AbilityActivator(this, ability);
		break;
	}
	// One frame, on activation
	case Ability::Activating:
	{
		completeSelection(ability);
		break;
	}
	// Ability doing its thing
	case Ability::Running:
	{
		ability->fastUpdate(dt);
		ability->slowUpdate(dt);
		break;
	}
	// Reset... cool down etc.
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
		Ability* ability = mAbilities[i];

		handleStates(ability, dt);

		// Either process event or pass to parent object to be handled 
		handleEvents(ability);
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

		if (event.data->eventType == Event::ActivateAreaAttack)
		{
			// TODO: can i just replace this with the activator?
			AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);
			activateOnArea(areaAbility);
			//AbilityActivator(areaAbility);
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


void AbilityManager::add(Ability* ability)
{
	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, ability->name()));

	AnimationReader reader(mGameData->textureManager, parser);
	Animator animator;

	if (reader.initAnimator(animator))
	{
		ability->init(animator, mCaster);
		
		if (ability->targetType() >= Ability::Area)
		{
			AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);

			Texture* rangeCircle = mGameData->textureManager->getTexture("RangeCircle", FileManager::Image_UI);
			areaAbility->setRangeCircle(rangeCircle);
		}

		mHotKeys.addHotKey(ability);
	}
	else
		DebugPrint(Warning, "Animator setup failed for '%s' ability\n", ability->name().c_str());

	setState(ability, Ability::Idle);
	mAbilities.push_back(ability);
}


void AbilityManager::setState(Ability* ability, Ability::State state)
{
	if (state == Ability::Selected)
	{
		if (ability->state() == Ability::Idle)
			sendSetTextColourEvent(ability, Colour::Green);
		else
			return;
	}
	else if (state == Ability::Activating)
	{
		sendSetTextColourEvent(ability, Colour::Red);
	}
	else if (state == Ability::Idle)
	{
		sendSetTextColourEvent(ability, Colour::White);
	}

	ability->setState(state);
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
void AbilityManager::completeSelection(Ability* ability)
{
	setState(ability, Ability::Running);

	UIButton* button = mGameData->uiManager->findButton(ability->name());
	if (button)
		button->setActive(false);
}

void AbilityManager::sendSetTextColourEvent(Ability* ability, Colour colour)
{
	std::string id = ability->name() + "IconText";
	SetTextColourEvent* event = new SetTextColourEvent(id, SDLColour(colour));
	EventPacket eventPacket(event);
	mEvents.push(eventPacket);
}
