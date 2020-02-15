#pragma once

enum class EnemyState
{
	None,
	Idle,
	Run,
	Patrol,
	Alert,
	Hit,
	Attack,
	Dead
};

enum class EnemyType
{
	None,
	Imp
};