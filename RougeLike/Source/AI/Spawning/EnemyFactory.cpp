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
	std::vector<EnemyState::Type> enemyStates;
	for (EnemyState::Type state = EnemyState::Type::None + 1; state < EnemyState::Type::Count; state = state + 1)
	{
		enemyStates.push_back(state);
	}
	mStatePool.load(enemyStates, 100);

	// The enemy pool is dynamically increased as required by the topUpPool function
	// Some of the more frequently used enemy states also follow this logic
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
	int totalCount = 0;
	for (std::unordered_map<EnemyType, int>::const_iterator iter = data.mTypeCount.begin(); iter != data.mTypeCount.end(); iter++)
	{
		int currentSize = mPool.size(iter->first);
		if (currentSize < iter->second)
		{
			int difference = iter->second - currentSize;
			mPool.addNewObjects(iter->first, difference);
		}

		totalCount += iter->second;
	}

	// These states are frequently required so keep these topped up too
	int stateCounts = mStatePool.size(EnemyState::Idle);
	if (stateCounts < totalCount)
	{
		int difference = totalCount - stateCounts;
		mStatePool.addNewObjects(EnemyState::Wait,	difference);
		mStatePool.addNewObjects(EnemyState::Idle,	difference);
		mStatePool.addNewObjects(EnemyState::Run,	difference);
		mStatePool.addNewObjects(EnemyState::Patrol,difference);
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
	Ability* basicAttack = abilityBuilder.build(AbilityType::BasicAttack, enemy);

	XMLNode abilityNode = enemyNode.child("BasicAttack");
	PropertyMap nodeProperties(abilityNode);
	basicAttack->properties().merge(nodeProperties);
	basicAttack->setState(AbilityState::Idle);
	return basicAttack;
}



