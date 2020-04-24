#pragma once

class Collider;

class CollisionTracker
{
public:
	void addAttackers(std::vector<Collider*> attackers);
	void addDefender(Collider* collider);

	void clearAttackers();

	void checkAttackerCollisions();
	void checkDefenderCollisions();
	void checkAttackerDefenderCollisions();

private:
	std::vector<Collider*> mAttackers;
	std::vector<Collider*> mDefenders;
};
