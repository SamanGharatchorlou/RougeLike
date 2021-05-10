#pragma once

#include "Objects/Abilities/Cooldown.h"

class WeaponData;
class EffectPool;
class Effect;
class Collider;


class Weapon
{
public:
	Weapon() : mAttacking(false) { }
	virtual ~Weapon() { };

	virtual bool canAttack() = 0;
	virtual void attack() = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) { };
	virtual void render() = 0;

	virtual std::vector<Collider*> getColliders() = 0;

	virtual const BasicString& hitSoundLabel() = 0;
	virtual const BasicString& missSoundLabel() = 0;

	bool isAttacking() const { return mAttacking; }

	virtual RectF rect() { return mRect.Translate(offset()); }
	virtual void setPosition(VectorF position) { mRect.SetBotCenter(position); }
	
	virtual void leftFlip() { offset().x = std::abs(offset().x) * -1.0f; }
	virtual void rightFlip() { offset().x = std::abs(offset().x); }

	virtual VectorF& offset() = 0;
	virtual void setOffset(VectorF newOffset) { offset() = newOffset; }
	virtual void updateAimDirection(VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual const WeaponData* getData() = 0;

	virtual std::vector<Effect*> getEffects(EffectPool* effectPool) = 0;


protected:
	RectF mRect;
	Cooldown mCooldown;

	VectorF mDirection;
	bool mAttacking;
};