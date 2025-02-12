#include "BuffDefinitions.h"

void MovementSpeedBuff::EvaluateStat(EBuffStat stat, float& value)
{
	if (stat == EBuffStat::MoveSpeed)
	{
		value += flatMoveSpeedIncrease;
		value *= multiplierMoveSpeedIncrease;
	}
}

void BulletCountBuff::EvaluateStat(EBuffStat stat, float& value)
{
	if (stat == EBuffStat::BulletCount)
	{
		value += additionalBulletsPerFire;
	}
}

ArmorBuff::ArmorBuff(float multiplier)
	: incomingDamageMultipier(multiplier)
{}

void ArmorBuff::EvaluateStat(EBuffStat stat, float& value)
{
	if (stat == EBuffStat::Armor)
	{
		value *= incomingDamageMultipier;
	}
}
