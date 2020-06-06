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
	// Setup animations
	AnimationReader reader(mGameData->textureManager);
	reader.initAnimator2(mAnimator2, characterConfig);
	mAnimator2.start();

	// Set size
	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Configs_Objects, characterConfig));

	VectorF baseSize = mAnimator2.frameSize();
	float maxDimention = std::stof(parser.firstRootNodeValue("MaxSize"));
	VectorF size = realiseSize(baseSize, maxDimention);

	mPhysics.setRect(RectF(VectorF(), size));

	// Set properties
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
	mAnimator2.slowUpdate(dt);
	mEffects.slowUpdate(dt);
}


void Actor::render()
{
	RectF rect = renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

	//mAnimator.getSpriteTile()->render(rect, mPhysics.flip());

	mAnimator2.render(rect, mPhysics.flip());
}

void Actor::reset()
{
	mPhysics.reset();
	mAnimator.clear();
	mAnimator2.clear();
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
