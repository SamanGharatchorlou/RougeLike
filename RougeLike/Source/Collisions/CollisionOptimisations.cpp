#include "pch.h"
#include "CollisionOptimisations.h"

#include "Actors/Actor.h"
#include "Colliders/QuadCollider.h"

#include <thread>

// Check if the collider bounds overlaps with the target collider bounds across the x-axis
// if so, store the index for the narrow phase
std::vector<int> broadPhaseIndexes(const Collider* collider, const std::vector<Actor*>* targets)
{
	std::vector<int> overlapIndexes;
	bool use_threading = true;

	if (use_threading)
	{
		float minMax[2] = { collider->xMin(), collider->xMax() };

		int threads = 2;

		int range[2] = { 0, targets->size() };
		int rangeA[2] = { 0, targets->size() / 2 };
		int rangeB[2] = { targets->size() / 2, targets->size() };

		//braodPhaseRange(minMax, targets, range, overlapIndexes);

		std::vector<int> indexesA;
		std::vector<int> indexesB;

		std::thread threadA(braodPhaseRange, minMax, targets, rangeA, &indexesA);
		std::thread threadB(braodPhaseRange, minMax, targets, rangeB, &indexesB);

		for (int i = 0; i < indexesB.size(); i++)
		{
			indexesA.push_back(indexesB[i]);
		}
		//std::thread threadA(testFunction, minMax, range, targets, &indexesA);

		threadA.join();
		threadB.join();

		return indexesA;
	}
	else
	{

		for (int i = 0; i < targets->size(); i++)
		{
			const Collider* targetCollider = targets->at(i)->collider();

			if (targetCollider->xMax() >= collider->xMin())
			{
				if (targetCollider->xMin() <= collider->xMax())
				{
					overlapIndexes.push_back(i);
				}
			}
		}

		return overlapIndexes;
	}
}

void testFunction(float* minMax, int* range, const std::vector<Actor*>* targets, std::vector<int>* indexes)
{
	// do stuff
}


void braodPhaseRange(float* minMax, const std::vector<Actor*>* targets, int* range, std::vector<int>* indexes)
{
	float min = *minMax;
	float max = *(minMax + 1);

	for (int i = *range; i < *(range + 1); i++)
	{
		const Collider* targetCollider = targets->at(i)->collider();

		if (targetCollider->xMax() >= min)
		{
			if (targetCollider->xMin() <= max)
			{
				indexes->push_back(i);
			}
		}
	}
}