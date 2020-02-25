#pragma once

#include "Swing.h"
#include "WeaponData.h"

struct GameData;
class Collider;

class Weapon
{
public:
	Weapon(GameData* gameData);
	~Weapon();

	void fastUpdate(VectorF anchorPosition);

	void equipt(const WeaponData* data);

	void rotate(double theta);
	const double getAngle() const { return getRotation(mDirection); }
	const double maxSwingAngle() const { return mData->swingArc; }
	const double swingSpeed() const { return mData->swingSpeed; }

	void setScale(float scale);

	const std::vector<Collider*> getColliders() const { return mBlockColliders; }
	const std::vector<RectF> getRects() const { return mBlockRects; }

	void setColliderActivite(bool isActive);

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }
	void flipSide() { mSwingDirection *= -1; }

	void render();


private:
	void updateWeaponBlocks();


private:
	GameData* mGameData;

	const WeaponData* mData;

	std::vector<RectF> mBlockRects;
	std::vector<Collider*> mBlockColliders;

	RectF mRect;
	VectorF mDirection;
	int mSwingDirection;

	bool mOverrideCursorControl;
};