#include "pch.h"
#include "PlayerManager.h"

#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"
#include "Game/Environment.h"
#include "UI/UIManager.h"

#include "Objects/Abilities/AbilityClasses/Ability.h"
#include "Weapons/Melee/MeleeWeapon.h"


PlayerManager::PlayerManager() { }
PlayerManager::~PlayerManager() { clear(); }


void PlayerManager::init(Environment* environment, Screen* gameScreen)
{
	mWallCollisions.setCollider(&mPlayer.mCollider);
	mWeaponStash.load();

	mPlayer.set(environment);
	mAbilities.init(&mPlayer, gameScreen, environment->effectPool(), environment->actors()->activeEnemyActors());

	mWeaponCollisions.init(environment->effectPool());
	mWeaponCollisions.linkTargets(environment->actors()->activeEnemyActors());
	
	mEvents.clear();
}

void PlayerManager::clear()
{
	mPlayer.clear();
	mAbilities.clear();
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

	SetUISlider* sliderEventPtr = new SetUISlider("LevelSlider", mLevelling.expProgress());
	EventPacket sliderEvent(sliderEventPtr);
	mEvents.push(sliderEvent);
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
		mAbilities.handleInput(input);

		if (input->isCursorPressed(Cursor::Left))
		{
			bool didAttack = mPlayer.attack();
			if (didAttack)
			{
				mWeaponCollisions.beginAttack();
			}
		}

	}
}

void PlayerManager::fastUpdate(float dt)
{
	mPlayer.fastUpdate(dt);

	mAbilities.fastUpdate(dt);

	mWeaponCollisions.checkCollisions();

	if (mPlayer.userHasControl())
	{
#if !IGNORE_WALLS
		const Map* playerMap = mPlayer.currentMap();
		if (playerMap)
		{
			VectorF velocity = mPlayer.physics()->velocity();
			velocity = mWallCollisions.allowedVelocity(playerMap, velocity, dt);
			mPlayer.physics()->setVelocity(velocity);
		}
#endif

		mPlayer.move(dt);
	}
}


void PlayerManager::slowUpdate(float dt)
{
	mPlayer.slowUpdate(dt);
	mLevelling.slowUpdate(dt);

	mAbilities.slowUpdate(dt);

	handleEvents();
}


void PlayerManager::handleEvents()
{
	while (mAbilities.events().hasEvent())
		mEvents.push(mAbilities.events().pop());

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



void PlayerManager::resetColliders() 
{ 
	mPlayer.resetColliders();
}


void PlayerManager::render()
{
	mPlayer.render();
	mAbilities.render();

	mLevelling.render(mPlayer.rect());
}


void PlayerManager::selectCharacter(const Character& character)
{ 
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Player, "Player"));

	// Animation
	XMLParser animationParser(FileManager::Get()->findFile(FileManager::Config_Player, character.mName + "Anim"));
	XMLNode animationNode = animationParser.rootChild("Animator");
	mPlayer.setCharacter(parser.rootNode(), animationNode);

	selectWeapon(character);

	mPlayer.init();
	mLevelling.init(mPlayer.rect().Size());

#if UNLOCK_ALL_ABILITIES
	mLevelling.unlockAllAbilities(this);
#endif
}


void PlayerManager::selectWeapon(const Character& character)
{
	if (mPlayer.mWeapon)
	{
		delete mPlayer.mWeapon;
		mPlayer.mWeapon = nullptr;
	}

	WeaponData* weaponData = mWeaponStash.getData(character.mWeaponName);
	Weapon* weapon = mWeaponStash.getNewWeapon(weaponData->type);
	weapon->equipt(weaponData);
	mPlayer.mWeapon = weapon;

	mWeaponCollisions.linkWeapon(weapon);
}