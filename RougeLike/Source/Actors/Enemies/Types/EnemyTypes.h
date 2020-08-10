#pragma once


enum class EnemyType
{
	None,
	Imp,
	Angel,
	Devil
};



inline void operator >>(EnemyType type, BasicString& string)
{
	switch (type)
	{
	case EnemyType::Devil:
		string = "Devil";
		break;
	case EnemyType::None:
	case EnemyType::Imp:
	case EnemyType::Angel:
	default:
		DebugPrint(Warning, "EnemyType to string conversion has not been defined\n");
		break;
	}
}


inline void operator <<(EnemyType& type, const BasicString& string)
{
	if (string == "Devil")
		type = EnemyType::Devil;
	else
		DebugPrint(Warning, "The string '%s' has not been defined as an enemy type\n", string);
}
