#include "pch.h"
#include "AbilityManager.h"

#include "Ability.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
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


void AbilityManager::completeSelection(Ability* ability)
{
	ability->setState(Ability::Running);

	UIButton* button = mGameData->uiManager->findButton(ability->name());
	if (button)
		button->setActive(false);

}


void AbilityManager::exitSelection()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
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
		case Ability::Selected:
		{
			attemptActivation(ability);
			break;
		}
		case Ability::Activating:
		{
			completeSelection(ability);

			break;
		}
		case Ability::Running:
		{
			ability->slowUpdate(dt);
			break;
		}
		case Ability::Finished:
		{
			// reset... cool down etc.
			ability->setState(Ability::Idle);
			break;
		}
		default:
			break;
		}

		// Pass events to parent object to be handled 
		while (ability->hasEvent())
		{
			mEvents.push(ability->popEvent());
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



void AbilityManager::attemptActivation(Ability* ability)
{
	switch (ability->targetType())
	{	
	// Player casts on self only
	case Ability::TargetType::Self:
	{
		ability->setState(Ability::Activating);
		ability->activate(nullptr);
		break;
	}
	// Activate on first enemy selected
	case Ability::TargetType::Single_Enemy:
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

		break;
	}
	// Select any floor tile
	case Ability::TargetType::Area_Attack:
	{
		if (mGameData->inputManager->isCursorPressed(Cursor::Left))
		{
			VectorF cursorPosition = mGameData->inputManager->cursorPosition();
			cursorPosition = mGameData->environment->toWorldCoords(cursorPosition);

			Map* map = mGameData->environment->map(cursorPosition);

			// Activate ability
			AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);

			// BUG: if you select just outside of the current map this breaks?
			if(areaAbility->isValidTarget(cursorPosition, map))
			{
				areaAbility->activate(cursorPosition);
				ability->setState(Ability::Activating);

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
		}
		break;
	}
	case Ability::TargetType::Area_Point:
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
		break;
	}
	default:
		break;
	}
}


void AbilityManager::activate(const std::string& name)
{
	if (hasAbility(name))
	{
		Ability* ability = get(name);
		ability->activate(mGameData->actors->playerActor());
		ability->setState(Ability::Running);
	}
	else
	{
		DebugPrint(Log, "Play does not have the '%s' ability\n", name.c_str());
	}
}

bool AbilityManager::canActivate(const std::string& name)
{
	return true;
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


void AbilityManager::setState(const std::string& name, Ability::State state)
{
	if(hasAbility(name))
	{
		get(name)->setState(state);
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


bool AbilityManager::hasAbility(const std::string& ability) const
{
	return get(ability) != nullptr;
}