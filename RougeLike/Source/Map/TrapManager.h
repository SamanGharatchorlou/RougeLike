#pragma once


#include "Collisions/Colliders/EffectCollider.h"
#include "Objects/Properties/Attributes/Damage.h"


class CollisionTracker;
class Map;
class MapTile;
class Actor;
class EffectPool;
enum class DecorType : Uint32;


using TrapDataMap = std::unordered_map<DecorType, PropertyMap>;

class TrapManager
{
public:
	class Trap
	{
	public:
		Trap(Index index, const PropertyMap& properties) : mIndex(index), mData(properties), timer(TimerF::Start), exhausted(false) { }

		inline bool operator == (Trap value)
		{
			return hasIndex(value.index());
		}

		Index index() const { return mIndex; }
		bool hasIndex(Index index) const { return index == mIndex; }

		bool isExhausted() const { return exhausted; }
		void setExhausted(bool isExhausted) { exhausted = isExhausted; }

		float time() const { return timer.getSeconds(); }
		void reset() { timer.restart(); }

		bool canTrigger() const { return timer.getSeconds() > mData.at(PropertyType::Time); }
		bool canUntrigger() const { return timer.getSeconds() > mData.at(PropertyType::Cooldown); }
		bool doesCollide(Index index) const { return mIndex == index && !exhausted; }

		const PropertyMap& properties() const { return mData; }

	private:
		Index mIndex;
		TimerF timer;
		bool exhausted;
		const PropertyMap& mData;
	};


public:
	TrapManager(Map* map) : mMap(map), mTrapData(nullptr), mActor(nullptr), mEffectPool(nullptr) { }

	void init(Actor* actor, EffectPool* effectPool, const TrapDataMap* trapData);
	void flushQueues();

	void slowUpdate();
	void processCollisions(VectorF position);


private:	
	
	void triggerAll(DecorType type);
	void triggerTrap(VectorF position);

	void resolveCollision(Trap& trap);

	void updateTriggerTraps();
	void updateResetTraps();

	MapTile* mapTile(Trap trap) const;


private:
	EffectPool* mEffectPool;
	Actor* mActor;
	Map* mMap;

	const TrapDataMap* mTrapData;

	UniqueQueue<Trap> mUntriggeredTraps;
	UniqueQueue<Trap> mTriggeredTraps;

	std::vector<Trap> mPersistingTraps;
};