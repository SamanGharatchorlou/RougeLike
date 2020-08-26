#include "pch.h"
#include "MeleeWeapon.h"

#include "Game/Camera/Camera.h"
#include "Graphics/Texture.h"

#include "Weapons/WeaponData.h"
#include "Collisions/Colliders/EffectCollider.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


MeleeWeapon::MeleeWeapon() :
	mSwingDirection(-1),
	mRotationSum(0.0)
{
	// 4 blocks seems to be a good number of blocks
	unsigned int blocks = 4;
	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockRects.push_back(RectF());
		mBlockColliders.push_back(new EffectCollider);

		mBlockColliders[i]->init(&mBlockRects[i]);
	}
}


MeleeWeapon::~MeleeWeapon()
{
	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		delete mBlockColliders[i];
	}

	mBlockRects.clear();
	mBlockColliders.clear();
}


VectorF& MeleeWeapon::offset() { return mData.offset; }

void MeleeWeapon::equipt(const WeaponData* data)
{
	mData.copy(data);

	// Set size
	VectorF baseSize = mData.texture->originalDimentions;
	VectorF size = realiseSize(baseSize, mData.maxDimention);
	mRect.SetSize(size);

	mAboutPoint = VectorF(rect().Width() / 2.0f, rect().Height() * 0.85f);

	mData.cooldown.begin();
}


void MeleeWeapon::attack()
{
	if (mData.cooldown.hasCompleted())
	{
		mAttacking = true;
		overrideCursorControl(true);
		mCanPlayHitSound = true;
	}
}


void MeleeWeapon::fastUpdate(float dt)
{
	if (mAttacking)
	{
		if (mRotationSum > mData.swingAngle)
			endAttack();
		else
			continueAttack(dt);

		updateWeaponBlocks();
	}
}



bool MeleeWeapon::didHit() const
{
	for (const Collider* collider : mBlockColliders)
	{
		if (collider->didHit())
			return true;
	}

	return false;
}


bool MeleeWeapon::containsCollider(Collider* collider) const
{
	for (int i = 0; i < mBlockColliders.size(); i++)
	{
		if (collider == mBlockColliders[i])
			return true;
	}

	return false;
}


// Follow character
void MeleeWeapon::updateAimDirection(VectorF cursorPosition)
{
	// Follow cursor
	if (!mOverrideCursorControl)
	{
		float offsetAngle = (mData.swingAngle / 2.0f) * mSwingDirection;

		// Camera to cursor vector
		VectorF botPoint = rect().TopLeft() + mAboutPoint;

		mDirection = (cursorPosition - Camera::Get()->toCameraCoords(botPoint));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

}


void MeleeWeapon::render()
{
	SDL_RendererFlip flip = (getRotation(mDirection) >= 0.0f && getRotation(mDirection) < 180.0f) ? SDL_FLIP_HORIZONTAL: SDL_FLIP_NONE;
	mData.texture->render(Camera::Get()->toCameraCoords(rect()), getRotation(mDirection), mAboutPoint, flip);

#if DRAW_PLAYER_RECTS
	// About point
	RectF about = RectF(rect().TopLeft() + mAboutPoint, VectorF(2.0f, 2.0f));
	debugDrawRect(about, RenderColour::Blue);

	// Weapon vector
	VectorF pos = rect().TopLeft() + mAboutPoint;
	VectorF weaponVector = mDirection.normalise() * mAboutPoint.y;
	debugDrawLine(pos, pos + weaponVector, RenderColour::Red);

	// block colliders
	debugDrawRects(mBlockRects, RenderColour(RenderColour::Yellow));
#endif
}


const std::vector<Collider*> MeleeWeapon::getColliders() const
{
	std::vector<Collider*> colliders;

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		colliders.push_back(mBlockColliders[i]);
	}

	return colliders;
}


void MeleeWeapon::addEffect(Effect* effect)
{
	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->addEffect(effect);
	}
}

bool MeleeWeapon::hasEffects() const
{
	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		if (!mBlockColliders[i]->hasEffects())
			return false;
	}

	return true;
}


// Audio
const BasicString& MeleeWeapon::missSoundLabel() { return mData.audioMiss(); };
const BasicString& MeleeWeapon::hitSoundLabel() 
{ 
	mCanPlayHitSound = false;
	return mData.audioHit();
};


/// --- Private Functions --- ///
void MeleeWeapon::updateWeaponBlocks()
{
	VectorF weaponVector = mDirection.normalise() * mAboutPoint.y;

	// Size
	int blocks = mBlockRects.size();
	float blockWidth = std::max(std::abs(weaponVector.x) / blocks, rect().Width());
	float blockHeight = std::max(std::abs(weaponVector.y) / blocks, rect().Width());
	VectorF blockSize = VectorF(blockWidth, blockHeight);

	// Position
	for (int i = 0; i < blocks; i++)
	{
		RectF block = RectF(blockSize / -2.0f, blockSize);

		VectorF shaftPosition = (weaponVector / (blocks)) * (i + 0.5f);
		block = block.Translate(shaftPosition);

		// Translate to world coords
		block = block.Translate(rect().TopLeft() + mAboutPoint);

		mBlockRects[i].SetRect(block);
	}
}




void MeleeWeapon::continueAttack(float dt)
{
	float theta = mData.swingSpeed * dt;
	mRotationSum += theta;

	if (mRotationSum > mData.swingAngle)
		theta = mData.swingAngle - mRotationSum;

	mDirection = rotateVector(mDirection, theta * -mSwingDirection);
}


void MeleeWeapon::endAttack()
{
	mRotationSum = 0.0f;
	mAttacking = false;

	flipSide();
	overrideCursorControl(false);
	mCanPlayHitSound = false;

	mData.cooldown.begin();
}