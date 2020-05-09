#include "pch.h"
#include "Actor.h"

#include "Game/Camera.h"
#include "System/Files/AnimationReader.h"


void Actor::init(const std::string& characterConfig)
{
	// Setup animations
	initAnimations(characterConfig);
}


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


void Actor::render()
{
	RectF rect = renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

	printf("left point %f\n", rect.LeftPoint());
	mAnimator.getSpriteTile()->render(rect, mPhysics.flip());
}