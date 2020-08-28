#include "pch.h"
#include "PlayerManager.h"

#include "Game/Camera/Camera.h"
#include "Game/Data/GameData.h"
#include "Game/Environment.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"

#include "Weapons/Melee/MeleeWeapon.h"


PlayerManager::PlayerManager() : mEnvironment(nullptr) { }


PlayerManager::~PlayerManager()
{
	clear();
	mEnvironment = nullptr;
}


void PlayerManager::init(Environment* environment, CollisionManager* collisions, Screen* gameScreen)
{
	mPlayerCollisions.init(&mPlayer, collisions);
	mWeaponStash.load();

	mEnvironment = environment;
	mPlayer.set(environment);
	mAbilities.init(environment, &mPlayer, gameScreen);
	mEvents.clear();
}

void PlayerManager::clear()
{
	mPlayer.clear();
	mAbilities.clear();
	mPlayerCollisions.clear();
	mWeaponStash.clear();
	mLevelling.reset();
}


void PlayerManager::addAbility(AbilityType ability)
{
	mAbilities.addAbility(ability);
}

void PlayerManager::addExp(int exp)
{
	bool didLevelUp = mLevelling.gainExp(this, exp);

	if (didLevelUp && mLevelling.level() == 2)
	{
		OpenPopupEvent* eventPtr = new OpenPopupEvent("LevelUpInfo");
		EventPacket event(eventPtr);
		mEvents.push(event);
	}
}

void PlayerManager::setPosition(VectorF position)
{
	mPlayer.rectRef().SetLeftCenter(position);
}


void PlayerManager::handleInput(const InputManager* input)
{
	if (mPlayer.userHasControl())
	{
		mPlayer.handleInput(input);
		mPlayer.updateCursorPosition(input->cursorPosition()); // move into player::handleInput?

		if (input->isCursorPressed(Cursor::Left) && !mAbilities.inSelectionMode())
		{
			bool didAttack = mPlayer.attemptAttack();
			if (didAttack)
			{
				mPlayerCollisions.clearExcludedColliders(CollisionManager::PlayerWeapon_Hit_Enemy);
				//mGameData->audioManager->playSound(mPlayer->weapon()->missSoundLabel(), this);
			}
		}


		mAbilities.handleInput(input);
	}
}

void PlayerManager::fastUpdate(float dt)
{
	mPlayer.fastUpdate(dt);

	Map* playerMap = mEnvironment->map(mPlayer.position());
	mPlayerCollisions.resolveWalls(playerMap, dt);
	mPlayerCollisions.resolveWeapons();
}

void PlayerManager::slowUpdate(float dt)
{
	mPlayer.slowUpdate(dt);
	mLevelling.slowUpdate(dt);

	Map* playerMap = mEnvironment->map(mPlayer.position());
	mPlayer.updateMapInfo(playerMap);



	//if (mPlayer->weapon()->isAttacking())
	//{
	//	mPlayer->updateWeaponHitSound(mGameData->audioManager);
	//}

	mAbilities.slowUpdate(dt);

	handleEvents();
}


void PlayerManager::handleEvents()
{
	while (mAbilities.hasEvent())
		mEvents.push(mAbilities.popEvent());

	while (mPlayer.events().hasEvent())
	{
		EventPacket event = mPlayer.events().pop();

		if (event.data->eventType == Event::HealthChanged)
		{
			event.free();

			Health* hp = static_cast<Health*>(mPlayer.getAttribute(AttributeType::Health));
			SetUISlider* healthBar = new SetUISlider("HealthSlider", hp->getPercentage());
			mEvents.push(EventPacket(healthBar));
		}
		else
		{
			mEvents.push(event);
		}
	}
}


void PlayerManager::render()
{
	mPlayer.render();
	mAbilities.render();

	mLevelling.render(mPlayer.rect());
}


void PlayerManager::selectCharacter(const BasicString& characterConfig)
{ 
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Player, characterConfig));
	mPlayer.setCharacter(parser.rootNode());

	EffectMap bag;
	XMLNode effects = parser.rootChild("Effects");
	bag.fill(effects);

	BasicString weapontype = parser.rootChild("WeaponType").value();
	mPlayer.setWeaponType(mWeaponStash.getWeapon(weapontype));


	mLevelling.init(parser.rootChild("Levelling"), mPlayer.rect());

#if UNLOCK_ALL_ABILITIES
	mLevelling.unlockAllAbilities(this);
#endif
}


void PlayerManager::selectWeapon(const BasicString& weaponName)
{
	WeaponData* weaponData = mWeaponStash.getData(weaponName);
	mPlayer.selectWeapon(weaponData);

	mPlayer.effects().setAttackingEffectdata(weaponData->effects);

	mPlayerCollisions.refreshWeaponColliders();
}