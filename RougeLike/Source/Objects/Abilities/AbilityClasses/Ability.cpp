#include "pch.h"
#include "Ability.h"

#include "Graphics/Renderer.h"
#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"

#include "Game/Camera/Camera.h"

#if DEBUG_CHECK
#include "Debug/DebugDraw.h"
#endif


Ability::Ability() : 
	mState(AbilityState::None), 
	mCollider(nullptr), mCaster(nullptr), 
	mActivateCollisions(false), mCompleted(false)
{ }


Ability::~Ability()
{
	if(mCollider)
		delete mCollider;
}


void Ability::baseInit(const PropertyMap& properties, Animator animator)
{
	mAnimator = animator;
	mProperties = properties;

	if(animator.animationCount() > 0 && properties.contains(PropertyType::MaxSize))
		mRect.SetSize(realiseSize(animator.frameSize(), properties.at(PropertyType::MaxSize)));
	
	mCooldown = Cooldown(properties.at(PropertyType::Cooldown));
	mSelectHighlight = RenderColour::LightGrey;

#if NO_ABILITY_COOLDOWNS
	mCooldown = Cooldown(0.0f);
#endif
}


void Ability::baseExit()
{
	mAnimator.reset();
	mCooldown.stop();
	mCompleted = false;
	mSelectHighlight = RenderColour::LightGrey;
}


void Ability::applyEffect(EffectType effectType, Actor* target, EffectPool* effectPool) const
{
	Effect* effect = effectPool->getObject(effectType);
	effect->fill(mProperties);
	target->addEffect(effect);
}



BasicString Ability::name() const
{ 
	AbilityType abilityType = type();
	BasicString id;
	abilityType >> id;

	return id; 
}


void Ability::render()
{
#if DRAW_ABILITY_RECTS
	debugDrawRect(mRect, RenderColour::Yellow);
#endif
#if TRACK_COLLISIONS
	if(mCollider)
		mCollider->renderCollider();
#endif

	mAnimator.render(Camera::Get()->toCameraCoords(mRect));
}


void renderQuadOutline(const Quad2D<float>& quad, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	for (unsigned int i = 0; i < quad.sides(); i++)
	{
		int j = i + 1 >= quad.sides() ? 0 : i + 1;
		Vector2D<int> pointA = quad.at(i).toInt();
		Vector2D<int> pointB = quad.at(j).toInt();
		SDL_RenderDrawLine(Renderer::Get()->sdlRenderer(), pointA.x, pointA.y, pointB.x, pointB.y);
	}
}