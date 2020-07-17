#include "pch.h"
#include "AbilityActivator.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Map/Environment.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"

#include "Collisions/Collider.h"

#include "AbilityManager.h"


AbilityActivator::AbilityActivator(AbilityManager* manager) : mManager(manager) { }

bool AbilityActivator::shouldActivate(Ability* ability, InputManager* input)
{
	bool activate = false;

	switch (ability->targetType())
	{
		// Player casts on self only
	case Ability::TargetType::Self:
	{
		Button::Key hotKey = mManager->mHotKeys.hotKey(ability);
		activate = input->isReleased(hotKey);
		break;
	}
	// Activate on first enemy selected
	case Ability::TargetType::Actor:
	case Ability::TargetType::Position:
	case Ability::TargetType::AttackArea:
	{
		activate = input->isCursorReleased(Cursor::Left);
		break;
	}
	default:
		break;
	}

	return activate;
}


bool AbilityActivator::activate(Ability* ability)
{
	bool didActivate = false;

	switch (ability->targetType())
	{
		// Player casts on self only
	case Ability::TargetType::Self:
	{
		TargetSelfAbility* selfAbility = static_cast<TargetSelfAbility*>(ability);
		didActivate = activateOnSelf(selfAbility);
		break;
	}
	// Activate on first enemy selected
	case Ability::TargetType::Actor:
	{
		TargetActorAbility* actorAbility = static_cast<TargetActorAbility*>(ability);
		didActivate = activateOnActor(actorAbility);
		break;
	}
	// Select any floor tile
	case Ability::TargetType::Position:
	case Ability::TargetType::AttackArea:
	{
		TargetPositionAbility* positionAbility = static_cast<TargetPositionAbility*>(ability);
		didActivate = activateOnPosition(positionAbility);
		break;
	}
	default:
		break;
	}

	return didActivate;
}


void AbilityActivator::activateAreaAttack(Ability* ability)
{
	TargePositionAttackAbility* attackAbility = static_cast<TargePositionAttackAbility*>(ability);
	Collider abilityCollider = attackAbility->collider();

	// Apply effect to all enemies caught in area
	std::vector<Actor*> enemies = mManager->mGameData->environment->actors()->getAllEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		Collider* enemyCollider = enemies[i]->collider();
		if (enemyCollider->doesIntersect(&abilityCollider))
		{
			attackAbility->activateOn(enemies[i], mManager->mGameData->effectPool);
		}
	}
}


bool AbilityActivator::activateOnSelf(TargetSelfAbility* ability)
{
	ability->activate(mManager->mGameData->effectPool);
	return true;
}


bool AbilityActivator::activateOnActor(TargetActorAbility* ability)
{
	std::vector<Actor*> enemies = mManager->mGameData->environment->actors()->getAllEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		// activate ability on first enemy selected
		if (enemies[i]->collider()->contains(cursorPosition()))
		{
			ability->activateOn(enemies[i], mManager->mGameData->effectPool);
			return true;
		}
	}

	return false;
}


bool AbilityActivator::activateOnPosition(TargetPositionAbility* ability)
{
	VectorF cursorPos = cursorPosition();

	// BUG: if you select just outside of the current map this breaks?
	if (ability->isValidTarget(cursorPos, map(cursorPos)))
	{
		ability->activateAt(cursorPos, mManager->mGameData->effectPool);
		return true;
	}

	return false;
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
