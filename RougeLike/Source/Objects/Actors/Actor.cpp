#include "pch.h"
#include "Actor.h"

#include "Game/GameData.h"
#include "Collisions/Collider.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Game/Camera.h"
#include "Animations/AnimationReader.h"

#include "Objects/Properties/PropertyBag.h"

/*
ASTCCodec
Avro
Basis
Chroma
Deflate
Detex
EASTL
EdgeGeom
ETC1
ETC2Comp
FreeType
Hydra
HydraCurl
Implode
JpegLib
LibPNG
LibTIFF
LZMA
MD5
MSDFGen
Ogg
OpenEXR
PVRTDecompress
Refpack
SPIRVCross
Squish
TinyXML2
Vivox
Vorbis
ZLib
*/

Actor::Actor(GameData* gameData) : mGameData(gameData), mEffects(this), mVisibility(true) { }


Actor::~Actor()
{
	delete mPropertyBag;
	delete mCollider;
}


void Actor::init(const std::string& characterConfig)
{
	// Setup animations
	AnimationReader reader(mGameData->textureManager);
	if(!reader.initAnimator(mAnimator, characterConfig))
		DebugPrint(Warning, "Animator setup failed for actor config '%s' \n", characterConfig.c_str());

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

bool Actor::hasProperty(const std::string& property) const
{
	return mPropertyBag->contains(property);
}

void Actor::addEffect(Effect* effect)
{
	mEffects.addEffect(effect);
}
