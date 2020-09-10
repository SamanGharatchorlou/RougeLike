#pragma once


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

		inline bool operator == (const Trap& value) const
		{
			return hasIndex(value.index());
		}

		Index index() const { return mIndex; }
		bool hasIndex(Index index) const { return index == mIndex; }

		bool isExhausted() const { return exhausted; }
		void setExhausted(bool isExhausted) { exhausted = isExhausted; }

		void pause() { timer.pause(); }
		void resume() { timer.resume(); }
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
	~TrapManager();

	void init(Actor* actor, EffectPool* effectPool, const TrapDataMap* trapData);

	void slowUpdate();
	void processCollisions(Index index);

	void pause();
	void resume();

private:	
	
	void triggerAll(DecorType type, VectorF triggerPosition);
	void triggerTrap(Index index);

	void resolveCollision(Trap& trap);

	void updateTriggerTraps();
	void updateResetTraps();

	MapTile* mapTile(const Trap& trap) const;


private:
	EffectPool* mEffectPool;
	Actor* mActor;
	Map* mMap;

	const TrapDataMap* mTrapData;

	UniqueQueue<Trap> mUntriggeredTraps;
	UniqueQueue<Trap> mTriggeredTraps;

	std::vector<Trap> mPersistingTraps;
};