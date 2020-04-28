#pragma once

class Collider;

class CollisionTracker
{
public:

	void addAttacker(Collider* attackers);
	void addAttackers(std::vector<Collider*> attackers);
	void addDefender(Collider* collider);

	void clearAttackers();
	void removeDefender(Collider* collider);

	void checkAttackerCollisions();
	void checkDefenderCollisions();
	void checkAttackerDefenderCollisions();

private:
	std::vector<Collider*> mAttackers;
	std::vector<Collider*> mDefenders;
};
