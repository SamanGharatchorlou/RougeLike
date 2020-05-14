#include "pch.h"
#include "Actor.h"

#include "Game/GameData.h"
#include "Collisions/Collider.h"
#include "Graphics/TextureManager.h"

#include "Game/Camera.h"
#include "System/Files/AnimationReader.h"

#include "Objects/Properties/PropertyBag.h"


Actor::Actor(GameData* gameData) : mGameData(gameData), mEffects(this)
{
	
}


Actor::~Actor()
{
	delete mPropertyBag;
	delete mCollider;
}


void Actor::init(const std::string& characterConfig)
{
	// Setup animations
	initAnimations(characterConfig);

	ASSERT(Warning, mPropertyBag != nullptr, "The property bag has not been set yet\n");
	mPhysics.init(getPropertyValue("Force"), getPropertyValue("MaxVelocity"));
}


void Actor::fastUpdate(float dt)
{
	// enemy needs reset here
	mPhysics.fastUpdate(dt);
	mEffects.fastUpdate(dt);
}


void Actor::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mEffects.slowUpdate(dt);
}


void Actor::render()
{
	RectF rect = renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

	mAnimator.getSpriteTile()->render(rect, mPhysics.flip());
}

void Actor::reset()
{
	mPhysics.reset();
	mAnimator.clear();
	mEffects.clear();
	mPropertyBag->resetProperties();
}



float Actor::getPropertyValue(const std::string& property) const 
{ 
	return mPropertyBag->value(property); 
}

Property* Actor::getProperty(const std::string& property) const
{
	return mPropertyBag->get(property);
}

void Actor::addEffect(Effect* effect)
{
	mEffects.addEffect(effect);
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