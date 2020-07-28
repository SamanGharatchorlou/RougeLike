#pragma once


class Player;
class EffectCollider;
class CollisionTracker;
class EffectPool;

class WeaponCollisionManager
{
public:
	WeaponCollisionManager() { }

	void init(Player* player, CollisionTracker* tracker);

	void processWeaponEffects(EffectPool* effects);

	void clearExcludedList();
	void addCollidersToExcludedList();


private:
	void updateWeaponEffect(EffectCollider* weaponCollider);
	void addWeaponEffects(EffectCollider* weaponCollider, EffectPool* effects);

public:
	Player* mPlayer;
	CollisionTracker* mTracker;
};