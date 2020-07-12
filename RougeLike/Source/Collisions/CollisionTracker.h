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



//class ComplexCollisionTracker : public CollisionTracker
//{
//public:
//
//	//ComplexCollisionTracker() : mCheckingCollisions(true) { }
//
//	//virtual void checkCollisions() override;
//
//	//void setCheckingStatus(bool shouldCheckCollisions) { mCheckingCollisions = shouldCheckCollisions; }
//
//	//void addExcludedDefender(Collider* defender);
//	//void clearExcludedDefenders();
//
//	//std::vector<Collider*> defenders() { return mDefenders; }
//
//	//int count() { return mExcludedDefenders.size(); }
//
//
//private:
//	std::unordered_set<Collider*> mExcludedDefenders;
//
//	bool mCheckingCollisions;
//};