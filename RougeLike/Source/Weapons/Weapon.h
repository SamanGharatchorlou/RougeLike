#pragma once

struct WeaponData;
class Collider;
class PlayerPropertyBag;


class Weapon
{
public:
	virtual ~Weapon() { };

	virtual void attack() = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;

	virtual void updateAnchor(VectorF anchor) = 0;
	virtual void updateAimDirection(VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual void updateStats(const PlayerPropertyBag* bag) = 0;

	virtual const std::vector<Collider*> getColliders() = 0;
	virtual const std::vector<RectF> getRects() const = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }

	virtual void render() = 0;


protected:
	RectF mRect;

	VectorF mDirection;

	bool mOverrideCursorControl;
};