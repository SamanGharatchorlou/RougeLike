#pragma once


enum class EnemyType
{
	None,
	Devil,
	Orc,
	Count
};


inline EnemyType operator +(EnemyType type, int number)
{
	int sum = static_cast<int>(type) + number;

#if _DEBUG
	if (sum > (int)EnemyType::Count)
		DebugPrint(Warning, "adding %d to EnemyType %d is out of bounds", number, (int)type);
#endif

	return static_cast<EnemyType>(sum);
}


inline void operator >>(EnemyType type, BasicString& string)
{
	switch (type)
	{
	case EnemyType::Devil:
		string = "Devil";
		break;
	case EnemyType::Orc:
		string = "Orc";
		break;
	case EnemyType::None:
	default:
		DebugPrint(Warning, "EnemyType to string conversion has not been defined\n");
		break;
	}
}


inline void operator <<(EnemyType& type, const BasicString& string)
{
	if (string == "Devil")
		type = EnemyType::Devil;
	if (string == "Orc")
		type = EnemyType::Orc;
	else
		DebugPrint(Warning, "The string '%s' has not been defined as an enemy type\n", string);
}
