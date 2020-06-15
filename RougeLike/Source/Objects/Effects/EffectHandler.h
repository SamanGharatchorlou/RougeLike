#pragma once

class EffectPool;
class Effect;
class Actor;

//struct EffectData
//{
//	EffectType type = EffectType::None;
//};
//
//
//struct DamageEffectData : EffectData
//{
//	DamageEffectData() { type = EffectType::Damage; }
//	float damage = 100.0f;
//};


class EffectHandler
{
public:
	EffectHandler(EffectPool* pool, Actor* actor) : mPool(pool), mActor(actor) { };

	void addEffect(Effect* effect);
	//void addEffect(EffectData* data);

	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();


private:
	EffectPool* mPool;
	Actor* mActor;
	std::vector<Effect*> mEffects;

	bool mDelayedAdd;
	std::queue<Effect*> mEffectsToAdd;
};

