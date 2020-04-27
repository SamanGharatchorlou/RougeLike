#include "pch.h"
#include "EnemyHit.h"

#include "Game/GameData.h"
#include "Characters/Enemies/Enemy.h"

#include "Collisions/DamageCollider.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"

// TEMP
#include "Map/MapLevel.h"
#include "Map/Map.h"


EnemyHit::EnemyHit(Enemy* enemy) : EnemyState(enemy) { }


void EnemyHit::init()
{
	decayTimer.restart();

	mEnemy->getAnimator()->selectAnimation("Hit");

	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->modifyAlpha(-100);

	attackTargetPosition = mEnemy->attackTargetRect()->Center();
	mEnemy->facePoint(attackTargetPosition);

	// Attackers weapon collider info
	const DamageCollider* damageCollider = static_cast<const DamageCollider*>(mEnemy->getCollider()->getOtherCollider());

	// Set/reduce hp
	Health health = mEnemy->propertyBag().pHealth.get() - damageCollider->damage();
	mEnemy->propertyBag().pHealth.set(health);

	// Store knockback info before collision info is reset (next fast frame)
	mKnockbackForce = damageCollider->knockbackforce(); 
}


void EnemyHit::fastUpdate(float dt)
{
	VectorF direction = mEnemy->position() - attackTargetPosition;
	VectorF velocity = direction.normalise() * mKnockbackForce;

	if (canMove(velocity, dt))
		mEnemy->move(velocity, dt);
}


void EnemyHit::slowUpdate(float dt)
{
	if (decayTimer.getSeconds() > mEnemy->propertyBag().pHurtTime.get())
	{
		if (mEnemy->propertyBag().pHealth.get().isDead())
			mEnemy->replaceState(EnemyState::Dead);
		else
			mEnemy->replaceState(EnemyState::Run);
	}
}


void EnemyHit::render()
{
	RectF rect = mEnemy->renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

#if DRAW_ENEMY_RECT
	debugDrawRect(mEnemy->rect(), RenderColour(RenderColour::Red));
#else
	mEnemy->getAnimator()->getSpriteTile()->render(rect, mEnemy->flip());
#endif
}


void EnemyHit::exit()
{
	Texture* texture = mEnemy->getAnimator()->getSpriteTexture();
	texture->setAlpha(alphaMax);

	mEnemy->facePoint(mEnemy->attackTargetRect()->Center());
}


// --- Private Functions --- //
bool EnemyHit::canMove(VectorF velocity, float dt)
{
	Map* map = mEnemy->getData()->level->primaryMap();

	VectorF position = mEnemy->position() + (velocity * dt);
	Index index = map->index(position);

	return map->isValidIndex(index) && map->floorCollisionTile(index);
}