#include "pch.h"
#include "EnemyFactory.h"

#include "Actors/Enemies/Enemy.h"

#include "Objects/Abilities/AbilityBuilder.h"
#include "Objects/Abilities/AbilityClasses/AbilityStates.h"
#include "Objects/Abilities/AbilityClasses/Ability.h"


EnemyFactory::~EnemyFactory()
{
	clear();
}


void EnemyFactory::initSpawnInfo()
{
	std::vector<BasicString> filePaths = FileManager::Get()->allFilesInFolder(FileManager::Config_Enemies);
	std::vector<BasicString> fileNames = FileManager::Get()->fileNamesInFolder(FileManager::Config_Enemies);

	for (const BasicString& name : fileNames)
	{
		BasicString filePath = FileManager::Get()->findFile(FileManager::Config_Enemies, name);

		mSpawnInfo[name].parseXML(filePath);
	}
}


void EnemyFactory::loadPools()
{
	std::vector<EnemyType> enemyTypes;
	for (EnemyType type = EnemyType::None + 1; type < EnemyType::Count; type = type + 1)
	{
		enemyTypes.push_back(type);
	}
	mPool.load(enemyTypes, 25);


	std::vector<EnemyState::Type> enemyStates;
	for (EnemyState::Type state = EnemyState::Type::None + 1; state < EnemyState::Type::Count; state = state + 1)
	{
		enemyStates.push_back(state);
	}
	mStatePool.load(enemyStates, 50);

	// Need lots of these states
	mStatePool.addNewObjects(EnemyState::Wait, 200);
	mStatePool.addNewObjects(EnemyState::Idle, 200);
	mStatePool.addNewObjects(EnemyState::Run, 200);
	mStatePool.addNewObjects(EnemyState::Patrol, 100);
}

void EnemyFactory::clear()
{
	mPool.freeAll();
	mStatePool.freeAll();
}


void EnemyFactory::returnEnemy(Enemy* enemy)
{
	// Return all enemy states (except null, statemachine itself deals with that)
	StateMachine<EnemyState>* states = enemy->getStateMachine();
	while (enemy->state() != EnemyState::None)
	{
		EnemyState* state = states->forcePop();
		mStatePool.returnObject(state);
	}

	mPool.returnObject(enemy, enemy->type());
}


void EnemyFactory::topUpPool(const SpawnDataList& data)
{
	for (std::unordered_map<EnemyType, int>::const_iterator iter = data.mTypeCount.begin(); iter != data.mTypeCount.end(); iter++)
	{
		int currentSize = mPool.size(iter->first);
		if (currentSize < iter->second)
		{
			int difference = iter->second - currentSize;
			mPool.addNewObjects(iter->first, difference);
		}
	}
}


// TODO: make this accept a queue
Enemy* EnemyFactory::buildEnemy(const SpawnData& data, const AIPathMap* aiPathMap)
{
	EnemyType type = data.type;

	BasicString name;
	type >> name;

	XMLNode root = mSpawnInfo[name].rootNode();
	XMLNode animationNode = root.child("Animator");
	BasicString enemyBaseType = root.child("BaseType").value();
	XMLNode baseTypeNode = mSpawnInfo[enemyBaseType].rootNode();

	Enemy* enemy = mPool.getObject(data.type);
	setEnemyData(enemy, data, baseTypeNode, animationNode, aiPathMap);

	return enemy;
}


Enemy* EnemyFactory::setEnemyData(Enemy* enemy, const SpawnData& data, const XMLNode baseNode, const XMLNode animationNode, const AIPathMap* aiPathMap)
{
	enemy->setCharacter(baseNode, animationNode);
	enemy->setStatePool(&mStatePool);
	enemy->spawn(data.state, data.position, aiPathMap);

	Ability* basicAttack = buildBasicAttack(enemy, baseNode);
	enemy->abilities().add(basicAttack);

	return enemy;
}


Ability* EnemyFactory::buildBasicAttack(Enemy* enemy, const XMLNode enemyNode)
{
	AbilityBuilder abilityBuilder;
	Ability* basicAttack = abilityBuilder.build(AbilityType::Attack, enemy);

	XMLNode abilityNode = enemyNode.child("BasicAttack");
	PropertyMap nodeProperties(abilityNode);
	basicAttack->properties().merge(nodeProperties);
	basicAttack->setState(AbilityState::Idle);
	return basicAttack;
}



