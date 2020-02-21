#pragma once

class Collider;

class CollisionTracker
{
public:
	void addCollider(Collider* collider);
	void subscribe(Collider* collider);
	void subscribe(std::vector<Collider*> colliders);

	void clearSubscriptions();

	void checkForCollisions();

private:
	std::vector<Collider*> mBaseColliders;
	std::vector<Collider*> mSubbedColliders;
};
