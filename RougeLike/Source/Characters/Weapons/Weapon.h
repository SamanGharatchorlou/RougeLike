#pragma once

#include "Swing.h"
#include "Characters/Attributes/Damage.h"
#include "Collisions/Collider.h"

struct GameData;
class Texture;
class Movement;

class Weapon
{
public:
	Weapon(GameData* gameData);
	~Weapon() { }

	void fastUpdate(VectorF anchorPosition);

	void selectWeapon(std::string weaponName);

	void rotate(double theta);
	const double getAngle() { return getRotation(mDirection); }
	const double getMaxSwingAngle() { return mSwing.getAngle(); }
	const double getSwingSpeed() { return mSwing.getSpeed(); }

	void setScale(float scale);
	VectorF getPositionCenter() { return mRect.Center(); }

	Damage getDamage() { return mDamage; }

	std::vector<Collider*> getColliders();
	std::vector<RectF> getRects() { return mBlockRects; }

	void setColliderActivite(bool isActive);

	void overrideCursorControl(bool overrideControl) { mOverrideCursorControl = overrideControl; }
	void flipSide() { mSwing.flipSide(); }

	void render();


private:
	void loadWeaponData(std::string weaponName);
	void updateWeaponBlocks();


private:
	GameData* mGameData;
	Texture* mTexture;

	std::vector<RectF> mBlockRects;
	std::vector<Collider> mBlockColliders;

	RectF mRect;
	VectorF mPommeloffset;

	VectorF mDirection;
	Swing mSwing;
	Damage mDamage;

	bool mOverrideCursorControl;
};