/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Spell_Scripts
SD%Complete: 100
SDComment: Spell scripts for dummy effects (if script need access/interaction with parts of other AI or instance, add it in related source files)
SDCategory: Spell
EndScriptData */

/* ContentData
spell 19512
EndContentData */

#include "precompiled.h"
#include "Unit.h"
#include "Spell.h"
#include "ObjectMgr.h"

/* When you make a spell effect:
- always check spell id and effect index
- always return true when the spell is handled by script
*/

enum
{
    // quest 6124/6129
    SPELL_APPLY_SALVE                   = 19512,
	SPELL_SICKLY_AURA                   = 19502,

    NPC_SICKLY_DEER                     = 12298,
    NPC_SICKLY_GAZELLE                  = 12296,

    NPC_CURED_DEER                      = 12299,
    NPC_CURED_GAZELLE                   = 12297,

};

bool EffectDummyCreature_spell_dummy_npc(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    switch(uiSpellId)
    {
        case SPELL_APPLY_SALVE:
        {
            if (uiEffIndex == EFFECT_INDEX_0)
            {
                if (pCaster->GetTypeId() != TYPEID_PLAYER)
                    return true;

                if (pCreatureTarget->GetEntry() != NPC_SICKLY_DEER && pCreatureTarget->GetEntry() != NPC_SICKLY_GAZELLE)
                    return true;

                // Update entry, remove aura, set the kill credit and despawn
                uint32 uiUpdateEntry = pCreatureTarget->GetEntry() == NPC_SICKLY_DEER ? NPC_CURED_DEER : NPC_CURED_GAZELLE;
                pCreatureTarget->RemoveAurasDueToSpell(SPELL_SICKLY_AURA);
                pCreatureTarget->UpdateEntry(uiUpdateEntry);
                ((Player*)pCaster)->KilledMonsterCredit(uiUpdateEntry);
                pCreatureTarget->ForcedDespawn(20000);

                return true;
            }
            return true;
        }
    }

    return false;
}

bool EffectGnomeShrink(Object* pCaster, uint32 spellId, SpellEffectIndex, Object* pTarget) {
	if(pCaster == nullptr || spellId != 13006 || pCaster->GetTypeId() != TYPEID_ITEM)
		return false;

	Player* plr = static_cast<Item*>(pCaster)->GetOwner();
	if(!plr)
		return false;

	if(pTarget == nullptr || (pTarget->GetTypeId() != TYPEID_UNIT && pTarget->GetTypeId() != TYPEID_PLAYER)) {
		Spell::SendCastResult(plr, spellId, SpellCastResult::SPELL_FAILED_BAD_TARGETS);
		return false;
	}

	if(plr->GetDistance((Unit*)pTarget) > 30.0f) {
		Spell::SendCastResult(plr, spellId, SpellCastResult::SPELL_FAILED_OUT_OF_RANGE);
		return false;
	}

	if(((Unit*)pTarget)->IsImmuneToSpell(13006)) {
		Spell::SendCastResult(plr, spellId, SpellCastResult::SPELL_FAILED_IMMUNE);
		return false;
	}

	if(pTarget->GetTypeId() != TYPEID_PLAYER) {
		if(((Creature*)pTarget)->getRank() >= CREATURE_ELITE_WORLDBOSS) {
			Spell::SendCastResult(plr, spellId, SpellCastResult::SPELL_FAILED_IMMUNE);
			return false;
		}
	}

	uint32 roll = urand(0, 99);
	if(roll < 60) {
		static const uint32 spells[] = { 13004, 13010 };
		uint32 spell = spells[urand(0, 1)];
		uint32 backFireMode = urand(0, 1);
		switch(backFireMode) {
		case 0:
			{
				plr->CastSpell(plr, spell, true);
				plr->AddSpellCooldown(13006, 10716, time(nullptr) + 300);
			}
			break;

		case 1:
			{
				Group* g = plr->GetGroup();
				Player* target = plr;
				if(g != nullptr) {
					uint32 gMember = urand(0, g->GetMembersCount() - 1);
					auto& list = g->GetMemberSlots();
					auto itr = list.begin();
					for(uint32 i = 0; i < gMember; ++i, ++itr);
					target = ObjectAccessor::FindPlayer(itr->guid);
					if(target == nullptr)
						target = plr;
				}

				target->CastSpell(target, spell, true);
				plr->AddSpellCooldown(13006, 10716, time(nullptr) + 300);
			}
			break;
		}

		return true;
	}

	((Unit*)pTarget)->CastSpell((Unit*)pTarget, 13010, true);
	plr->AddSpellCooldown(13006, 10716, time(nullptr) + 300);
	return true;
}

bool EffectGnomeChicken(Object* pCaster, uint32 spellId, SpellEffectIndex, Object* pTarget) {
	if(pCaster == nullptr || spellId != 23133)
		return false;

	Player* plr = nullptr;

	if(pCaster->GetTypeId() == TYPEID_PLAYER) {
		plr = (Player*)pCaster;
	} else if(pCaster->GetTypeId() == TYPEID_ITEM) {
		if(pTarget->GetTypeId() == TYPEID_PLAYER)
			plr = (Player*)pTarget;
		else
			return false;
	} else {
		return false;
	}
	
	plr->summonGuardian(8836, 90000, true);
	plr->AddSpellCooldown(spellId, 10725, time(nullptr) + 1200);
	return true;
}

bool EffectGoblinBomb(Object* pCaster, uint32, SpellEffectIndex, Object* pTarget) {
	if(pCaster->GetTypeId() != TYPEID_PLAYER && pTarget->GetTypeId() != TYPEID_PLAYER)
		return false;

	Player* plr = (pCaster->GetTypeId() == TYPEID_PLAYER) ? (Player*)pCaster : (Player*)pTarget;

	plr->CastSpell(plr, 13258, true);
	return true;
}

bool EffectVariousDummySpells(Object* pCaster, uint32 spellId, SpellEffectIndex effIndex, Object* pTarget) {
	switch(spellId) {
	case 13006:
		return EffectGnomeShrink(pCaster, spellId, effIndex, pTarget);

	case 23133:
		return EffectGnomeChicken(pCaster, spellId, effIndex, pTarget);

	case 23134:
		return EffectGoblinBomb(pCaster, spellId, effIndex, pTarget);

	default:
		return false;
	}
}

class GoblinBombAI : public ScriptedAI
{
public:
	GoblinBombAI(Creature* pCreature) : ScriptedAI(pCreature) {
	}

	void UpdateAI(const uint32) {
		Unit* vic = m_creature->getVictim();
		if(vic) {
			debug_log("Has vicitm");
			if(m_creature->GetDistance(vic) < 10) {
				m_creature->CastSpell(vic, 22334, true);
				m_creature->ForcedDespawn();
			} else {
				debug_log("%f", m_creature->GetDistance(vic));
			}
		} else {
			m_creature->SelectHostileTarget();
		}
	}

	void Reset() { }
};

CreatureAI* getGoblinBombAI(Creature* pCreature) {
	return new GoblinBombAI(pCreature);
}


int random(int min, int max)
{
    return (int)((max-min) * rand()/(float)RAND_MAX +min);
}

uint32 ASHENBRINGER_SOUNDS[12] = 
{
	8906,
	8907,
	8908,
	8920,
	8921,
	8922,
	8923,
	8924,
	8925,
	8926,
	8927,
	8928,
};


bool AshbringerEffect(Object* pCaster, uint32, SpellEffectIndex, Object* pTarget)
{
	static std::map<uint64, uint32> soundMap;

	uint64 guid = pCaster->GetGUID();
	if(soundMap.find(guid) == soundMap.end())
		soundMap[guid] = 0;
	else
	{
		soundMap[guid] += 1;
		soundMap[guid] = soundMap[guid] < 12 ? soundMap[guid]  : 0;
	}


	if(random(1, 120) == 5)
		((Unit*)(pTarget))->PlayDirectSound(ASHENBRINGER_SOUNDS[soundMap[guid]]);

	return false;
}


enum : uint32
{
	DEATH_RAY_ITEM_SPELL = 13278,
	DEATH_RAY_CAST = 13279,
	DEATH_RAY_DELAYED_DUMMY = 13280,
	DEATH_RAY_PERIODIC_DAMAGE = 13493,
};

bool EffectGnomishDeathRay(Object* pCaster, uint32 spellId, SpellEffectIndex, Object* pTarget)
{
	if((pCaster->GetTypeId() != TYPEID_ITEM && pCaster->GetTypeId() != TYPEID_PLAYER) || (pTarget->GetTypeId() != TYPEID_PLAYER && pTarget->GetTypeId() != TYPEID_UNIT))
		return false;
	
	switch(spellId)
	{
	case DEATH_RAY_ITEM_SPELL:
		{
			Item* item = static_cast<Item*>(pCaster);
			item->GetOwner()->CastSpell(item->GetOwner(), DEATH_RAY_PERIODIC_DAMAGE, true);
		} break;
	case DEATH_RAY_DELAYED_DUMMY:
		{
			int32 dmg = 600 + random(0, 800);
			Player* plr = static_cast<Player*>(pCaster);
			if(plr == nullptr)
				return false;

			Unit* target = plr->GetMap()->GetUnit(plr->GetTargetGuid());
			if(target == nullptr)
				return false;

			plr->CastCustomSpell(target, DEATH_RAY_CAST, &dmg, nullptr, nullptr, true);
		} break;
	}

	return false;
}

enum : uint32
{
	SPELL_AXE_FLURRY_DUMMY = 24019,
	SPELL_AXE_FLURRY_TRIGGERED = 24020,
};

bool EffectAxeFlurryDummy(Object* pCaster, uint32 spellId, SpellEffectIndex, Object* pTarget)
{
	if (pCaster->GetTypeId() != TYPEID_UNIT)
		return false;

	auto caster = static_cast<Unit*>(pCaster);
	auto target = caster->SelectRandomUnfriendlyTarget(nullptr, 30.f);

	if (!target || !caster->CanCast(target, SPELL_AXE_FLURRY_TRIGGERED, true))
		return false;

	caster->CastSpell(target, SPELL_AXE_FLURRY_TRIGGERED, true);

	return false;
}


void AddSC_spell_scripts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "spell_dummy_npc";
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_spell_dummy_npc;
    pNewScript->RegisterSelf();

	pNewScript = new Script();
	pNewScript->Name = "spell_various_dummy";
	pNewScript->SpellIds.push_back(13006);
	pNewScript->SpellIds.push_back(23133);
	//pNewScript->SpellIds.push_back(23134);
	pNewScript->pEffectDummy = EffectVariousDummySpells;
	pNewScript->RegisterAsSpellScript();

	pNewScript = new Script();
	pNewScript->Name = "npc_goblin_bomb";
	pNewScript->GetAI = getGoblinBombAI;
	//pNewScript->RegisterSelf();

	pNewScript = new Script();
	pNewScript->Name = "ab_effect_000";
	pNewScript->SpellIds.push_back(28441);
	pNewScript->pEffectDummy = AshbringerEffect;
	pNewScript->RegisterAsSpellScript();

	pNewScript = new Script();
	pNewScript->Name = "gnomish_death_ray";
	pNewScript->SpellIds.push_back(DEATH_RAY_ITEM_SPELL);
	pNewScript->SpellIds.push_back(DEATH_RAY_DELAYED_DUMMY);
	pNewScript->pEffectDummy = EffectGnomishDeathRay;
	pNewScript->RegisterAsSpellScript();

	pNewScript = new Script();
	pNewScript->Name = "axe_flurry_dummy";
	pNewScript->SpellIds.push_back(SPELL_AXE_FLURRY_DUMMY);
	pNewScript->pEffectDummy = EffectAxeFlurryDummy;
	pNewScript->RegisterAsSpellScript();
}
