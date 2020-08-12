#pragma once

#include <unordered_set>

class Collider;

class CollisionTracker
{
public:
	void addAttackers(std::vector<Collider*> colliders);
	void addDefenders(std::vector<Collider*> colliders);

	void clearAttackers();
	void clearDefenders();
	void removeAttacker(const Collider* collider);
	void removeDefender(const Collider* collider);

	std::vector<Collider*> defenders() { return mDefenders; }

	void checkCollisions();
	void resetColliders();

	void setCheckingStatus(bool shouldCheckCollisions) { mCheckingCollisions = shouldCheckCollisions; }

	void addExcludedDefender(Collider* defender);
	void clearExcludedDefenders();

private:
	void addAttacker(Collider* colliders);
	void addDefender(Collider* colliders);


protected:
	std::vector<Collider*> mAttackers;
	std::vector<Collider*> mDefenders;

	std::unordered_set<Collider*> mExcludedDefenders;

	bool mCheckingCollisions = true;
};

