#pragma once

#include "Ability.h"
#include "Events/Events.h"

class Map;

class RangedAbility : public Ability
{
public:
	RangedAbility() : mRangeCircle(nullptr) { }
	virtual ~RangedAbility() { }

	void init() override;

	bool initiate(const InputManager* input) const override;

	EventPacket renderRangeCircleEvent();

	virtual bool isValidTarget(VectorF target, const Map* map) const = 0;

	AbilityTarget targetType() const override { return AbilityTarget::Ranged; }

	virtual bool requiresTarget() const { return false; }

protected:
	Texture* mRangeCircle;
};



class TargetAreaRangedAbility : public RangedAbility
{
public:
	virtual bool activateOn(Actor* target, EffectPool* effectPool) override;

	virtual bool isValidTarget(VectorF target, const Map* map) const override;
	virtual void baseExit() override;

private:
	virtual void applyEffects(Actor* actor, EffectPool* effectPool) = 0;


protected:
	std::unordered_set<Actor*> mHitList;
};


class DirectionalAreaRangedAbility : public TargetAreaRangedAbility
{
public:
	virtual bool isValidTarget(VectorF target, const Map* map) const override { return true; }
};