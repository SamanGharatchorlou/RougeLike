#include "pch.h"
#include "Weapon.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Game/Cursor.h"
#include "Characters/Attributes/Movement.h"

#include "Collisions/Collider.h"



Weapon::Weapon(GameData* gameData) : 
	mGameData(gameData)
	, mOverrideCursorControl(false)
	, mSwingDirection(-1)
{ 
	// 4 blocks seems to be a good number of blocks
	unsigned int blocks = 4;
	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockRects.push_back(RectF());
		mBlockColliders.push_back(new Collider);
	}

	for (unsigned int i = 0; i < blocks; i++)
	{
		mBlockColliders[i]->init(&mBlockRects[i]);
		mBlockColliders[i]->setActive(false);
	}
}


Weapon::~Weapon()
{
	for (Collider* collider : mBlockColliders)
	{
		delete collider;
	}

	mBlockRects.clear();
	mBlockColliders.clear();
}


void Weapon::equipt(const WeaponData* data)
{
	mData = data;

	for (Collider* collider : mBlockColliders)
	{
		collider->setDamage(mData->damage);
	}

	// TODO: where/what to do with this 1.5 scale factor
	mRect.SetSize(mData->texture->originalDimentions * 1.5f);
}



void Weapon::fastUpdate(VectorF anchorPosition)
{
	// Follow character
	mRect.SetTopLeft(anchorPosition + mData->pommeloffset);

	// Follow cursor
	if (!mOverrideCursorControl)
	{
		double offsetAngle = (mData->swingArc / 2.0) * mSwingDirection;

		// Camera to cursor vector
		mDirection = (mGameData->cursor->getPosition() - mGameData->camera->toCameraCoords(mRect.BotCenter()));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

	updateWeaponBlocks();
}


void Weapon::render()
{
	VectorF aboutPoint(mRect.Width() / 2.0f, mRect.Height());
	mData->texture->render(mGameData->camera->toCameraCoords(mRect), getRotation(mDirection), aboutPoint);
}


void Weapon::rotate(double theta)
{
	mDirection = rotateVector(mDirection, theta * -mSwingDirection);
}


// TODO: have this return blocks instead?
void Weapon::updateWeaponBlocks()
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

		VectorF shaftPosition = (weaponVector / (blocks)) * ( 0.5 + i);
		block = block.Translate(shaftPosition);

		// Translate to world coords
		block = block.Translate(mRect.BotCenter());

		mBlockRects[i].SetRect(block);
	}
}


void Weapon::setColliderActivite(bool isActive)
{
	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i]->setActive(isActive);
	}
}