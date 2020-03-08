#include "pch.h"
#include "Quiver.h"

#include "Projectile.h"


Quiver::Quiver(int stashCount)
{
	for (unsigned int i = 0; i < stashCount; i++)
	{
		projectileBuffer.add(new Projectile);
	}
}


Quiver::~Quiver()
{
	for (unsigned int i = 0; i < projectileBuffer.size(); i++)
	{
		delete projectileBuffer[i];
	}
}


// BUG: if you change weapon while the buffer is not full then there will be nulls
// even any in the quiver will cause this
void Quiver::init(const RangedWeaponData* data)
{
	mSize = data->quiverSize;

	for (unsigned int i = 0; i < projectileBuffer.size(); i++)
	{
		projectileBuffer[i]->init(data);
	}

	load();
}


void Quiver::load()
{
	while (loadedProjectiles.size() < mSize)
	{
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


void Quiver::lostProjectile(Projectile* projectile)
{
	projectileBuffer.insert(projectile);
}