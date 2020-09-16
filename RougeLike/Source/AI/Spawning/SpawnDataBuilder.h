#pragma once

#include "Actors/Enemies/EnemyStates/EnemyState.h"

class AIPathMap;
class Formation;
enum class EnemyType;

struct SpawnData
{
	SpawnData(EnemyType enemyType, EnemyState::Type enemyState, VectorF enemyPosition) : 
		type(enemyType), state(enemyState), position(enemyPosition) { }

	EnemyType type;
	EnemyState::Type state;
	VectorF position;
};


// Track how many of each enemy we will need
struct SpawnDataList
{
	SpawnDataList() : mMap(nullptr) { }

	void init(const AIPathMap* map)
	{
#if _DEBUG
		if (hasData())
		{
			DebugPrint(Warning, "Attempting to init the SpawnList when it still has data left over (count: %d), \
				was it not spawned for the previous level?\n", mData.size());
		}
#endif

		mMap = map;
		mTypeCount.clear();
	}

	bool hasData() const { return mData.size(); }

	void add(const Queue<SpawnData>& spawnData)
	{
		if (spawnData.size() > 0)
		{
			EnemyType type = spawnData.front().type;
			int size = spawnData.size();

			if (mTypeCount.count(type) > 0)
			{
				size += mTypeCount[type];
			}
			
			mTypeCount[type] = size;
			mData.concat(spawnData);
		}
	}

	void setSpawnPerFrames(int frames)
	{
		// split the spawning over 100 frames
		float count = (float)mData.size();
		count /= (float)frames;

		mSpawnsPerFrame = (int)ceilf(count);
	}

	const AIPathMap* mMap;
	Queue<SpawnData> mData;
	std::unordered_map<EnemyType, int> mTypeCount;

	int mSpawnsPerFrame;
};


class SpawnDataBuilder
{
public:
	Queue<SpawnData> buildSpawnData(Formation& formation, EnemyType type, EnemyState::Type state);

	void setRandomPosition(Formation& formation, const AIPathMap* map);
	Vector2D<int> getRandomFormationSize(int mapLevel) const;

private:
	Queue<SpawnData> buildFormationDataList(const Formation& formation, EnemyState::Type state, EnemyType type) const;

	bool validFormation(const Formation& formation, const AIPathMap* map);
};