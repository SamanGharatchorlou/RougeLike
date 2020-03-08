#pragma once

#include "Utilities/RingBuffer.h"
#include "Utilities/Buffer.h"

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

	void lostProjectile(Projectile* projectile);

private:
	std::queue<Projectile*> loadedProjectiles;

	Buffer<Projectile*> projectileBuffer;

	int mSize;
};