#pragma once

#include "EnemyStates/EnemyState.h"
#include "EnemyStates/EnemyStateHeaders.h"

class Environment;
class Enemy;

class EnemyDebugger
{
public:
	void labelState(EnemyState::Type state, RectF enemyRect) const;
	void drawPlayerDistance(Environment* environment, const Enemy* enemy) const;

};



namespace std
{
	template <>
	struct hash<EnemyState::Type>
	{
		std::size_t operator()(EnemyState::Type state) const
		{
			unsigned long enemyState = (unsigned long)state;
			return std::hash<unsigned long>{ }(enemyState);
		}
	};
}


const static std::unordered_map<EnemyState::Type, BasicString> enemyStateLookUpTable
{
	{ EnemyState::Wait, "Wait" },
	{ EnemyState::Idle, "Idle" },
	{ EnemyState::Run, "Run" },
	{ EnemyState::Hit, "Hit" },
	{ EnemyState::Patrol, "Patrol" },
	{ EnemyState::Alert, "Alert" },
	{ EnemyState::PreAttack, "PreAttack" },
	{ EnemyState::Attack, "Attack" },
	{ EnemyState::Dead, "Dead" },
};

