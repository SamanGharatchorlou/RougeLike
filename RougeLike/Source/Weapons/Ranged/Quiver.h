#pragma once

#include "Utilities/RingBuffer.h"

struct RangedWeaponData;
class Projectile;

class Quiver
{
public:
	Quiver(int stashCount);
	~Quiver();

	void init(const RangedWeaponData* data);

	void load();

	Projectile* draw();

	void lostProjectile();

private:
	std::queue<Projectile*> loadedProjectiles;

	RingBuffer<Projectile*> ProjectileRingBuffer;

	int mSize;

};