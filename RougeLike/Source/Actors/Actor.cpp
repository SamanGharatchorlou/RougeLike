#include "pch.h"
#include "Actor.h"

#include "Collisions/EffectCollider.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"
#include "Objects/Effects/EffectPool.h"

#include "Game/Camera.h"
#include "Animations/AnimationReader.h"

#include "Map/Map.h"
#include "Map/Environment.h"

#include "Objects/Effects/EffectTypes/Effect.h"


// TEMP
#include "Objects/Properties/EffectBag.h"


Actor::Actor() : mVisibility(true) { }


void Actor::setCharacter(const XMLParser& parser, const TextureManager* textureManager)
{
	mAttributeBag.readData(parser, "Properties");
	mEffects.fillEffectBag(parser.root().first("Effects"));

	// Animations
	AnimationReader reader(textureManager, parser);
	reader.initAnimator(mAnimator);
	mAnimator.start();

	// Physics 
	VectorF baseSize = mAnimator.frameSize();
	float maxDimention = atof(parser.firstRootNodeValue("MaxSize").c_str()); // added new calculations to animator, can i remove the realise size bit and just set the rect to (maxDim, maxDim). it should auto size.
	VectorF size = realiseSize(baseSize, maxDimention);
	mPhysics.rectRef().SetSize(size);
	updatePhysicsStats();

	// Collider
	Attributes attributes = parser.attributes(parser.rootNode()->first_node("ColliderScale"));
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	mCollider.init(&mPhysics.rectRef(), VectorF(x, y));

#if _DEBUG
	mCollider.setName(FileManager::Get()->getItemName(parser.path));
#endif
}


void Actor::set(Environment* environment) 
{ 
	mEnvironment = environment;
	mEffects.init(environment->effectPool());
}


void Actor::updatePhysicsStats()
{
	mPhysics.init(getAttributeValue(AttributeType::Force), getAttributeValue(AttributeType::MaxVelocity));
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
	//mEffects.clear();
	//mPropertyBag.resetProperties(); // TODO: need to clear properties?
}


float Actor::getAttributeValue(AttributeType type) const
{ 
	return mAttributeBag.value(type);
}


Attribute* Actor::getAttribute(AttributeType type) const
{
	return mAttributeBag.get(type);
}


bool Actor::hasAttribute(AttributeType type) const
{
	return mAttributeBag.contains(type);
}


void Actor::addEffect(Effect* effect)
{
	effect->setReceiver(this);
	mEffects.addReceivedEffect(effect);
}

void Actor::handleEffects(EffectCollider* effectCollider)
{
	while (effectCollider->hasEffects())
	{
		addEffect(effectCollider->popEffect());
	}
}


RectF Actor::scaledRect() const 
{ 
	return mCollider.scaledRect(); 
}


const Map* Actor::currentMap() const
{
	return mEnvironment->map(position());
}