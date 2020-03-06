#pragma once

class Collider;

class CollisionTracker
{
public:
	void addCollider(Collider* collider);
	void subscribe(Collider* collider);
	void subscribe(std::vector<Collider*> colliders);

	void clearSubscriptions();

	// Only mark base colliders as has collided
	void checkBaseCollisions();

	// Mark both base and subbed colliders as has collded
	void checkAllCollisions();

private:
	std::vector<Collider*> mBaseColliders;
	std::vector<Collider*> mSubbedColliders;
};
