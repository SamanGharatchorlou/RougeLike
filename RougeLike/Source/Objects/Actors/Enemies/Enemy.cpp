#include "pch.h"
#include "Enemy.h"

#include "Game/GameData.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Graphics/Texture.h"

#include "EnemyPropertyBag.h"
#include "Map/Environment.h"
#include "Collisions/DamageCollider.h"

#include "Objects/Actors/ActorManager.h"
#include "Objects/Actors/Player/Player.h"
#include "Objects/Effects/KnockbackEffect.h"


#include "Objects/Effects/KnockbackStunEffect.h"
#include "Animations/AnimationReader.h"

#include "Objects/Effects/DamageEffect.h"

#include "AI/AIPathMap.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Enemy::Enemy(GameData* gameData) :
	Actor(gameData),
	mStateMachine(new EnemyNullState),
	mMap(nullptr),
	mCurrentIndex(Vector2D<int>(-1,-1))
{
	physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void Enemy::init(const std::string& config)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, config));

	// Property bag
	EnemyPropertyBag* propertyBag = new EnemyPropertyBag;
	propertyBag->readProperties(config);
	setPropertyBag(propertyBag);

	// Collider	
	Damage* damage = static_cast<Damage*>(getProperty("Damage"));
	float knockback = getPropertyValue("KnockbackDistance");
	mCollider = new DamageCollider(*damage, knockback);

	Actor::init(parser);
	
#if _DEBUG
	mCollider->setName(config);
#endif
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();

	mStateMachine.getActiveState().fastUpdate(dt);

	Actor::fastUpdate(dt);
}


void Enemy::slowUpdate(float dt)
{
	// Must run first, some effects e.g. damage apply a gotHit state to the collider
	Actor::slowUpdate(dt);

	// Reset alpha for enemies sharing the same texture
	mAnimator.texture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();
	mStateMachine.getActiveState().slowUpdate(dt);

	Index index = mMap->index(position());
	if (mCurrentIndex != index)
	{
		UpdateAICostMapEvent* event = new UpdateAICostMapEvent;
		pushEvent(EventPacket(event));
		mCurrentIndex = index;
	}

	Health* health = static_cast<Health*>(getProperty("Health"));
	if (health->isDead())
		addState(EnemyState::Dead);
}


void Enemy::render()
{
	if(mVisibility)
		mStateMachine.getActiveState().render();
}


void Enemy::renderCharacter()
{
#if DRAW_ENEMY_RECTS
	debugDrawRect(rect(), RenderColour(RenderColour::Red));
	debugDrawRect(scaledRect(), RenderColour(RenderColour::Blue));
#endif	
	
	Actor::render();
}

const Map* Enemy::getEnvironmentMap() const
{
	return mGameData->environment->primaryMap();
}


// Reset everything that needs to be recalculated when spawned
void Enemy::clear()
{
	// Clear statemachine states (except null state at i = 0)
	mStateMachine.clearStates();
	mEvents.clear();

	mMap = nullptr;

	Actor::reset();
}


void Enemy::resolveCollisions()
{
	if (mCollider->gotHit())
	{
		// Player weapon hit enemy
		if (mCollider->getOtherCollider())
		{
			const DamageCollider* collider = static_cast<const DamageCollider*>(mCollider->getOtherCollider());

			// Apply knockback
			//KnockbackEffect* knockback = new KnockbackEffect(mGameData->actors->player()->position(), collider->knockbackforce());
			//mEffects.addEffect(knockback);


			XMLParser parser;
			parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, "Stun"));

			AnimationReader reader(mGameData->textureManager, parser);
			Animator animator;
			reader.initAnimator(animator);

			KnockbackStunEffect* knockbackStunEffect = new KnockbackStunEffect(mGameData->actors->player()->position(), collider->knockbackforce(), animator, 50.0f );
			mEffects.addEffect(knockbackStunEffect);

			// Apply damage
			DamageEffect* damage = new DamageEffect(collider->damage());
			mEffects.addEffect(damage);
			//printf("taking damage hp: %f\n", getPropertyValue("Health"));
		}

		addState(EnemyState::Hit);
	}
}


void Enemy::spawn(EnemyState::Type state, VectorF position)
{
	mPhysics.setPosition(position);

	addState(state);
	mStateMachine.processStateChanges();
}


void Enemy::accellerateTowards(VectorF position)
{
	mPhysics.accellerate(position - mPhysics.position());
}


void Enemy::replaceState(EnemyState::Type state)
{
	mStateMachine.replaceState(newEnemyState(state, this));
}

void Enemy::addWaitState(float waitTime)
{
	if(state() != EnemyState::Dead)
		mStateMachine.addState(new EnemyWait(this, waitTime));
}


void Enemy::addState(EnemyState::Type newState)
{
	if (state() != EnemyState::Dead)
		mStateMachine.addState(newEnemyState(newState, this));
}


void Enemy::popState()
{
	mStateMachine.popState();
}


EnemyState::Type Enemy::state() const
{
	if (mStateMachine.size() > 0)
	{
		return mStateMachine.getActiveState().type();
	}
	else
	{
		DebugPrint(Warning, "Enemy state machine has no state, size = 0\n");
		return EnemyState::None;
	}
}
