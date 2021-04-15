#include "pch.h"
#include "AbilityHandler.h"

#include "Audio/AudioManager.h"
#include "AbilityActivator.h"
#include "AbilityClasses/Ability.h"

#include "Actors/Actor.h"
#include "Collisions/Colliders/QuadCollider.h"



void AbilityHandler::init(EffectPool* effectPool, std::vector<Actor*>* targets)
{
	mTargets = targets;
	mEffects = effectPool;
}


void AbilityHandler::clear()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		delete mAbilities[i];
		mAbilities[i] = nullptr;
	}

	mAbilities.clear();
}


void AbilityHandler::handleInput(const InputManager* input)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		if (ability->state() == AbilityState::Selected)
		{
			ability->handleInput(input);
		}
	}
}


void AbilityHandler::fastUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];

		if (ability->state() == AbilityState::Selected)
		{
			if (doesCollide(ability))
			{
				ability->setSelectHighligh(RenderColour::Green);
			}
			else
			{
				ability->setSelectHighligh(RenderColour::LightGrey);
			}
		}
		else if (ability->state() == AbilityState::Running)
		{
			ability->fastUpdate(dt);

			if (ability->shouldActivateCollisions())
			{
				activateCollisions(ability);				
			}
		}
	}
}


void AbilityHandler::slowUpdate(float dt)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		Ability* ability = mAbilities[i];
		handleState(ability, dt);

		if (ability->state() == AbilityState::Running)
			ability->slowUpdate(dt);
	}
}


void AbilityHandler::render()
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		mAbilities[i]->render();
	}
}


void AbilityHandler::handleState(Ability* ability, float dt)
{
	switch (ability->state())
	{
	case AbilityState::Activate:
	{
		ability->activate();

		if (ability->targetType() == AbilityTarget::Self)
			ability->activateOn(nullptr, mEffects);

		ability->setState(AbilityState::Running);
		break;
	}
	case AbilityState::Running:
	{
		if (ability->hasCompleted())
		{
			ability->cooldown().begin();
			ability->setState(AbilityState::Cooldown);
		}
		break;
	}
	case AbilityState::Cooldown:
	{
		if (ability->cooldown().hasCompleted())
		{
			ability->setState(AbilityState::Finished);

			if(ability->type() != AbilityType::BasicAttack)
				AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "AbilityCooled", ability));
		}
		break;
	}
	case AbilityState::Finished:
	{
		ability->exit();
		ability->baseExit();
		ability->setState(AbilityState::Idle);
		break;
	}
	case AbilityState::Selected:
	case AbilityState::Idle:
	default:
		break;
	}
}


Ability* AbilityHandler::get(AbilityType type)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		if (mAbilities[i]->type() == type)
		{
			return mAbilities[i];
		}
	}

	BasicString abilityName;
	type >> abilityName;
	DebugPrint(Warning, "Caster has no '%s' ability\n", abilityName.c_str());
	return nullptr;
}


void AbilityHandler::add(Ability* ability)
{
	for (int i = 0; i < mAbilities.size(); i++)
	{
		if (ability->type() == mAbilities[i]->type())
		{
			DebugPrint(Warning, "Ability handler already has a '%s' ability\n", ability->name());
			return;
		}
	}

	mAbilities.push_back(ability);
}


void AbilityHandler::activateCollisions(Ability* ability)
{
	// Apply effect to all enemies caught in area
	Collider* abilityCollider = ability->collider();

	std::vector<int> overlapIndexes = broadPhaseIndexes(abilityCollider, mTargets);
	for (int i = 0; i < overlapIndexes.size(); i++)
	{
		int index = overlapIndexes[i];
		Collider* targetCollider = mTargets->at(index)->collider();
		if (abilityCollider->doesIntersect(targetCollider))
		{
			if (ability->activateOn(mTargets->at(index), mEffects))
			{
				abilityCollider->setDidHit(true);
				targetCollider->setGotHit(true);
			}
		}
	}
}


bool AbilityHandler::doesCollide(Ability* ability) const
{
	// Apply effect to all enemies caught in area
	const Collider* abilityCollider = ability->selectionCollider();

	bool usingBroadPhase = true;
	std::vector<int> overlapIndexes = broadPhaseIndexes(abilityCollider, mTargets);

	for (int i = 0; i < overlapIndexes.size(); i++)
	{
		int index = overlapIndexes[i];
		Collider* targetCollider = mTargets->at(index)->collider();
		if (abilityCollider->doesIntersect(targetCollider))
		{
			return true;
		}
	}

	return false;
}