#pragma once

struct IDamageable
{
	virtual void ProcessDamage(float damageAmount) { }
	virtual int GetFaction() { return -1; }
	virtual float GetHealth() const { return 0.f; }
	virtual float GetMaxHealth() const { return 0.f; }
};