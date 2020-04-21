#include "pch.h"
#include "MeleeWeapon.h"

#include "Characters/Player/PlayerPropertyBag.h"
#include "Weapons/WeaponData.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"



MeleeWeapon::MeleeWeapon() : mSwingDirection(-1), mSwingSpeed(0.0f), mRotationSum(0.0), mAttacking(false)
{
	// 4 blocks seems to be a good number of blocks
	unsigned int blocks = 4;

	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockRects.push_back(RectF());
		mBlockColliders.push_back(new DamageCollider);

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


void MeleeWeapon::equipt(const WeaponData* data)
{
	mData = static_cast<const MeleeWeaponData*>(data);

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->set(mData->damage, mData->knockbackDistance);
	}

	mSwingSpeed = mData->swingSpeed;

	// TODO: where/what to do with this 1.5 scale factor
	mRect.SetSize(mData->texture->originalDimentions * 1.5f);
}


void MeleeWeapon::updateStats(const PlayerPropertyBag* bag)
{
	mSwingSpeed = mData->swingSpeed * (1 + (bag->pAttackSpd.get() / 100));

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->set(mData->damage + bag->pAttackDmg.get(), mData->knockbackDistance);
	}
}


void MeleeWeapon::attack()
{
	mAttacking = true;
	overrideCursorControl(true);
}


void MeleeWeapon::fastUpdate(float dt)
{
	if (mAttacking)
	{
		if (mRotationSum > maxSwingAngle())
		{
			mRotationSum = 0.0f;
			mAttacking = false;

			flipSide();
			overrideCursorControl(false);
		}
		else
		{
			float theta = mSwingSpeed * dt;
			mRotationSum += theta;

			if (mRotationSum > maxSwingAngle())
				theta = maxSwingAngle() - mRotationSum;

			mDirection = rotateVector(mDirection, theta * -mSwingDirection);
		}
	}
}


// Follow character
void MeleeWeapon::updateAnchor(VectorF anchor)
{
	mRect.SetTopLeft(anchor + mData->handleOffset);
}


void MeleeWeapon::updateAimDirection(VectorF cursorPosition)
{
	// Follow cursor
	if (!mOverrideCursorControl)
	{
		float offsetAngle = (mData->swingArc / 2.0f) * mSwingDirection;

		// Camera to cursor vector
		mDirection = (cursorPosition - Camera::Get()->toCameraCoords(mRect.BotCenter()));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

	updateWeaponBlocks();
}


void MeleeWeapon::render()
{
	VectorF aboutPoint(mRect.Width() / 2.0f, mRect.Height());
	mData->texture->render(Camera::Get()->toCameraCoords(mRect), getRotation(mDirection), aboutPoint);
}


void MeleeWeapon::updateWeaponBlocks()
{
	VectorF weaponVector = mDirection.normalise() * mRect.Height();

	int blocks = mBlockRects.size();
	float blockWidth = std::max(std::abs(weaponVector.x) / blocks, mRect.Width());
	float blockHeight = std::max(std::abs(weaponVector.y) / blocks, mRect.Width());
	VectorF blockSize = VectorF(blockWidth, blockHeight);

	for (int i = 0; i < blocks; i++)
	{
		// Center block on weapon aboutpoint (pommel) coords
		RectF block = RectF(blockSize / -2, blockSize);

		VectorF shaftPosition = (weaponVector / (blocks)) * (0.5 + i);
		block = block.Translate(shaftPosition);

		// Translate to world coords
		block = block.Translate(mRect.BotCenter());

		mBlockRects[i].SetRect(block);
	}
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
	return mData->swingArc; 
}

const float MeleeWeapon::knockbackDistance() const
{
	return mData->knockbackDistance;
}