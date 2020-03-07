#include "pch.h"
#include "Quiver.h"

#include "Projectile.h"


Quiver::Quiver(int stashCount)
{
	for (unsigned int i = 0; i < stashCount; i++)
	{
		//ProjectileRingBuffer.push(new Projectile);

		projectileBuffer.add(new Projectile);
	}
}


Quiver::~Quiver()
{
	for (unsigned int i = 0; i < projectileBuffer.size(); i++)
	{
		//delete ProjectileRingBuffer[i];

		delete projectileBuffer[i];
	}
}


void Quiver::init(const RangedWeaponData* data)
{
	mSize = data->quiverSize;

	for (unsigned int i = 0; i < projectileBuffer.size(); i++)
	{
		//ProjectileRingBuffer[i]->init(data);

		projectileBuffer[i]->init(data);
	}

	load();
}


void Quiver::load()
{
	while (loadedProjectiles.size() <= mSize)
	{
		//// increment ring buffer head
		//ProjectileRingBuffer.advance();

		//ProjectileRingBuffer.front()->reset();

		//loadedProjectiles.push(ProjectileRingBuffer.front());

		loadedProjectiles.push(projectileBuffer.getAvailable());

	}
}


Projectile* Quiver::draw()
{
	Projectile* projectile = nullptr;

	if (loadedProjectiles.size() > 0)
	{
		projectile = loadedProjectiles.front();
		loadedProjectiles.pop();
	}

	return projectile;
}


// Assumes in order
void Quiver::lostProjectile(Projectile* projectile)
{
	//ProjectileRingBuffer.retreat();

	projectileBuffer.insert(projectile);
}