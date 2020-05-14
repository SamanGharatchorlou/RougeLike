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



void AbilityManager::handleInput()
{
	mActiveAbility = nullptr;

	for (std::unordered_map<std::string, Ability*>::iterator iter = mAbilities.begin(); iter != mAbilities.end(); iter++)
	{
		UIButton* button = mGameData->uiManager->findButton(iter->first);

		if (button != nullptr)
		{
			//printf("button\n");
		}

		if (button)
		{
			if (button->isActive())
			{
				mActiveAbility = iter->second;
			}
		}
	}

	if (inSelectionMode())
	{
		activateActiveAbility();
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

			std::vector<Actor*> enemies = mGameData->actors->getAllEnemies();


			for (int i = 0; i < enemies.size(); i++)
			{
				// activate ability on first enemy selected
				if (enemies[i]->collider()->contains(cursorPosition))
				{
					activate(enemies[i]);
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



void AbilityManager::slowUpdate()
{
	// in selection mode
	if (mActiveAbility)
	{

	}
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