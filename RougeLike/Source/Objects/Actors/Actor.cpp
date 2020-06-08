#include "pch.h"
#include "Actor.h"

#include "Game/GameData.h"
#include "Collisions/Collider.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Game/Camera.h"
#include "Animations/AnimationReader.h"

#include "Objects/Properties/PropertyBag.h"


Actor::Actor(GameData* gameData) : mGameData(gameData), mEffects(this), mVisibility(true) { }


Actor::~Actor()
{
	delete mPropertyBag;
	delete mCollider;
}


void Actor::init(XMLParser& parser)
{
	// Animations
	AnimationReader reader(mGameData->textureManager, parser);
	reader.initAnimator(mAnimator);
	mAnimator.start();

	// Physics
	VectorF baseSize = mAnimator.frameSize();
	float maxDimention = std::stof(parser.firstRootNodeValue("MaxSize"));
	VectorF size = realiseSize(baseSize, maxDimention);
	mPhysics.setRect(RectF(VectorF(), size));

	// Collider
	ASSERT(Warning, mCollider != nullptr, "Collider must be created before being set\n");
	Attributes attributes = parser.attributes(parser.rootNode()->first_node("ColliderScale"));
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	mCollider->init(&mPhysics.rectRef(), VectorF(x, y));

	// Properties
	ASSERT(Warning, mPropertyBag != nullptr, "The property bag has not been set yet\n");
	mPhysics.init(getPropertyValue("Force"), getPropertyValue("MaxVelocity"));
}

void Actor::updatePhysicsStats()
{
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
	RectF renderRect = Camera::Get()->toCameraCoords(rect());
	mAnimator.render(renderRect, mPhysics.flip());
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

bool Actor::hasProperty(const std::string& property) const
{
	return mPropertyBag->contains(property);
}

void Actor::addEffect(Effect* effect)
{
	mEffects.addEffect(effect);
}


RectF Actor::scaledRect() const 
{ 
	return mCollider->scaledRect(); 
}