#include "pch.h"
#include "KnockbackEffect.h"


KnockbackEffect::KnockbackEffect(Physics* physics, VectorF source, float force) :
	mPhysics(physics), mSource(source), mForce(force) { }


void KnockbackEffect::init()
{
	timer.restart();
}

void KnockbackEffect::fastUpdate(float dt)
{
	VectorF direction = mPhysics->position() - mSource;
	VectorF velocity = direction.normalise() * mForce;

	//if (canMove(velocity, dt))
		mPhysics->move(velocity, dt);
}

void KnockbackEffect::slowUpdate(float dt)
{
	if (timer.getSeconds() > 0.1f)
		endEffect();

}

void KnockbackEffect::render()
{

}

void KnockbackEffect::exit()
{

}

// --- Private Functions --- //
//bool KnockbackEffect::canMove(VectorF velocity, float dt)
//{
//	Map* map = mEnemy->getData()->environment->primaryMap();
//
//	VectorF position = mEnemy->position() + (velocity * dt);
//	Index index = map->index(position);
//
//	return map->isValidIndex(index) && map->floorCollisionTile(index);
//}