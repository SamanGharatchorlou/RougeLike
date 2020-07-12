#include "pch.h"
#include "MeleeWeapon.h"

#include "Objects/Properties/PropertyBag.h"
#include "Weapons/WeaponData.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/EffectCollider.h"


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
		mBlockColliders.push_back(new EffectCollider);

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

	mSwingSpeed = mMeleeData->swingSpeed;
	mOffset = mMeleeData->offset;

	// Set size
	VectorF baseSize = mMeleeData->texture->originalDimentions;
	VectorF size = realiseSize(baseSize, mMeleeData->maxDimention);
	mRect.SetSize(size);

	mAboutPoint = VectorF(rect().Width() / 2.0f, rect().Height() * 0.85f);

	mCooldown = mMeleeData->cooldown;
	mCooldown.begin();
}


void MeleeWeapon::attack()
{
	if (mCooldown.completed())
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
	for (const Collider* collider : mBlockColliders)
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
		VectorF botPoint = rect().TopLeft() + mAboutPoint;

		mDirection = (cursorPosition - Camera::Get()->toCameraCoords(botPoint));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

	updateWeaponBlocks();
}


void MeleeWeapon::render()
{
	SDL_RendererFlip flip = (getRotation(mDirection) >= 0.0f && getRotation(mDirection) < 180.0f) ? SDL_FLIP_HORIZONTAL: SDL_FLIP_NONE;
	mMeleeData->texture->render(Camera::Get()->toCameraCoords(rect()), getRotation(mDirection), mAboutPoint, flip);

#if DRAW_PLAYER_RECTS
	// About point
	RectF about = RectF(rect().TopLeft() + aboutPoint, VectorF(2.0f, 2.0f));
	debugDrawRect(about, RenderColour::Blue);

	// Weapon vector
	VectorF pos = rect().TopLeft() + aboutPoint;
	debugDrawLine(pos, pos + weaponVectorTest , RenderColour::Red);
#endif
}


const std::vector<Collider*> MeleeWeapon::getColliders()
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
	VectorF weaponVector = mDirection.normalise() * mAboutPoint.y;
#if DRAW_PLAYER_RECTS
	weaponVectorTest = weaponVector;
#endif

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

	mCooldown.begin();
}