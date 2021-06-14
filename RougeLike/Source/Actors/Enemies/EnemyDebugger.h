#pragma once

#if DEBUG_CHECK
#include "EnemyStates/EnemyState.h"
#include "EnemyStates/EnemyStateHeaders.h"
#include "Map/Tiles/MapTile.h"

class Environment;
class Enemy;

class EnemyDebugger
{
public:
	EnemyDebugger(Enemy* enemy) : mEnemy(enemy) { }

	void draw();

	void setPath(const Path path) { mPath = path; }


private:
	void labelState();
	void labelHP();
	void drawTargetDistance();
	void drawRects();
	void drawPath();

private:
	Enemy* mEnemy;
	Path mPath;
};


const static std::unordered_map<EnemyState::Type, BasicString> enemyStateLookUpTable
{
	{ EnemyState::Wait, "Wait" },
	{ EnemyState::Idle, "Idle" },
	{ EnemyState::Run, "Run" },
	{ EnemyState::Patrol, "Patrol" },
	{ EnemyState::Alert, "Alert" },
	{ EnemyState::Hit, "Hit" },
	{ EnemyState::Stun, "Stunned" },
	{ EnemyState::PreAttack, "PreAttack" },
	{ EnemyState::Attack, "Attack" },
	{ EnemyState::Charge, "Charge"},
	{ EnemyState::Dead, "Dead" },
	{ EnemyState::Exit, "Exit" }
};

#endif