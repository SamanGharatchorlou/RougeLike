#pragma once

struct WeaponData;
class Collider;
class Effect;


class Weapon
{
public:
	Weapon() : mOverrideCursorControl(false), mAttacking(false) { }
	virtual ~Weapon() { };

	virtual void attack() = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;

	virtual bool canPlayHitSound() { return mCanPlayHitSound; }
	virtual const std::string& hitSoundLabel() = 0;
	virtual const std::string& missSoundLabel() = 0;

	bool isAttacking() const { return mAttacking; }
	virtual bool didHit() const = 0;

	virtual RectF rect() const { return mRect.Translate(mOffset); }
	virtual void setPosition(VectorF position) { mRect.SetBotCenter(position); }
	
	virtual void leftFlip() { mOffset.x = std::abs(mOffset.x) * -1.0f; }
	virtual void rightFlip() { mOffset.x = std::abs(mOffset.x); }


	virtual void setOffset(VectorF offset) { mOffset = offset; }
	virtual void updateAimDirection(VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual const std::vector<Collider*> getColliders() = 0;
	virtual const std::vector<RectF> getRects() const = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }

	//virtual void addEffect(Effect* effect) = 0;
	//virtual void clearEffect() = 0;

protected:
	RectF mRect;
	VectorF mOffset;

	VectorF mDirection;

	bool mAttacking;
	bool mOverrideCursorControl;
	
	bool mCanPlayHitSound;
	const std::string* mAudioToPlay;
	const std::string* mAudioToStop;
};