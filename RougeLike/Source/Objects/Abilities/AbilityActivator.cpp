#include "pch.h"
#include "AbilityActivator.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Map/Environment.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Actors/ActorManager.h"

#include "Collisions/Collider.h"

#include "AbilityManager.h"


AbilityActivator::AbilityActivator(AbilityManager* manager, Ability* ability) : mManager(manager)
{
	attemptActivation(ability);
}

void AbilityActivator::attemptActivation(Ability* ability)
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


void AbilityActivator::attemptActivationOnSelf(Ability* ability)
{
	mManager->setState(ability, Ability::Activating);
	ability->activate(ability->caster(), mManager->mGameData->effectPool);
}


void AbilityActivator::attemptActivationOnSingleEnemy(Ability* ability)
{
	if (mManager->mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		std::vector<Actor*> enemies = mManager->mGameData->actors->getAllEnemies();
		for (int i = 0; i < enemies.size(); i++)
		{
			// activate ability on first enemy selected
			if (enemies[i]->collider()->contains(cursorPosition()))
			{
				//ability->setState(Ability::Activating);
				mManager->setState(ability, Ability::Activating);
				ability->activate(enemies[i], mManager->mGameData->effectPool);
				break;
			}
		}
	}
}


void AbilityActivator::attemptActivationOnArea(Ability* ability)
{
	if (mManager->mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		VectorF cursorPos = cursorPosition();

		// Activate ability
		AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);

		// BUG: if you select just outside of the current map this breaks?
		if (areaAbility->isValidTarget(cursorPos, map(cursorPos)))
		{
			areaAbility->activate(cursorPos);
			activateOnArea(areaAbility);

			setState(areaAbility, Ability::Activating);
		}
	}
}

void AbilityActivator::activateOnArea(AreaAbility* areaAbility)
{
	Collider abilityCollider = areaAbility->collider();

	// Apply effect to all enemies caught in area
	std::vector<Actor*> enemies = mManager->mGameData->actors->getAllEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		Collider* enemyCollider = enemies[i]->collider();
		if (enemyCollider->doesIntersect(&abilityCollider))
		{
			areaAbility->activate(enemies[i], mManager->mGameData->effectPool);
		}
	}
}



void AbilityActivator::attemptActivationOnPoint(Ability* ability)
{
	if (mManager->mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		VectorF cursorPos = cursorPosition();

		// Activate ability
		AreaAbility* areaAbility = static_cast<AreaAbility*>(ability);
		if (areaAbility->isValidTarget(cursorPos, map(cursorPos)))
		{
			// Apply effect
			areaAbility->activate(cursorPos);
			areaAbility->activate(ability->caster(), mManager->mGameData->effectPool);

			setState(areaAbility, Ability::Activating);
		}
	}
}


VectorF AbilityActivator::cursorPosition() const
{
	VectorF cursorPosition = mManager->mGameData->inputManager->cursorPosition();
	return mManager->mGameData->environment->toWorldCoords(cursorPosition);
}


const Map* AbilityActivator::map(VectorF position) const
{
	return mManager->mGameData->environment->map(position);
}


void AbilityActivator::setState(Ability* ability, Ability::State state)
{
	mManager->setState(ability, state);
}
