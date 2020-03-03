#pragma once

struct GameData;
struct WeaponData;
class Collider;
class PlayerPropertyBag;

class Weapon
{
public:
	virtual ~Weapon() { };

	virtual void fastUpdate(VectorF anchorPosition) = 0;

	virtual void equipt(const WeaponData* data) = 0;

	virtual void updateStats(const PlayerPropertyBag* bag) = 0;

	virtual const std::vector<Collider*> getColliders() const = 0;
	virtual void setColliderActivite(bool isActive) = 0;

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }

	virtual void render() = 0;


protected:
	GameData* mGameData; // TODO: remove this

	const WeaponData* mData;

	RectF mRect;

	bool mOverrideCursorControl;
};