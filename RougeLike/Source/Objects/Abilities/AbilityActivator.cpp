#include "pch.h"
#include "AbilityActivator.h"

#include "AbilityTypes/Ability.h"

#include "Input/InputManager.h"
#include "Map/Environment.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"
#include "Collisions/Collider.h"


bool AbilityActivator::canSelect(Ability* ability) const
{
	if (ability->state() == Ability::Idle)
	{
		return true;
	}
	else
		return false;
}


bool AbilityActivator::selected(Ability* ability, Button::State buttonState) const
{
	AbilityType type = ability->type();

	if (buttonState == Button::State::Pressed)
		return true;
	else
		return false;
}


bool AbilityActivator::released(Ability* ability, Button::State buttonState) const
{
	AbilityType type = ability->type();

	if (buttonState == Button::State::Released)
		return true;
	else
		return false;
}


bool AbilityActivator::activate(Ability* ability, Button::State buttonState, const InputManager* input)
{
	bool didActivate = false;

	if (ability->state() == Ability::Selected)
	{
		switch (ability->targetType())
		{
			// Player casts on self
		case Ability::TargetType::Self:
		{
			if (buttonState == Button::State::Released)
			{
				TargetSelfAbility* selfAbility = static_cast<TargetSelfAbility*>(ability);
				didActivate = activateOnSelf(selfAbility);
			}
			break;
		}
		// Activate on first enemy selected
		case Ability::TargetType::Actor:
		{
			if (input->isCursorReleased(Cursor::Left))
			{
				TargetActorAbility* actorAbility = static_cast<TargetActorAbility*>(ability);
				didActivate = activateOnActor(actorAbility, input);
			}
			break;
		}
		// Select any floor tile
		case Ability::TargetType::Position:
		case Ability::TargetType::AttackArea:
		{
			if (input->isCursorReleased(Cursor::Left))
			{
				TargetPositionAbility* positionAbility = static_cast<TargetPositionAbility*>(ability);
				didActivate = activateOnPosition(positionAbility, input);
			}
			break;
		}
		default:
			break;
		}
	}

	return didActivate;
}


void AbilityActivator::activateAreaAttack(Ability* ability)
{
	TargePositionAttackAbility* attackAbility = static_cast<TargePositionAttackAbility*>(ability);
	Collider abilityCollider = attackAbility->collider();

	// Apply effect to all enemies caught in area
	std::vector<Actor*> enemies = mEnvironment->actors()->getAllEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		Collider* enemyCollider = enemies[i]->collider();
		if (enemyCollider->doesIntersect(&abilityCollider))
		{
			attackAbility->activateOn(enemies[i], mEnvironment->effectPool());
		}
	}
}


bool AbilityActivator::activateOnSelf(TargetSelfAbility* ability)
{
	ability->activate(mEnvironment->effectPool());
	return true;
}


bool AbilityActivator::activateOnActor(TargetActorAbility* ability, const InputManager* input)
{
	std::vector<Actor*> enemies = mEnvironment->actors()->getAllEnemies();
	VectorF cursorPosition = cursorWorldPosition(input->cursorPosition());
	for (int i = 0; i < enemies.size(); i++)
	{
		// activate ability on first enemy selected
		if (enemies[i]->collider()->contains(cursorPosition))
		{
			ability->activateOn(enemies[i], mEnvironment->effectPool());
			return true;
		}
	}

	return false;
}


bool AbilityActivator::activateOnPosition(TargetPositionAbility* ability, const InputManager* input)
{
	VectorF cursorPos = cursorWorldPosition(input->cursorPosition());

	// BUG: if you select just outside of the current map this breaks?
	if (ability->isValidTarget(cursorPos, map(cursorPos)))
	{
		ability->activateAt(cursorPos, mEnvironment->effectPool());
		return true;
	}

	return false;
}


VectorF AbilityActivator::cursorWorldPosition(VectorF cursorPosition) const
{
	return mEnvironment->toWorldCoords(cursorPosition);
}


const Map* AbilityActivator::map(VectorF position) const
{
	return mEnvironment->map(position);
}
