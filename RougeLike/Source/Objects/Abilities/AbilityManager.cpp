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
	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		UIButton* button = mGameData->uiManager->findButton(iter->first);

		// Enter selection mode
		if (button && button->isReleased())
		{
			exitSelection();
			iter->second->setState(Ability::Selected);
		}
	}
}


void AbilityManager::completeSelection(std::unordered_map<std::string, Ability*>::iterator iter)
{
	iter->second->setState(Ability::Running);

	UIButton* button = mGameData->uiManager->findButton(iter->first);
	if (button)
		button->setActive(false);

}


void AbilityManager::exitSelection()
{
	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		iter->second->setState(Ability::Idle);

		UIButton* button = mGameData->uiManager->findButton(iter->first);
		if (button)
			button->setActive(false);
	}
}


void AbilityManager::slowUpdate(float dt)
{
	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		Ability* ability = iter->second;

		switch (ability->state())
		{
		case Ability::Selected:
		{
			attemptActivation(ability);
			break;
		}
		case Ability::Activating:
		{
			completeSelection(iter);
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
		while (iter->second->hasEvent())
		{
			mEvents.push(iter->second->popEvent());
		}
	}
}



void AbilityManager::render()
{
	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		iter->second->render();
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


void AbilityManager::activate(const std::string& ability)
{
	if (mAbilities.count(ability) > 0)
	{
		mAbilities[ability]->activate(mGameData->actors->playerActor());
		mAbilities[ability]->setState(Ability::Running);
	}
	else
	{
		DebugPrint(Log, "Play does not have the '%s' ability\n", ability.c_str());
	}
}


bool AbilityManager::inSelectionMode() const
{
	bool selectionMode = false;

	for (std::unordered_map<std::string, Ability*>::const_iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		if (iter->second->state() == Ability::Selected)
			selectionMode = true;
	}

	return selectionMode;
}

void AbilityManager::add(const std::string& name, Ability* ability)
{
	AnimationReader reader(mGameData->textureManager);
	Animator animator;

	if (reader.initAnimator(animator, name))
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
		DebugPrint(Warning, "Animator setup failed for '%s' ability\n", name.c_str());

	ability->setState(Ability::Idle);
	mAbilities[std::string(name)] = ability;
}


void AbilityManager::setState(const std::string& ability, Ability::State state)
{
	if(AbilityManager::hasAbility(ability))
	{
		mAbilities[ability]->setState(state);
	}
}