#include "pch.h"
#include "MeleeWeapon.h"

#include "Objects/Actors/Player/PlayerPropertyBag.h"
#include "Weapons/WeaponData.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"


MeleeWeapon::MeleeWeapon() : 
	mMeleeData(nullptr),
	mSwingDirection(-1), 
	mSwingSpeed(0.0f), 
	mRotationSum(0.0)
{
	// 4 blocks seems to be a good number of blocks
	unsigned int blocks = 4;

	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockRects.push_back(RectF());
		mBlockColliders.push_back(new DamageCollider);

		mBlockColliders[i]->init(&mBlockRects[i]);

#if _DEBUG
		mBlockColliders[i]->setName("weapon");
#endif
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


void MeleeWeapon::equipt(const WeaponData* data)
{
	mMeleeData = static_cast<const MeleeWeaponData*>(data);

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->initDamage(mMeleeData->damage, mMeleeData->knockbackDistance);
	}

	mSwingSpeed = mMeleeData->swingSpeed;
	mOffset = mMeleeData->offset;

	// Set size
	VectorF baseSize = mMeleeData->texture->originalDimentions;
	VectorF size = realiseSize(baseSize, mMeleeData->maxDimention);
	mRect.SetSize(size);
}


void MeleeWeapon::attack()
{
	mAttacking = true;
	overrideCursorControl(true);
	mCanPlayHitSound = true;
}


void MeleeWeapon::fastUpdate(float dt)
{
	if (mAttacking)
	{
		if (mRotationSum > maxSwingAngle())
		{
			endAttack();
		}
		else
		{
			continueAttack(dt);
		}
	}
}


bool MeleeWeapon::didHit() const
{
	for (const DamageCollider* collider : mBlockColliders)
	{
		if (collider->didHit())
		{
			return true;
		}
	}

	return false;
}


// Follow character
void MeleeWeapon::updateAimDirection(VectorF cursorPosition)
{
	// Follow cursor
	if (!mOverrideCursorControl)
	{
		float offsetAngle = (mMeleeData->swingArc / 2.0f) * mSwingDirection;

		// Camera to cursor vector
		mDirection = (cursorPosition - Camera::Get()->toCameraCoords(rect().BotCenter()));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

	updateWeaponBlocks();
}


void MeleeWeapon::render()
{
	VectorF aboutPoint(rect().Width() / 2.0f, rect().Height() * 0.9f);

	SDL_RendererFlip flip = (getRotation(mDirection) >= 0.0f && getRotation(mDirection) < 180.0f) ? SDL_FLIP_HORIZONTAL: SDL_FLIP_NONE;
	mMeleeData->texture->render(Camera::Get()->toCameraCoords(rect()), getRotation(mDirection), aboutPoint, flip);
}


const std::vector<Collider*> MeleeWeapon::getColliders()
{
	std::vector<Collider*> colliders;

	if (mAttacking)
	{
		for (unsigned int i = 0; i < mBlockColliders.size(); i++)
		{
			colliders.push_back(mBlockColliders[i]);
		}
	}

	return colliders;
}


const float MeleeWeapon::maxSwingAngle() const
{ 
	return mMeleeData->swingArc;
}

const float MeleeWeapon::knockbackDistance() const
{
	return mMeleeData->knockbackDistance;
}


// Audio
const std::string& MeleeWeapon::hitSoundLabel() 
{ 
	mCanPlayHitSound = false;
	return mMeleeData->audioHit;
};
const std::string& MeleeWeapon::missSoundLabel() { return mMeleeData->audioMiss; };


/// --- Private Functions --- ///
void MeleeWeapon::updateWeaponBlocks()
{
	VectorF weaponVector = mDirection.normalise() * rect().Height();

	// Size
	int blocks = mBlockRects.size();
	float blockWidth = std::max(std::abs(weaponVector.x) / blocks, rect().Width());
	float blockHeight = std::max(std::abs(weaponVector.y) / blocks, rect().Width());
	VectorF blockSize = VectorF(blockWidth, blockHeight);

	// Position
	for (int i = 0; i < blocks; i++)
	{
		// Center block on weapon aboutpoint (pommel) coords
		RectF block = RectF(blockSize / -2, blockSize);

		VectorF shaftPosition = (weaponVector / (blocks)) * (0.5 + i);
		block = block.Translate(shaftPosition);

		// Translate to world coords
		block = block.Translate(rect().BotCenter());

		mBlockRects[i].SetRect(block);
	}
}


void MeleeWeapon::continueAttack(float dt)
{
	float theta = mSwingSpeed * dt;
	mRotationSum += theta;

	if (mRotationSum > maxSwingAngle())
		theta = maxSwingAngle() - mRotationSum;

	mDirection = rotateVector(mDirection, theta * -mSwingDirection);
}


void MeleeWeapon::endAttack()
{
	mRotationSum = 0.0f;
	mAttacking = false;

	flipSide();
	overrideCursorControl(false);
	mCanPlayHitSound = false;
}