#include "pch.h"
#include "PlayerManager.h"

#include "Game/Camera/Camera.h"
#include "Game/Data/GameData.h"
#include "Game/Environment.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"

#include "Weapons/Melee/MeleeWeapon.h"


PlayerManager::PlayerManager(GameData* gameData) :
	mEnvironment(gameData->environment), 
	mAbilities(gameData->textureManager, &mPlayer, gameData->uiManager->screen(Screen::Game))
{
	mPlayerCollisions.init(&mPlayer, gameData->collisionManager);
	mWeaponStash.load(gameData->textureManager);
}


void PlayerManager::init(Environment* environment)
{
	mEnvironment = environment;
	mPlayer.set(environment);
	mAbilities.init(environment);
}

void PlayerManager::addAbility(const BasicString& ability)
{
	mAbilities.addAbility(ability, &mPlayer);
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
	mPlayerCollisions.resolveWeapons(mEnvironment->effectPool());
}

void PlayerManager::slowUpdate(float dt)
{
	// TODO: move any of these into fast update?
	mPlayerCollisions.enableBodyCollisions(mPlayer.hasBodyCollisions());

	mPlayer.slowUpdate(dt);

	Map* playerMap = mEnvironment->map(mPlayer.position());
	mPlayer.updateCurrentTile(playerMap);

	//if (mPlayer->weapon()->isAttacking())
	//{
	//	mPlayer->updateWeaponHitSound(mGameData->audioManager);
	//}

	mAbilities.slowUpdate(dt);
	while (mAbilities.hasEvent())
		 mEvents.push(mAbilities.popEvent());

	while (mPlayer.events().hasEvent())
		mEvents.push(mPlayer.events().pop());
}

void PlayerManager::render()
{
	mPlayer.render();
	mAbilities.render();
}


void PlayerManager::exit()
{
	mPlayer.reset();
}

void PlayerManager::selectCharacter(const BasicString& characterConfig, const TextureManager* textureManager)
{ 
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Player, characterConfig));
	mPlayer.setCharacter(parser.rootNode(), textureManager);

	EffectMap bag;
	XMLNode effects = parser.rootChild("Effects");
	bag.fill(effects);

	BasicString weapontype = parser.rootChild("WeaponType").value();
	mPlayer.setWeaponType(mWeaponStash.getWeapon(weapontype));
}


void PlayerManager::selectWeapon(const BasicString& weaponName)
{
	WeaponData* weaponData = mWeaponStash.getData(weaponName);
	mPlayer.selectWeapon(weaponData);
	mPlayer.effects().setEffectBag(weaponData->effects);

	mPlayerCollisions.refreshWeaponColliders();
}