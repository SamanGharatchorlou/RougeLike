#include "pch.h"
#include "AbilityManager.h"

#include "Ability.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"

#include "UI/UIManager.h"
#include "UI/Elements/UIButton.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Actors/ActorManager.h"

#include "Collisions/Collider.h"
#include "Map/Environment.h"


void AbilityManager::handleInput()
{
	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		UIButton* button = mGameData->uiManager->findButton(iter->first);

		if (button && button->isActive() && !inSelectionMode())
		{
			mActiveAbility = iter->second;
			printf("ability %s selected\n", iter->first.c_str());
		}
	}
}


void AbilityManager::slowUpdate()
{
	if (inSelectionMode())
	{
		activateActiveAbility();
	}

	// in selection mode
	if (mActiveAbility && mActiveAbility->hasBeenActivated())
	{
		// get events from current active ability
		// do other things
		endSelectionMode();
	}
}


void AbilityManager::activateActiveAbility()
{
	switch (mActiveAbility->targetType())
	{
		// activate on first enemy selected
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
					activate(enemies[i]);
					break;
				}
			}
		}

		break;
	}
	case Ability::TargetType::Self:
	{
		activate(mGameData->actors->playerActor());
	}
	default:
		break;
	}
}


void AbilityManager::endSelectionMode()
{
	// Deactivate button
	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		if (iter->second == mActiveAbility)
		{
			UIButton* button = mGameData->uiManager->findButton(iter->first);

			if (button && button->isActive())
			{
				button->setActive(false);
			}
		}
	}

	mActiveAbility->setActive(false);
	mActiveAbility = nullptr;
}


void AbilityManager::add(std::string name, Ability* ability)
{
	mAbilities[name] = ability;
}


void AbilityManager::select(const std::string& ability)
{
	auto search = mAbilities.find(ability);

	if (search != mAbilities.end())
	{
		mActiveAbility = search->second;
	}
	else
	{
		DebugPrint(Warning, "Player does not have the '%s' ability\n", ability.c_str());
		mActiveAbility = nullptr;
	}
}


void AbilityManager::activate(Actor* target)
{
	mActiveAbility->activate(target);
}