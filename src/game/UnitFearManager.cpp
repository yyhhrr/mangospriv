#include "Common.h"
#include "UnitFearManager.h"

UnitFearManager::UnitFearManager(Unit* target) {
	mUnit = target;
	mHasFearAura = false;
}

void UnitFearManager::updateAuras(AuraType type, bool apply) {
	if(mUnit->GetTypeId() != TYPEID_PLAYER)
		return;

	if(type == SPELL_AURA_MOD_FEAR) {
		if(apply == true) {
			mDamageReceived = 0;
			mBreakChance = 10;
		}

		mHasFearAura = apply;
	}
}

void UnitFearManager::breakFear() {
	Unit::AuraList auras = mUnit->GetAurasByType(SPELL_AURA_MOD_FEAR);
	for(auto auraItr : auras) {
		mUnit->RemoveAura(auraItr, AURA_REMOVE_BY_SHIELD_BREAK);
	}
}

void UnitFearManager::onDamageReceived(uint32 damage) {
	if(mUnit->GetTypeId() != TYPEID_PLAYER)
		return;

	if(mHasFearAura == true) {
		uint32 breakRand = urand(0, 99);
		if(breakRand < mBreakChance) {
			breakFear();
			return;
		}

		uint32 pctHealth = (uint32)(mUnit->GetMaxHealth() * 0.25f);

		mBreakChance = 10 + (uint32)(50 * (mDamageReceived / (float)pctHealth));

		mDamageReceived += damage;

		if(mDamageReceived >= /*MAX_FEAR_DAMAGE*/ pctHealth) {
			breakFear();
		}
	}
}