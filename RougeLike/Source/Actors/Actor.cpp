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


Actor::Actor() : mVisibility(true) { }


void Actor::setCharacter(const XMLNode actorNode, const TextureManager* textures)
{
	mAttributeBag.readData(actorNode.child("Properties"));
	mEffects.fillEffectBag(actorNode.child("Effects"));

	// Animations
	AnimationReader reader;
	mAnimator = reader.buildAnimator(actorNode.child("Animator"), textures);
	mAnimator.start();

	// Physics 
	VectorF baseSize = mAnimator.frameSize();
	float maxDimention = actorNode.child("MaxSize").getFloat(); // added new calculations to animator, can i remove the realise size bit and just set the rect to (maxDim, maxDim). it should auto size.
	VectorF size = realiseSize(baseSize, maxDimention);
	mPhysics.rectRef().SetSize(size);
	updatePhysicsStats();

	// Collider
	VectorF colliderScale = getXYAttributes(actorNode.child("ColliderScale"));
	mCollider.init(&mPhysics.rectRef(), colliderScale);

#if _DEBUG // TODO remove this name bit from the collider??
	//mCollider.setName(FileManager::Get()->getItemName(parser.path));
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