#pragma once

enum class EBuffStat
{
	MoveSpeed,
	BulletCount,
	Armor
};

class Buff
{
public:

	virtual ~Buff() = default;
	virtual void EvaluateStat(EBuffStat stat, float& value) {}

	float lifetime = -1.f;

};

class ArmorBuff : public Buff
{
public:
	ArmorBuff() = default;
	ArmorBuff(float multiplier);

	virtual void EvaluateStat(EBuffStat stat, float& value) override;

	float incomingDamageMultipier = 0.5f;
};

class MovementSpeedBuff : public Buff
{
public:
	virtual void EvaluateStat(EBuffStat stat, float& value) override;

	float flatMoveSpeedIncrease = 0.f;
	float multiplierMoveSpeedIncrease = 1.f;
};

class BulletCountBuff : public Buff
{
public:
	virtual void EvaluateStat(EBuffStat stat, float& value) override;

	int additionalBulletsPerFire = 0;
};