#include "pch.h"
#include "AbilityManager.h"

#include "Ability.h"

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

// Abilities
#include "Objects/Abilities/SlowAbility.h"
#include "Objects/Abilities/HealAbility.h"
#include "Objects/Abilities/SpikeAbility.h"
#include "Objects/Abilities/BilnkAbility.h"
#include "Objects/Abilities/ArmorAbility.h"
#include "Objects/Abilities/SmashAbility.h"


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
}


void AbilityManager::exitSelection()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		// TODO: do i need some kind of exit mechanism?
		//mAbilities[i]->setState(Ability::Idle);
		if (mAbilities[i]->state() == Ability::Selected)
			mAbilities[i]->setState(Ability::Idle);

		UIButton* button = mGameData->uiManager->findButton(mAbilities[i]->name());
		if (button)
			button->setActive(false);
	}
}


void AbilityManager::slowUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];

		switch (ability->state())
		{
		// Ready for/attemping activation
		case Ability::Selected:
		{
			attemptActivation(ability);
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
			ability->slowUpdate(dt);
			break;
		}
		// Reset... cool down etc.
		case Ability::Finished:
		{
			ability->exit();
			ability->setState(Ability::Idle);
			break;
		}
		default:
			break;
		}

		// Either process event or pass to parent object to be handled 
		handleEvents(ability);
	}
}

void AbilityManager::handleEvents(Ability* ability)
{
	while (ability->hasEvent())
	{
		EventPacket event = ability->popEvent();

		if (event.data->eventType == Event::ActivateAreaAttack)
		{
			activateOnArea(ability);
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
		ability->init(animator, mGameData->actors->player());
		
		if (ability->targetType() >= Ability::Area)
		{
			AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);

			Texture* rangeCircle = mGameData->textureManager->getTexture("RangeCircle", FileManager::Image_UI);
			areaAbility->setRangeCircle(rangeCircle);
		}

	}
	else
		DebugPrint(Warning, "Animator setup failed for '%s' ability\n", ability->name().c_str());

	ability->setState(Ability::Idle);
	mAbilities.push_back(ability);
}


Ability* AbilityManager::createNewAbility(const std::string& name)
{
	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, name));

	xmlNode propertyNode = parser.rootNode()->first_node("Properties");
	ValueMap values = parser.values(propertyNode);

	Ability* ability = nullptr;

	if (name == "Slow")
	{
		ability = new SlowAbility;
	}
	else if (name == "Heal")
	{
		ability = new HealAbility;
	}
	else if (name == "Spikes")
	{
		ability = new SpikeAbility;
	}
	else if (name == "Blink")
	{
		ability = new BlinkAbility;
	}
	else if (name == "Armor")
	{
		ability = new ArmorAbility;
	}
	else if (name == "Smash")
	{
		// Build stun animator
		XMLParser parser;
		parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, values["Stun"]));

		AnimationReader reader(mGameData->textureManager, parser);
		Animator animator;
		reader.initAnimator(animator);

		Texture* texture = mGameData->textureManager->getTexture(values["HammerTexture"], FileManager::Image_Weapons);
		VectorF size = realiseSize(texture->originalDimentions, std::stof(values["HammerMaxSize"]));

		ability = new SmashAbility(texture, size, animator);
	}

	if (ability)
	{
		ability->fillValues(values);
		ability->setName(name);
	}
	else
		DebugPrint(Warning, "No new ability could be created wth the name '%s'\n", name.c_str());

	return ability;
}


void AbilityManager::setState(const std::string& name, Ability::State state)
{
	Ability* ability = get(name);

	if(ability)
	{
		ability->setState(state);
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
void AbilityManager::attemptActivation(Ability* ability)
{
	if (ability->state() != Ability::Selected)
		return;

	switch (ability->targetType())
	{
		// Player casts on self only
	case Ability::TargetType::Self:
	{
		attemptActivationOnSelf(ability);
		break;
	}
	// Activate on first enemy selected
	case Ability::TargetType::Single_Enemy:
	{
		attemptActivationOnSingleEnemy(ability);
		break;
	}
	// Select any floor tile
	case Ability::TargetType::Area_Attack:
	{
		attemptActivationOnArea(ability);
		break;
	}
	case Ability::TargetType::Area_Point:
	{
		attemptActivationOnPoint(ability);
		break;
	}
	default:
		break;
	}
}


void AbilityManager::completeSelection(Ability* ability)
{
	ability->setState(Ability::Running);

	UIButton* button = mGameData->uiManager->findButton(ability->name());
	if (button)
		button->setActive(false);

}


void AbilityManager::attemptActivationOnSelf(Ability* ability)
{
	ability->setState(Ability::Activating);
	ability->activate(nullptr);
}

void AbilityManager::attemptActivationOnSingleEnemy(Ability* ability)
{
	if (mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		VectorF cursorPosition = mGameData->inputManager->cursorPosition();
		cursorPosition = mGameData->environment->toWorldCoords(cursorPosition);

		std::vector<Actor*> enemies = mGameData->actors->getAllEnemies();
		for (int i = 0; i < enemies.size(); i++)
		{
			// activate ability on first enemy selected
			if (enemies[i]->collider()->contains(cursorPosition))
			{
				ability->setState(Ability::Activating);
				ability->activate(enemies[i]);
				break;
			}
		}
	}
}

void AbilityManager::attemptActivationOnArea(Ability* ability)
{
	if (mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		VectorF cursorPosition = mGameData->inputManager->cursorPosition();
		cursorPosition = mGameData->environment->toWorldCoords(cursorPosition);

		Map* map = mGameData->environment->map(cursorPosition);

		// Activate ability
		AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);

		// BUG: if you select just outside of the current map this breaks?
		if (areaAbility->isValidTarget(cursorPosition, map))
		{
			areaAbility->activate(cursorPosition);
			activateOnArea(ability);
		}
	}
}

void AbilityManager::activateOnArea(Ability* ability)
{
	ability->setState(Ability::Activating);

	AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);
	Collider abilityCollider = areaAbility->collider();

	// Apply effect to all enemies caught in area
	std::vector<Actor*> enemies = mGameData->actors->getAllEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		Collider* enemyCollider = enemies[i]->collider();
		if (enemyCollider->doesIntersect(&abilityCollider))
		{
			ability->activate(enemies[i]);
		}
	}
}



void AbilityManager::attemptActivationOnPoint(Ability* ability)
{
	if (mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		VectorF cursorPosition = mGameData->inputManager->cursorPosition();
		cursorPosition = mGameData->environment->toWorldCoords(cursorPosition);

		Map* map = mGameData->environment->map(cursorPosition);

		// Activate ability
		AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);
		if (areaAbility->isValidTarget(cursorPosition, map))
		{
			// Apply effect
			areaAbility->activate(cursorPosition);
			ability->activate(nullptr);
			ability->setState(Ability::Activating);
		}
	}
}
