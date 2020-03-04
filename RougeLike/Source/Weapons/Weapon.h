#pragma once


class Camera;
struct WeaponData;
class Collider;
class PlayerPropertyBag;


class Weapon
{
public:
	virtual ~Weapon() { };

	virtual void attack() = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void updateAnchor(VectorF anchor) = 0;
	virtual void updateAimDirection(Camera* camera, VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual void updateStats(const PlayerPropertyBag* bag) = 0;

	virtual const std::vector<Collider*> getColliders() const = 0;
	virtual void setColliderActivite(bool isActive) = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }

	// Render relative to the camera
	virtual void render(Camera* camera) = 0;


protected:
	RectF mRect;

	VectorF mDirection;

	bool mOverrideCursorControl;
};