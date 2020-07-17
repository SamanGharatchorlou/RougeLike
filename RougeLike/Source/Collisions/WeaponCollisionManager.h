#pragma once


class Player;
class EffectCollider;
class CollisionTracker;


class WeaponCollisionManager
{
public:
	WeaponCollisionManager() { }

	void init(Player* player, CollisionTracker* tracker);

	void processWeaponEffects();

	void clearExcludedList();
	void addEnemiesToExcludedList();


private:
	void updateWeaponEffect(EffectCollider* weaponCollider);
	void addWeaponEffects(EffectCollider* weaponCollider);

public:
	Player* mPlayer;
	CollisionTracker* mTracker;
};