#include "pch.h"
#include "EnemyHit.h"

#include "Game/GameData.h"
#include "Objects/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Map/Environment.h"
#include "Map/Map.h"


EnemyHit::EnemyHit(Enemy* enemy) : EnemyState(enemy) { }


void EnemyHit::init()
{
	decayTimer.restart();

	mEnemy->animator().selectAnimation("Hit");

	Texture* texture = mEnemy->animator().getSpriteTexture();
	texture->modifyAlpha(-100);

	attackTargetPosition = mEnemy->attackTargetRect()->Center();
	mEnemy->facePoint(attackTargetPosition);

	// Attackers weapon collider info
	const DamageCollider* damageCollider = static_cast<const DamageCollider*>(mEnemy->collider()->getOtherCollider());

	// Set/reduce hp
	Health* health = static_cast<Health*>(mEnemy->getProperty("Health"));
	*health = *health - damageCollider->damage();

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
	if (decayTimer.getSeconds() > mEnemy->getPropertyValue("HurtTime"));
	{
		Health* health = static_cast<Health*>(mEnemy->getProperty("Health"));

		if (health->isDead())
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
	mEnemy->animator().getSpriteTile()->render(rect, mEnemy->flip());
#endif
}


void EnemyHit::exit()
{
	Texture* texture = mEnemy->animator().getSpriteTexture();
	texture->setAlpha(alphaMax);

	mEnemy->facePoint(mEnemy->attackTargetRect()->Center());
}


// --- Private Functions --- //
bool EnemyHit::canMove(VectorF velocity, float dt)
{
	Map* map = mEnemy->getData()->environment->primaryMap();

	VectorF position = mEnemy->position() + (velocity * dt);
	Index index = map->index(position);

	return map->isValidIndex(index) && map->floorCollisionTile(index);
}