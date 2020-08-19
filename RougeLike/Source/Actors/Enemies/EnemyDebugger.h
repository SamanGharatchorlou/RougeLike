#pragma once

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
	void drawPlayerDistance(Environment* environment, const Enemy* enemy);
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
	{ EnemyState::Hit, "Hit" },
	{ EnemyState::Patrol, "Patrol" },
	{ EnemyState::Alert, "Alert" },
	{ EnemyState::Stun, "Stunned" },
	{ EnemyState::PreAttack, "PreAttack" },
	{ EnemyState::Attack, "Attack" },
	{ EnemyState::Dead, "Dead" },
};

