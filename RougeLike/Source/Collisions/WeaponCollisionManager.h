#pragma once


class Player;
class EffectCollider;
class CollisionTracker;


class WeaponCollisionManager
{
public:
	WeaponCollisionManager(Player* player) : mPlayer(player) { }

	void setTracker(CollisionTracker* tracker);

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