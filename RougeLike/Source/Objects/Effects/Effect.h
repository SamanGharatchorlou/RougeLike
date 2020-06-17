#pragma once

class Actor;

enum class EffectType
{
	None,
	Damage,
	Displacement,
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
	Effect() : mShouldExit(false) { }
	virtual ~Effect() { }

	void setReceiver(Actor* receiver) { 
		if (receiver == nullptr) 
			printf("pause"); 
		mReceiver = receiver; }

	virtual void fillData(const Actor* distributer) { }
	virtual void clearData() { mReceiver = nullptr; mShouldExit = false; }

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() { mReceiver = nullptr; mShouldExit = false; }

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }

	virtual EffectType type() const { return EffectType::None; }


protected:
	bool hasProperty(const Actor* distributer, const std::string property);


protected:
	Actor* mReceiver;
	bool mShouldExit;


};