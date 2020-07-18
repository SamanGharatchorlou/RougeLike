#include "pch.h"
#include "PlayerManager.h"

#include "Game/Camera.h"
#include "Game/GameData.h"
#include "Map/Environment.h"
#include "Input/InputManager.h"


#include "Weapons/Melee/MeleeWeapon.h"


PlayerManager::PlayerManager(GameData* gameData) :
	mEnvironment(gameData->environment), 
	mAbilities(gameData->textureManager, gameData->environment, &mPlayer)
{
	mPlayerCollisions.init(&mPlayer, gameData->collisionManager);
	weaponStash.init(gameData->textureManager);
}


void PlayerManager::init(Environment* environment)
{
	mEnvironment = environment;
	mPlayer.init(environment);
}

void PlayerManager::setPosition(VectorF position)
{
	VectorF playerPosition = VectorF(Camera::Get()->rect().Center().x, mEnvironment->size().y / 2.0f);
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
	mPlayerCollisions.fastUpdate(dt, mEnvironment->map(mPlayer.position()));
	mPlayer.fastUpdate(dt);
}

void PlayerManager::slowUpdate(float dt)
{
	Map* playerMap = mEnvironment->map(mPlayer.position());
	mPlayerCollisions.slowUpdate(playerMap);

	mPlayer.slowUpdate(dt);
	mPlayer.updateCurrentTile(playerMap);

	//if (mPlayer->weapon()->isAttacking())
	//{
	//	mPlayer->updateWeaponHitSound(mGameData->audioManager);
	//}

	mAbilities.slowUpdate(dt);
	while (mAbilities.hasEvent())
		 mEvents.push(mAbilities.popEvent());

	while (mPlayer.hasEvent())
		mEvents.push(mPlayer.popEvent());
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

void PlayerManager::selectCharacter(const std::string& characterConfig, TextureManager* textureManager)
{ 
	mPlayer.setCharacter(characterConfig, textureManager); 
}


void PlayerManager::selectWeapon(const std::string& weaponName)
{
	Weapon* weapon = weaponStash.getWeapon(weaponName);
	mPlayer.setWeapon(static_cast<MeleeWeapon*>(weapon));

	mPlayerCollisions.refreshWeaponColliders();
}