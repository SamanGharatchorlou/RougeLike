#include "pch.h"
#include "AbilityActivator.h"

#include "AbilityClasses/TargetAbilities.h"

#include "Input/InputManager.h"
#include "Game/Environment.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"
#include "Collisions/Colliders/Collider.h"


bool AbilityActivator::canSelect(Ability* ability) const
{
	if (ability->state() == AbilityState::Idle)
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


bool AbilityActivator::activate(Ability* ability, Button::State buttonState, const InputManager* input) const
{
	bool didActivate = false;

	if (ability->state() == AbilityState::Selected)
	{
		switch (ability->targetType())
		{
			// Player casts on self
		case AbilityTarget::Self:
		{
			if (buttonState == Button::State::Released)
			{
				TargetSelfAbility* selfAbility = static_cast<TargetSelfAbility*>(ability);
				didActivate = activateOnSelf(selfAbility);
			}
			break;
		}
		// Activate on first enemy selected
		case AbilityTarget::Actor:
		{
			if (input->isCursorReleased(Cursor::Left))
			{
				TargetActorAbility* actorAbility = static_cast<TargetActorAbility*>(ability);
				didActivate = activateOnActor(actorAbility, input);
			}
			break;
		}
		// Select any floor tile
		case AbilityTarget::Position:
		case AbilityTarget::AttackArea:
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


void AbilityActivator::activateAreaAttack(Ability* ability) const
{
	TargePositionAttackAbility* attackAbility = static_cast<TargePositionAttackAbility*>(ability);
	Collider* abilityCollider = attackAbility->collider();

	// Apply effect to all enemies caught in area
	std::vector<Actor*> enemies = mEnvironment->actors()->getAllEnemies();
	for (int i = 0; i < enemies.size(); i++)
	{
		// TODO: dont know why but... make this abiltiyCollider->doesIntersect(enemy) feels better
		Collider* enemyCollider = enemies[i]->collider();
		if (abilityCollider->doesIntersect(enemyCollider))//->doesIntersect(abilityCollider))
		{
			attackAbility->activateOn(enemies[i], mEnvironment->effectPool());
		}
	}
}

 
bool AbilityActivator::activateOnSelf(TargetSelfAbility* ability) const
{
	ability->activate(mEnvironment->effectPool());
	return true;
}


bool AbilityActivator::activateOnActor(TargetActorAbility* ability, const InputManager* input) const
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


bool AbilityActivator::activateOnPosition(TargetPositionAbility* ability, const InputManager* input) const
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
