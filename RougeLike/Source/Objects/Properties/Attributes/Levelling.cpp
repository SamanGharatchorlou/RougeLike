#include "pch.h"
#include "Levelling.h"

#include "Animations/AnimationReader.h"
#include "Game/Camera/Camera.h"

#include "Actors/Player/PlayerManager.h"
#include "Objects/Abilities/AbilityClasses/AbilityStates.h"


Levelling::Levelling() : mLevel(1), mCurrentExp(0), mRequiredExp(100) { }
Levelling::~Levelling() { reset(); }

void Levelling::init(const XMLNode& levelNode, RectF rect)
{
	XMLNode abilities = levelNode.child("Abilities");

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

	buildAnimator(levelNode.child("Info").value(), rect);
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


void Levelling::buildAnimator(const BasicString& infoFile, RectF rect)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Player, infoFile));

	AnimationReader reader;
	XMLNode root = parser.rootNode();
	mAnimator = reader.buildAnimator(root.child("Animator"));
	mAnimator.selectAnimation(Action::Active);

	// size
	float targetWidth = rect.Width() * atof(root.child("WidthMultiplier").value().c_str());
	float targetHeight = targetWidth * mAnimator.frameSize().y / mAnimator.frameSize().x;
	VectorF size = VectorF(targetWidth, targetHeight);
	mRect.SetSize(size);

	// render offset
	XMLNode renderOffsetNode = root.child("RenderOffsetRatio");
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
	mRequiredExp = (int)((float)mRequiredExp * 1.5f);

	mAnimator.start();

	if(mLockedAbilities.size() > 0)
		player->addAbility(mLockedAbilities.popFront());
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