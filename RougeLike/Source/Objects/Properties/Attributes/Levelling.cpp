#include "pch.h"
#include "Levelling.h"

#include "Animations/AnimationReader.h"
#include "Game/Camera/Camera.h"

#include "Actors/Player/PlayerManager.h"
#include "Objects/Abilities/AbilityClasses/AbilityStates.h"

#include "Audio/AudioManager.h"


Levelling::Levelling() : mLevel(1), mCurrentExp(0), mRequiredExp(0) { }
Levelling::~Levelling() { reset(); }

void Levelling::init(const XMLNode& levelNode, RectF rect)
{
	const BasicString& levelInfoFile = levelNode.value();
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Player, levelInfoFile));

	XMLNode requiredExpNode = parser.rootChild("RequiredExp");
	mRequiredExp = atoi(requiredExpNode.value().c_str());

	XMLNode abilities = parser.rootChild("Abilities");

	XMLNode ability = abilities.child();
	while (ability)
	{
		BasicString name = ability.name();
		AbilityType type = AbilityType::None;

		type << name;

		if (type != AbilityType::None)
			mLockedAbilities.push(type);
		else
			DebugPrint(Warning, "Ability name %s not recognised\n", name.c_str());

		ability = ability.next();
	}

	buildAnimator(parser.rootNode(), rect);
}


void Levelling::reset()
{
	mLevel = 1;
	mCurrentExp = 0;
	mRequiredExp = 0;

	mRect = RectF();
	mRenderOffset = VectorF();
	mAnimator.clear();

	mLockedAbilities.clear();
}


void Levelling::buildAnimator(const XMLNode& node, RectF rect)
{
	AnimationReader reader;
	mAnimator = reader.buildAnimator(node.child("Animator"));
	mAnimator.selectAnimation(Animation::Active);

	// size
	float targetWidth = rect.Width() * (float)atof(node.child("WidthMultiplier").value().c_str());
	float targetHeight = targetWidth * mAnimator.frameSize().y / mAnimator.frameSize().x;
	VectorF size = VectorF(targetWidth, targetHeight);
	mRect.SetSize(size);

	// render offset
	XMLNode renderOffsetNode = node.child("RenderOffsetRatio");
	StringMap offsetData = renderOffsetNode.attributes();
	mRenderOffset = offsetData.getVector("x", "y") * size;
}


void Levelling::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() == 1)
	{
		mAnimator.reset();
		mAnimator.stop();
	}
}

void Levelling::render(const RectF& playerRect)
{
	if (mAnimator.isRunning())
	{
		mRect.SetBotCenter(playerRect.BotCenter());
		mRect = Camera::Get()->toCameraCoords(mRect.Translate(mRenderOffset));
		mAnimator.render(mRect);
	}
}


bool Levelling::gainExp(PlayerManager* player, int exp)
{
	mCurrentExp += exp;

	if (mCurrentExp >= mRequiredExp)
	{
		levelUp(player);
		return true;
	}

	return false;
}



void Levelling::levelUp(PlayerManager* player)
{
	mLevel++;
	mCurrentExp -= mRequiredExp;
	mRequiredExp = mRequiredExp * 3;

	mAnimator.start();
	AudioManager::Get()->play("LevelUp", player);

	if (mLockedAbilities.size() > 0)
	{
		AbilityType type = mLockedAbilities.pop();
		player->addAbility(type);

		// FIX ME: Right now opening a bunch of popups as soon as the game starts breaks the game
		// this is becuase we can only have 1 popup at the time and it auto adds a pause state
		// pressing ok pops the latest state, the issue is when the game starts the adding of
		// the game state overrites the adding of a pause state so the popup pops te game state... I think
#if !UNLOCK_ALL_ABILITIES
		BasicString abilityName;
		type >> abilityName;
		OpenPopupEvent* eventPtr = new OpenPopupEvent(abilityName);
		EventPacket event(eventPtr);
		player->events().push(event);
#endif
	}
}



#if UNLOCK_ALL_ABILITIES
void Levelling::unlockAllAbilities(PlayerManager* player)
{
	// + 2 so we also skip the basic attack ability
	for (AbilityType type = AbilityType::None + 2; type < AbilityType::Count; type = type + 1)
	{
		mLockedAbilities.push(type);
	}

	while (mLockedAbilities.size() > 0)
	{
		gainExp(player, 100);
	}
}
#endif