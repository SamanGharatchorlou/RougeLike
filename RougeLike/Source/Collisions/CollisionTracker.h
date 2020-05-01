#pragma once

class Collider;

class CollisionTracker
{
public:
	void addAttackers(std::vector<Collider*> colliders);
	void addDefenders(std::vector<Collider*> colliders);

	void clearAttackers();
	void removeDefender(Collider* collider);

	void checkCollisions();
	void resetColliders();

private:
	void addAttacker(Collider* colliders);
	void addDefender(Collider* colliders);

private:
	std::vector<Collider*> mAttackers;
	std::vector<Collider*> mDefenders;
};
