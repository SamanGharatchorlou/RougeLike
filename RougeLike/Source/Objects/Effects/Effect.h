#pragma once

class Actor;

enum class EffectType
{
	None,
	Damage,
	Count
};

//inline bool operator <(EffectType a, EffectType b)
//{
//	return static_cast<int>(a) < static_cast<int>(b);
//}

//inline EffectType operator =(EffectType a, EffectType b)
//{
//	static_cast<int>(a) = static_cast<int>(b);
//	return static_cast<int>(a) < static_cast<int>(b);
//}

template<class T>
inline EffectType operator +(EffectType a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<EffectType>(sum);
}

class Effect
{

public:
	Effect() : mShouldExit(false) { }
	virtual ~Effect() { }

	void set(Actor* actor) { mActor = actor; }

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }


protected:
	Actor* mActor;
	bool mShouldExit;
};