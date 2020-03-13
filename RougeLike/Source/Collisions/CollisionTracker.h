#pragma once

class Collider;

class CollisionTracker
{
public:
	void addCollider(Collider* collider);
	void subscribe(Collider* collider);
	void subscribe(std::vector<Collider*> colliders);

	void addAttackers(std::vector<Collider*> attackers);
	void addDefender(Collider* collider);

	void clearSubscriptions();

	void clearAttackers();

	// Only mark base colliders as has collided
	void checkBaseCollisions();

	// Mark both base and subbed colliders as has collded
	void checkAllCollisions();

	void checkCollisions();

	// TEMP
	std::vector<Collider*> mAttackers;

private:
	std::vector<Collider*> mDefenders;
};
