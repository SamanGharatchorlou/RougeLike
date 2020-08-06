#pragma once


struct WeaponData;
class Effect;
class Collider;


class Weapon
{
public:
	Weapon() : mOverrideCursorControl(false), mAttacking(false), mCanPlayHitSound(true) { }
	virtual ~Weapon() { };

	virtual void attack() = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;

	virtual bool canPlayHitSound() const { return mCanPlayHitSound; }
	virtual const BasicString& hitSoundLabel() = 0;
	virtual const BasicString& missSoundLabel() = 0;

	bool isAttacking() const { return mAttacking; }
	virtual bool didHit() const = 0;

	virtual RectF rect() { return mRect.Translate(offset()); }
	virtual void setPosition(VectorF position) { mRect.SetBotCenter(position); }
	
	virtual void leftFlip() { offset().x = std::abs(offset().x) * -1.0f; }
	virtual void rightFlip() { offset().x = std::abs(offset().x); }

	virtual VectorF& offset() = 0;
	virtual void setOffset(VectorF newOffset) { offset() = newOffset; }
	virtual void updateAimDirection(VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual bool containsCollider(Collider* collider) const = 0;
	virtual const std::vector<Collider*> getColliders() const = 0;
	virtual const std::vector<RectF> getRects() const = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }


protected:
	RectF mRect;

	VectorF mDirection;

	bool mAttacking;
	bool mOverrideCursorControl;
	
	bool mCanPlayHitSound;
	const BasicString* mAudioToPlay;
	const BasicString* mAudioToStop;
};