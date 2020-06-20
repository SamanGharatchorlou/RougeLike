#include "pch.h"
#include "Actor.h"

#include "Game/GameData.h"
#include "Collisions/EffectCollider.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Game/Camera.h"
#include "Animations/AnimationReader.h"

#include "Map/Map.h"
#include "Map/Environment.h"


Actor::Actor(GameData* gameData) 
	: mGameData(gameData), 
	mEffects(gameData->effectPool, this), mVisibility(true) { }


Actor::~Actor()
{

}


void Actor::init(const std::string& config)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, config));

	// Properties
	mPropertyBag.readProperties(config);

	// Animations
	AnimationReader reader(mGameData->textureManager, parser);
	reader.initAnimator(mAnimator);
	mAnimator.start();

	// Physics
	VectorF baseSize = mAnimator.frameSize();
	float maxDimention = std::stof(parser.firstRootNodeValue("MaxSize"));
	VectorF size = realiseSize(baseSize, maxDimention);
	mPhysics.setRect(RectF(VectorF(), size));
	mPhysics.init(getPropertyValue("Force"), getPropertyValue("MaxVelocity"));

	// Collider
	Attributes attributes = parser.attributes(parser.rootNode()->first_node("ColliderScale"));
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	mCollider.init(&mPhysics.rectRef(), VectorF(x, y));

#if _DEBUG
	mCollider.setName(FileManager::Get()->getItemName(parser.path));
#endif

	// Effects
	mEffectProperties.readProperties(config);
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

	mEffects.render();
}


void Actor::reset()
{
	mPhysics.reset();
	mAnimator.clear();
	mEffects.clear();
	mPropertyBag.resetProperties();
}


float Actor::getPropertyValue(const std::string& property) const 
{ 
	return mPropertyBag.value(property); 
}


Property* Actor::getProperty(const std::string& property) const
{
	return mPropertyBag.get(property);
}


bool Actor::hasProperty(const std::string& property) const
{
	return mPropertyBag.contains(property);
}


void Actor::addEffect(Effect* effect)
{
	mEffects.addEffect(effect);
}


void Actor::processEffects(EffectCollider* effectCollider)
{
	while (effectCollider->hasEffects())
	{
		mEffects.addEffect(effectCollider->popEffect());
	}
}


RectF Actor::scaledRect() const 
{ 
	return mCollider.scaledRect(); 
}


const Map* Actor::currentMap() const
{
	return mGameData->environment->map(position());
}