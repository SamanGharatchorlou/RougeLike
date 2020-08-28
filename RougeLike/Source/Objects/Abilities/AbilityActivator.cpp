#include "pch.h"
#include "AbilityActivator.h"

#include "AbilityClasses/RangedAbility.h"
#include "AbilityClasses/MeleeAbility.h"

#include "Input/InputManager.h"
#include "Game/Environment.h"

#include "Actors/Actor.h"
#include "Actors/ActorManager.h"
#include "Collisions/Colliders/Collider.h"


bool AbilityActivator::selected(Ability* ability, Button::State buttonState) const
{
	AbilityType type = ability->type();

	if (buttonState == Button::State::Pressed && ability->state() == AbilityState::Idle)
		return true;
	else
		return false;
}


bool AbilityActivator::deselected(Ability* ability, Button::State buttonState) const
{
	AbilityType type = ability->type();

	if (buttonState == Button::State::Released && ability->state() == AbilityState::Selected)
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
				ability->activateOn(nullptr, mEnvironment->effectPool());
				didActivate = true;
			}
			break;
		}
		case AbilityTarget::Ranged:
		{
			if (input->isCursorReleased(Cursor::Left))
			{
				RangedAbility* rangedAbility = static_cast<RangedAbility*>(ability);

				VectorF cursorPos = cursorWorldPosition(input->cursorPosition());

				// BUG: if you select just outside of the current map this breaks?
				if (rangedAbility->isValidTarget(cursorPos, map(cursorPos)))
				{
					ability->activate(cursorPos);
					didActivate = true;
				}
			}
			break;
		}
		case AbilityTarget::Melee:
		{
			if (input->isCursorReleased(Cursor::Left))
			{
				MeleeAbility* meleeAbility = static_cast<MeleeAbility*>(ability);
				VectorF cursorPos = cursorWorldPosition(input->cursorPosition());
				meleeAbility->activate(cursorPos);
				return true;
			}
			break;
		}
		default:
			break;
		}
	}

	return didActivate;
}


void AbilityActivator::activateCollisions(Ability* ability) const
{	
	// Apply effect to all enemies caught in area
	std::vector<Actor*> enemies = mEnvironment->actors()->getAllEnemies();
	Collider* abilityCollider = ability->collider();

	for (int i = 0; i < enemies.size(); i++)
	{
		Collider* enemyCollider = enemies[i]->collider();
		if (abilityCollider->doesIntersect(enemyCollider))
		{
			ability->activateOn(enemies[i], mEnvironment->effectPool());
		}
	}
}


VectorF AbilityActivator::cursorWorldPosition(VectorF cursorPosition) const
{
	return mEnvironment->toWorldCoords(cursorPosition);
}


const Map* AbilityActivator::map(VectorF position) const
{
	return mEnvironment->map(position);
}
