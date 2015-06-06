/*
 * Copyright (C) 2006-2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Durotar
SD%Complete: 100
SDComment: Quest support: 5441
SDCategory: Durotar
EndScriptData */

/* ContentData
npc_lazy_peon
EndContentData */

#include "precompiled.h"
#include "Spell.h"

/*######
## npc_lazy_peon
######*/

enum
{
    SAY_PEON_AWAKE_1        = -1000639,
    SAY_PEON_AWAKE_2        = -1000640,

    SPELL_PEON_SLEEP        = 17743,
    SPELL_AWAKEN_PEON       = 19938,

    NPC_SLEEPING_PEON       = 10556,
    GO_LUMBERPILE           = 175784,
};

struct MANGOS_DLL_DECL npc_lazy_peonAI : public ScriptedAI
{
private:
	WorldLocation mLastSleep;

public:

    npc_lazy_peonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset ();
        m_uiStopSleepingTimer = urand(30000, 120000);
		pCreature->GetPosition(mLastSleep);
    }

    uint32 m_uiResetSleepTimer;
    uint32 m_uiStopSleepingTimer;

    void Reset()
    {
        m_uiResetSleepTimer = 0;
        m_uiStopSleepingTimer = urand(90000, 120000);
    }

    void StartLumbering(Unit* pInvoker)
    {
        m_uiStopSleepingTimer = 0;
        if (GameObject* pLumber = GetClosestGameObjectWithEntry(m_creature, GO_LUMBERPILE, 25.0f))
        {
            m_creature->RemoveAurasDueToSpell(SPELL_PEON_SLEEP);

            float fX, fY, fZ;
            m_creature->SetWalk(false);
            pLumber->GetContactPoint(m_creature, fX, fY, fZ, CONTACT_DISTANCE);
			m_creature->GetPosition(mLastSleep);

            if (pInvoker->GetTypeId() == TYPEID_PLAYER)
            {
                DoScriptText(SAY_PEON_AWAKE_1, m_creature);
                ((Player*)pInvoker)->KilledMonsterCredit(m_creature->GetEntry(), m_creature->GetObjectGuid());
                m_creature->GetMotionMaster()->MovePoint(1, fX, fY, fZ);
            }
            else
                m_creature->GetMotionMaster()->MovePoint(2, fX, fY, fZ);
        }
        else
            error_log("SD2: No GameObject of entry %u was found in range or something really bad happened.", GO_LUMBERPILE);
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || !uiPointId)
            return;

		if(uiPointId == 3) {
			m_creature->CastSpell(m_creature, SPELL_PEON_SLEEP, false);
			m_uiStopSleepingTimer = urand(90000, 120000);
			return;
		}

        m_creature->HandleEmote(EMOTE_STATE_WORK_CHOPWOOD);
        m_uiResetSleepTimer = uiPointId == 1 ? 80000 : urand(30000, 60000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiResetSleepTimer)
        {
            if (m_uiResetSleepTimer <= uiDiff)
            {
                DoScriptText(SAY_PEON_AWAKE_2, m_creature);
				m_creature->HandleEmote(EMOTE_STATE_NONE);
				m_creature->GetMotionMaster()->MovePoint(3, mLastSleep.coord_x, mLastSleep.coord_y, mLastSleep.coord_z);
                m_uiResetSleepTimer = 0;
            }
            else
                m_uiResetSleepTimer -= uiDiff;
        }

        if (m_uiStopSleepingTimer)
        {
            if (m_uiStopSleepingTimer <= uiDiff)
                StartLumbering(m_creature);
            else
                m_uiStopSleepingTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_lazy_peon(Creature* pCreature)
{
    return new npc_lazy_peonAI(pCreature);
}

bool EffectDummyCreature_lazy_peon_awake(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    if (uiSpellId == SPELL_AWAKEN_PEON && uiEffIndex == EFFECT_INDEX_0)
    {
        if (!pCreatureTarget->HasAura(SPELL_PEON_SLEEP) || pCaster->GetTypeId() != TYPEID_PLAYER || pCreatureTarget->GetEntry() != NPC_SLEEPING_PEON) {
			if(pCaster->GetTypeId() == TYPEID_PLAYER) {
				Spell::SendCastResult((Player*)pCaster, uiSpellId, SpellCastResult::SPELL_FAILED_BAD_TARGETS);
			}

            return true;
		}

        if (npc_lazy_peonAI* pPeonAI = dynamic_cast<npc_lazy_peonAI*>(pCreatureTarget->AI()))
            pPeonAI->StartLumbering(pCaster);
        return true;
    }

    return false;
}

void AddSC_durotar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_lazy_peon";
    pNewScript->GetAI = &GetAI_npc_lazy_peon;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_lazy_peon_awake;
    pNewScript->RegisterSelf();
}