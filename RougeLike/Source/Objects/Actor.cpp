#include "pch.h"
#include "Actor.h"

#include "Game/Camera.h"
#include "System/Files/AnimationReader.h"

#include "Properties/PropertyBag.h"


void Actor::init(const std::string& characterConfig)
{
	// Setup animations
	initAnimations(characterConfig);
}


void Actor::fastUpdate(float dt)
{
	// enemy needs reset here
	mPhysics.fastUpdate(dt);
	mAnimator.fastUpdate(dt);

	if (mPhysics.isMoving())
		mAnimator.setSpeedFactor(mPhysics.relativeSpeed());
	else
		mAnimator.setSpeedFactor(1.0f);
}


void Actor::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

}


void Actor::render()
{
	RectF rect = renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

	printf("left point %f\n", rect.LeftPoint());
	mAnimator.getSpriteTile()->render(rect, mPhysics.flip());
}



float Actor::getPropertyValue(const std::string& property) const 
{ 
	return mPropertyBag->value(property); 
}

/// --- Private Functions --- ///

void Actor::initAnimations(const std::string& config)
{
	// config reader
	AnimationReader reader(config);

	// Setup sprite sheet
	TilesetData spriteSheetData = reader.readTilesetData(mGameData->textureManager);
	Tileset spriteSheet(spriteSheetData);

	// Setup animations
	Animations animationData = reader.readAnimationData();
	mAnimator.init(spriteSheet, animationData);
}