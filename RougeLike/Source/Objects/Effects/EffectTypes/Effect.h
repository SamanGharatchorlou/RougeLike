#pragma once

class Actor;
class EffectPropertyBag;

enum class EffectType
{
	None,

	Armor,
	Heal,

	Blink,

	Damage,
	Displacement,
	Stun,
	KnockbackStun,

	Count
};


template<class T>
inline EffectType operator +(EffectType a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<EffectType>(sum);
}


class Effect
{

public:
	Effect() : mReceiver(nullptr), mShouldExit(false) { }
	virtual ~Effect() { }

	void setReceiver(Actor* receiver) { mReceiver = receiver; }

	virtual void clearData()  = 0;

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }

	virtual EffectType type() const = 0;


protected:
	void clearBaseData() { mReceiver = nullptr; mShouldExit = false; }


protected:
	Actor* mReceiver;
	bool mShouldExit;
};