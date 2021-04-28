#pragma once

class Actor;
class Weapon;
class EffectPool;

class WeaponCollisionTracker
{
public:
	void init(EffectPool* effectPool) { mEffectPool = effectPool; }
	void linkTargets(std::vector<Actor*>* targets) { mTargets = targets; }
	void linkWeapon(Weapon* weapon) { mWeapon = weapon; }

	void checkCollisions();

	void beginAttack();

private:
	EffectPool* mEffectPool;

	std::vector<Actor*>* mTargets;
	Weapon* mWeapon;

	Queue<Actor*> mHitTargets;
};
