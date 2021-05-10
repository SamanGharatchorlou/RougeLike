#include "pch.h"
#include "MeleeWeapon.h"

#include "Game/Camera/Camera.h"
#include "Graphics/Texture.h"

#include "Objects/Pools/EffectPool.h"

#if DEBUG_CHECK
#include "Debug/DebugDraw.h"
#endif


MeleeWeapon::MeleeWeapon() : mSwingDirection(-1), mRotationSum(0.0) { }
MeleeWeapon::~MeleeWeapon() { }


void MeleeWeapon::equipt(const WeaponData* data)
{
	mMeleeData.copy(data);

	VectorF baseSize = mMeleeData.texture->originalDimentions;
	VectorF size = realiseSize(baseSize, mMeleeData.maxDimention);
	mRect.SetSize(size);

	mQuad = Quad2D<float>(mRect);
	mCollider.init(&mQuad);

	mAboutPoint = VectorF(rect().Width() / 2.0f, rect().Height() * 0.85f);

	mCooldown.set(mMeleeData.effectData.at(PropertyType::Cooldown));
	mCooldown.begin();
}

bool MeleeWeapon::canAttack()
{
	return mCooldown.hasCompleted();
}

std::vector<Effect*> MeleeWeapon::getEffects(EffectPool* effectPool)
{
	std::vector<Effect*> effects;

	mMeleeData.effectData.addXYPosition(mRect.Center());

	effects.push_back(effectPool->getObject(EffectType::Damage));
	effects.push_back(effectPool->getObject(EffectType::Displacement));

	for (Effect* effect : effects)
	{
		effect->fill(mMeleeData.effectData);
	}

	return effects;
}


VectorF& MeleeWeapon::offset()
{
	return mMeleeData.offset;
}


void MeleeWeapon::attack()
{
	mAttacking = true;
	mRotationSum = 0.0f;
	mCooldown.begin();
}


void MeleeWeapon::fastUpdate(float dt)
{
	if (mAttacking)
	{
		if (mRotationSum > mMeleeData.swingAngle)
			endAttack();
		else
			continueAttack(dt);
	}
}



void MeleeWeapon::updateAimDirection(VectorF cursorPosition)
{
	if (!mAttacking)
	{
		VectorF botPoint = rect().TopLeft() + mAboutPoint;
		mDirection = (cursorPosition - Camera::Get()->toCameraCoords(botPoint));

		float offsetAngle = (mMeleeData.swingAngle / 2.0f) * mSwingDirection;
		mDirection = rotateVector(mDirection, offsetAngle);
	}
}


void MeleeWeapon::render()
{
#if TRACK_COLLISIONS
	mCollider.renderCollider();
#endif
#if DRAW_PLAYER_WEAPON_RECT
	VectorF about = rect().TopCenter() + VectorF(0.0f, rect().Height() * 0.85f);
	debugDrawPoint(about, 10.0f, RenderColour::Black);

#if !TRACK_COLLISIONS
	debugDrawQuad(mQuad, RenderColour::Blue);
#endif
#endif

	double rotation = getRotation(mDirection) + 90.0f;
	SDL_RendererFlip flip = (rotation >= 0.0f && rotation < 180.0f) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	mMeleeData.texture->render(Camera::Get()->toCameraCoords(rect()), rotation, mAboutPoint, flip);
}


// Audio
const BasicString& MeleeWeapon::missSoundLabel() { return mMeleeData.audioMiss(); };
const BasicString& MeleeWeapon::hitSoundLabel() { return mMeleeData.audioHit(); };

std::vector<Collider*> MeleeWeapon::getColliders() 
{ 
	return std::vector<Collider*>{ &mCollider };
}



/// --- Private Functions --- ///
void MeleeWeapon::continueAttack(float dt)
{
	float theta = mMeleeData.swingSpeed * dt;
	mRotationSum += theta;
	mDirection = rotateVector(mDirection, theta * -mSwingDirection);

	updateQuadCollider();
}


void MeleeWeapon::updateQuadCollider()
{
	RectF weaponRect = rect();
	VectorF about = weaponRect.TopCenter() + VectorF(0.0f, weaponRect.Height() * 0.85f);

	mQuad = Quad2D<float>(weaponRect);
	mQuad.rotate(getRotation(mDirection) + 90.0f, about);
}



void MeleeWeapon::endAttack()
{
	mAttacking = false;
	flipSide();
}

