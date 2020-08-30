#include "pch.h"
#include "AbilityActivator.h"

#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"

#include "AbilityClasses/RangedAbility.h"
#include "AbilityClasses/MeleeAbility.h"

#include "Actors/Actor.h"
#include "Collisions/Colliders/Collider.h"


void AbilityActivator::init(EffectPool* effectPool, std::vector<Actor*>* targets)
{
	mEffectPool = effectPool;
	mTargets = targets;
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
				ability->activateOn(nullptr, mEffectPool);
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
				const Map* map = ability->caster()->currentMap();
				if (rangedAbility->isValidTarget(cursorPos, map))
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
	Collider* abilityCollider = ability->collider();

	for (int i = 0; i < mTargets->size(); i++)
	{
		Collider* targetCollider = mTargets->at(i)->collider();
		if (abilityCollider->doesIntersect(targetCollider))
		{
			if (ability->activateOn(mTargets->at(i), mEffectPool))
			{
				abilityCollider->setDidHit(true);
				targetCollider->setGotHit(true);
			}
		}
	}
}


VectorF AbilityActivator::cursorWorldPosition(VectorF cursorPosition) const
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	return cursorPosition + cameraPosition;
}

