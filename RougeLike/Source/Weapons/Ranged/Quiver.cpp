#include "pch.h"
#include "Quiver.h"

#include "Projectile.h"


Quiver::Quiver(int stashCount)
{
	for (unsigned int i = 0; i < stashCount; i++)
	{
		ProjectileRingBuffer.push(new Projectile);
	}
}


Quiver::~Quiver()
{
	for (unsigned int i = 0; i < ProjectileRingBuffer.size(); i++)
	{
		delete ProjectileRingBuffer[i];
	}
}


void Quiver::init(const RangedWeaponData* data)
{
	mSize = data->quiverSize;

	for (unsigned int i = 0; i < ProjectileRingBuffer.size(); i++)
	{
		ProjectileRingBuffer[i]->init(data);
	}
}


void Quiver::load()
{
	while (loadedProjectiles.size() <= mSize)
	{
		// increment ring buffer head
		ProjectileRingBuffer.advance();

		ProjectileRingBuffer.front()->reset();

		loadedProjectiles.push(ProjectileRingBuffer.front());

	}
}


Projectile* Quiver::draw()
{
	Projectile* projectile = loadedProjectiles.front();

	loadedProjectiles.pop();

	return projectile;
}


// Assumes in order
void Quiver::lostProjectile()
{
	ProjectileRingBuffer.retreat();
}