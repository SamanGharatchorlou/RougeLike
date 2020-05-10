#pragma once

struct WeaponData;
class Collider;
class PlayerPropertyBag;


class Weapon
{
public:
	Weapon() : mOverrideCursorControl(false), mAttacking(false) { }
	virtual ~Weapon() { };

	virtual void attack() = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;

	virtual bool canPlayHitSound() { return mCanPlayHitSound; }
	virtual const std::string& hitSoundLabel() = 0;
	virtual const std::string& missSoundLabel() = 0;

	bool isAttacking() const { return mAttacking; }
	virtual bool didHit() const = 0;

	virtual void updateAnchor(VectorF anchor) = 0;
	virtual void updateAimDirection(VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual const std::vector<Collider*> getColliders() = 0;
	virtual const std::vector<RectF> getRects() const = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }

	virtual void render() = 0;


protected:
	RectF mRect;
	VectorF mDirection;

	bool mAttacking;
	bool mOverrideCursorControl;

	bool mCanPlayHitSound;
	const std::string* mAudioToPlay;
	const std::string* mAudioToStop;
};