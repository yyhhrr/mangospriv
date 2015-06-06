#pragma once

#include "Unit.h"
#include "SpellAuraDefines.h"

class UnitFearManager
{
	Unit* mUnit;
	uint32 mDamageReceived;
	bool mHasFearAura;
	uint32 mBreakChance;

	static const uint32 MAX_FEAR_DAMAGE = 1500;

	void breakFear();

public:
	UnitFearManager(Unit* pUnit);

	void updateAuras(AuraType type, bool apply);
	void onDamageReceived(uint32 damage);
};