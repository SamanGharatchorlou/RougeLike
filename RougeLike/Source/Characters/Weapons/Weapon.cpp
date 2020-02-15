#include "pch.h"
#include "Weapon.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "Game/Cursor.h"
#include "Characters/Attributes/Movement.h"

#include "Collisions/Collider.h"




Weapon::Weapon(GameData* gameData) : mGameData(gameData) { }


void Weapon::selectWeapon(std::string weaponName)
{
	loadWeaponData(weaponName);

	// couple the block rects and colliders
	int blocks = 4;
	for (int i = 0; i < blocks; i++)
	{
		mBlockRects.push_back(RectF());
		mBlockColliders.push_back(Collider(mDamage));
	}

	for (int i = 0; i < blocks; i++)
	{
		mBlockColliders[i].init(&mBlockRects[i]);
		mBlockColliders[i].setActive(false);
	}
}

void Weapon::fastUpdate(VectorF anchorPosition)
{
	// follow character
	mRect.SetTopLeft(anchorPosition + mPommeloffset);

	// follow cursor
	if (!mOverrideCursorControl)
	{
		double offsetAngle = (mSwing.getAngle() / 2.0) * mSwing.getDirection();

		// camera to cursor vector
		mDirection = (mGameData->cursor->getPosition() - mGameData->camera->toCameraCoords(mRect.BotCenter()));
		mDirection = rotateVector(mDirection, offsetAngle);
	}

	updateWeaponBlocks();
}


void Weapon::render()
{
	VectorF aboutPoint(mRect.Width() / 2.0f, mRect.Height());
	mTexture->render(mGameData->camera->toCameraCoords(mRect), getRotation(mDirection), aboutPoint);
}


void Weapon::rotate(double theta)
{
	mDirection = rotateVector(mDirection, theta * -mSwing.getDirection());
}


// have this return blocks instead?
void Weapon::updateWeaponBlocks()
{
	VectorF weaponVector = mDirection.normalise() * mRect.Height();

	int blocks = mBlockRects.size();
	float blockWidth = std::max(std::abs(weaponVector.x) / blocks, mRect.Width());
	float blockHeight = std::max(std::abs(weaponVector.y) / blocks, mRect.Width());
	VectorF blockSize = VectorF(blockWidth, blockHeight);

	for (int i = 0; i < blocks; i++)
	{
		// Center block on weapon aboutpoint (pommel) coords
		RectF block = RectF(blockSize / -2, blockSize);

		VectorF shaftPosition = (weaponVector / (blocks)) * ( 0.5 + i);
		block = block.Translate(shaftPosition);

		// Translate to world coords
		block = block.Translate(mRect.BotCenter());

		mBlockRects[i].SetRect(block);
	}
}

std::vector<Collider*> Weapon::getColliders()
{
	std::vector<Collider*> blockColliders;

	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		blockColliders.push_back(&mBlockColliders[i]);
	}

	return blockColliders;
}


void Weapon::loadWeaponData(std::string weaponName)
{
	std::string configFilePath = FileManager::Get()->getXMLFilePath(FileManager::Config_Weapons, weaponName);

	XMLParser parser;
	parser.parseXML(configFilePath);
	xmlNode rootNode = parser.getRootNode();

	std::string textureName = rootNode->first_node("Name")->value();
	mTexture = mGameData->textureManager->getTexture(textureName);

	// Get values
	float damage = std::stof(rootNode->first_node("Damage")->value());
	mDamage.set(damage);

	double swingSpeed = std::stod(rootNode->first_node("SwingSpeed")->value());
	double swingAngle = std::stod(rootNode->first_node("SwingAngle")->value());
	mSwing = Swing(swingAngle, swingSpeed);

	Attributes attributes = parser.getAttributes(rootNode->first_node("PommelOffset"));
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");
	mPommeloffset.set(x, y);
}


void Weapon::setScale(float scale)
{
	mRect.SetSize(mTexture->originalDimentions * scale);
}

void Weapon::setColliderActivite(bool isActive)
{
	for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	{
		mBlockColliders[i].setActive(isActive);
	}
}