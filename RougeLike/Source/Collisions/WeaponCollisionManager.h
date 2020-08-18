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
	void clear() { mPlayer = nullptr; mTracker = nullptr; }

	void processWeaponEffects(EffectPool* effects);

	void clearExcludedList();
	void addCollidersToExcludedList();


private:
	void updateWeaponEffect(EffectCollider* weaponCollider);
	void addWeaponEffects(EffectCollider* weaponCollider);

public:
	Player* mPlayer;
	CollisionTracker* mTracker;
};