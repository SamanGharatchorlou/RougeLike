#include "pch.h"
#include "Actor.h"

#include "Collisions/Colliders/EffectCollider.h"
#include "Objects/Pools/EffectPool.h"

#include "Game/Camera/Camera.h"
#include "Animations/AnimationReader.h"

#include "Map/Map.h"
#include "Game/Environment.h"

#include "Objects/Effects/EffectTypes/Effect.h"


Actor::Actor() : mVisibility(true) { }


void Actor::setCharacter(const XMLNode actorNode)
{
	mAttributeBag.fill(actorNode.child("Properties"));
	mEffects.fillEffectBag(actorNode.child("Effects"));

	// Animations
	AnimationReader reader;
	mAnimator = reader.buildAnimator(actorNode.child("Animator"));
	mAnimator.start();

	// Physics 
	VectorF baseSize = mAnimator.frameSize();
	float maxDimention = toFloat(actorNode.child("MaxSize").value()); // added new calculations to animator, can i remove the realise size bit and just set the rect to (maxDim, maxDim). it should auto size.
	VectorF size = realiseSize(baseSize, maxDimention);
	mPhysics.rectRef().SetSize(size);
	updatePhysicsStats();

	// Collider
	StringMap colliderScale = actorNode.child("ColliderScale").attributes();
	VectorF scale = colliderScale.getVector("x", "y");
	mCollider.init(&mPhysics.rectRef(), scale);

	XMLNode renderOffsetNode = actorNode.child("RenderOffset");
	if (renderOffsetNode)
	{
		StringMap renderOffset = renderOffsetNode.attributes();
		VectorF offset = renderOffset.getVector("x", "y");
		mRenderOffset = offset;
	}
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
	//mPhysics.move(mPhysics.movementDistance(dt));
	mEffects.fastUpdate(dt);
}


void Actor::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	mEffects.slowUpdate(dt);
}

void Actor::render(VectorF offset)
{
	RectF renderRect = Camera::Get()->toCameraCoords(rect());
	renderRect = renderRect.Translate(offset);
	mAnimator.render(renderRect, mPhysics.flip());

	mEffects.render();
}

void Actor::render()
{
	RectF renderRect = Camera::Get()->toCameraCoords(rect());
	mAnimator.render(renderRect, mPhysics.flip());

	mEffects.render();
}


void Actor::clear()
{
	mEnvironment = nullptr;
	mPhysics.reset();
	mAnimator.clear();

	while (mCollider.hasEffects())
	{
		mEffects.returnEffect(mCollider.popEffect());
	}

	mEffects.clear();
	mAttributeBag.clear();

	mEvents.clear();

	mVisibility = true;
}


float Actor::getAttributeValue(AttributeType type) const
{ 
	return mAttributeBag.value(type);
}


Attribute* Actor::getAttribute(AttributeType type) const
{
	return mAttributeBag.at(type);
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