#include "pch.h"
#include "MeleeWeapon.h"

#include "Weapons/WeaponData.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Characters/Player/PlayerPropertyBag.h"


MeleeWeapon::MeleeWeapon() : mSwingDirection(-1), mPlayerSwingSpeed(0.0f) 
{
	// 4 blocks seems to be a good number of blocks
	unsigned int blocks = 4;
	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockRects.push_back(RectF());
		mBlockColliders.push_back(new DamageCollider);
	}

	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockColliders[i]->init(&mBlockRects[i]);
		mBlockColliders[i]->setActive(false);
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
	mData = data;

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->setDamage(mData->damage);
	}

	// TODO: where/what to do with this 1.5 scale factor
	mRect.SetSize(mData->texture->originalDimentions * 1.5f);
}


void MeleeWeapon::updateStats(const PlayerPropertyBag* bag)
{
	mPlayerSwingSpeed = mData->swingSpeed * (1 + (bag->pAttackSpd.get() / 100));

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->setDamage(mData->damage + bag->pAttackDmg.get());
	}
}


// Follow character
void MeleeWeapon::updateAnchor(VectorF anchor)
{
	mRect.SetTopLeft(anchor + mData->pommeloffset);
}


void MeleeWeapon::updatePommelToCursor(Camera* camera, VectorF cursorPosition)
{
	// Follow cursor
	if (!mOverrideCursorControl)
	{
		double offsetAngle = (mData->swingArc / 2.0) * mSwingDirection;

		// Camera to cursor vector
		mDirection = (cursorPosition - camera->toCameraCoords(mRect.BotCenter()));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

	updateWeaponBlocks();
}


void MeleeWeapon::render(Camera* camera)
{
	VectorF aboutPoint(mRect.Width() / 2.0f, mRect.Height());
	mData->texture->render(camera->toCameraCoords(mRect), getRotation(mDirection), aboutPoint);
}


void MeleeWeapon::rotate(double theta)
{
	mDirection = rotateVector(mDirection, theta * -mSwingDirection);
}


// TODO: have this return blocks instead?
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


const std::vector<Collider*> MeleeWeapon::getColliders() const
{
	std::vector<Collider*> colliders;

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		colliders.push_back(mBlockColliders[i]);
	}

	return colliders;
}


void MeleeWeapon::setColliderActivite(bool isActive)
{
	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->setActive(isActive);
	}
}


const double MeleeWeapon::maxSwingAngle() const 
{ 
	return mData->swingArc; 
}