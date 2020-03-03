#pragma once


class Camera;
struct WeaponData;
class Collider;
class PlayerPropertyBag;

class Weapon
{
public:
	virtual ~Weapon() { };

	virtual void updateAnchor(VectorF anchor) = 0;
	virtual void updatePommelToCursor(Camera* camera, VectorF cursorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual void updateStats(const PlayerPropertyBag* bag) = 0;

	virtual const std::vector<Collider*> getColliders() const = 0;
	virtual void setColliderActivite(bool isActive) = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }

	// Render relative to the camera
	virtual void render(Camera* camera) = 0;


protected:
	const WeaponData* mData;

	RectF mRect;

	bool mOverrideCursorControl;
};