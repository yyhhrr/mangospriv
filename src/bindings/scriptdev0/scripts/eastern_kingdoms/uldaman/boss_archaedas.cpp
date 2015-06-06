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
SDName: Boss_Archaedas
SD%Complete: 60
SDComment: Need correct way to deal with awaken vault and guardian spells, waiting for additions in mangos for them (target combination 22/7)
SDCategory: Uldaman
EndScriptData */

#include "precompiled.h"
#include "uldaman.h"

enum
{
    SPELL_GROUND_TREMOR             = 6524,

    SPELL_AWAKEN_EARTHEN_GUARDIAN   = 10252,
    SPELL_AWAKEN_VAULT_WARDER       = 10258,
    SPELL_AWAKEN_EARTHEN_DWARF      = 10259,

    SPELL_ARCHAEDAS_AWAKEN_VISUAL   = 10347,

    SAY_AGGRO                       = -1070001,
    SAY_AWAKE_GUARDIANS             = -1070002,
    SAY_AWAKE_WARDERS               = -1070003,
    SAY_UNIT_SLAIN					= -1070004,
    EMOTE_BREAKS_FREE				= -1070005,
};

struct MANGOS_DLL_DECL boss_archaedasAI : public ScriptedAI
{
    boss_archaedasAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_uldaman*)pCreature->GetInstanceData();
        Reset();
    }

    instance_uldaman* m_pInstance;

    uint32 m_uiAwakeningTimer;
    uint32 m_uiAwakeDwarfTimer;
    uint8 m_uiSubevent;
    bool m_bDwarvesAwaken;
    bool m_bGuardiansAwaken;
    bool m_bWardersAwaken;

    void Reset()
    {
        m_uiAwakeningTimer  = 1000;
        m_uiSubevent        = 0;
        m_uiAwakeDwarfTimer = 10000;
        m_bGuardiansAwaken  = false;
        m_bWardersAwaken    = false;
        m_bDwarvesAwaken    = false;

		m_creature->CastSpell(m_creature, SPELL_STONED, true);
    }

    void Aggro(Unit*)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAEDAS, IN_PROGRESS);
    }

    void KilledUnit(Unit*)
    {
        DoScriptText(SAY_UNIT_SLAIN, m_creature);
    }

    void JustDied(Unit*)
    {
        // open door to vault (handled by instance script)
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAEDAS, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAEDAS, FAIL);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry*)
    {
        if (pTarget->GetTypeId() != TYPEID_PLAYER)
        {
            if (pTarget->HasAura(SPELL_FREEZE_ANIM))
            {
                pTarget->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);

                if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTarget->SetInCombatWith(pUnit);
                    pTarget->AddThreat(pUnit);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // so many things are based in this script on instance data
        // so if we don't have access to it better do nothing
        if (!m_pInstance)
            return;

        // OOC Intro part triggered by Altar activation
        if (m_pInstance->GetData(TYPE_ARCHAEDAS) == SPECIAL)
        {
            if (m_uiAwakeningTimer <= uiDiff)
            {
                switch (m_uiSubevent)
                {
                    case 0:
                        DoCastSpellIfCan(m_creature, SPELL_ARCHAEDAS_AWAKEN_VISUAL);
                        m_uiAwakeningTimer = 2000;
                        break;
                    case 1:
                        DoScriptText(EMOTE_BREAKS_FREE, m_creature);
                        m_uiAwakeningTimer = 3000;
                        break;
                    case 2:
                        DoScriptText(SAY_AGGRO, m_creature, NULL);
						m_creature->RemoveAura(SPELL_STONED, SpellEffectIndex::EFFECT_INDEX_0);
                        m_uiAwakeningTimer = 5000;
                        break;
                    case 3:
                        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_pInstance->GetGuid(DATA_EVENT_STARTER)))
                            AttackStart(pPlayer);
                        else
                            EnterEvadeMode();
                        break;
                    default:
                        break;
                }

                ++m_uiSubevent;
            }
            else
                m_uiAwakeningTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Awake random Dwarf
        if (!m_bDwarvesAwaken && m_creature->GetHealthPercent() >= 33.0f)
        {
            if (m_uiAwakeDwarfTimer <= uiDiff)
            {
                if (Creature* pEarthen = m_pInstance->GetClosestDwarfNotInCombat(m_creature, PHASE_ARCHA_1))
                {
                    if (DoCastSpellIfCan(pEarthen, SPELL_AWAKEN_EARTHEN_DWARF) == CAST_OK)
					{
						pEarthen->Attack(m_creature->getVictim(), true);
						pEarthen->RemoveAura(SPELL_STONED, SpellEffectIndex::EFFECT_INDEX_0);
                        m_uiAwakeDwarfTimer = urand(9000, 12000);
					}
                }
                else
                    m_bDwarvesAwaken = true;
            }
            else
                m_uiAwakeDwarfTimer -= uiDiff;
        }

        //Awake Earthen Guardians
        if (!m_bGuardiansAwaken && m_creature->GetHealthPercent() <= 66.0f)
        {
            if (Creature* pGuard = m_pInstance->GetClosestDwarfNotInCombat(m_creature, PHASE_ARCHA_2))
            {
				if (DoCastSpellIfCan(pGuard, SPELL_AWAKEN_EARTHEN_GUARDIAN) == CAST_OK)
				{
					pGuard->Attack(m_creature->getVictim(), true);
					pGuard->RemoveAura(SPELL_STONED, SpellEffectIndex::EFFECT_INDEX_0);
                    DoScriptText(SAY_AWAKE_GUARDIANS, m_creature);
                    m_bGuardiansAwaken = true;
                }
            }
        }

        // Awake Warders
        if (!m_bWardersAwaken && m_creature->GetHealthPercent() <= 33.0f)
        {
            if (Creature* pWarder = m_pInstance->GetClosestDwarfNotInCombat(m_creature, PHASE_ARCHA_3))
            {
				if (DoCastSpellIfCan(pWarder, SPELL_AWAKEN_VAULT_WARDER) == CAST_OK)
				{
					pWarder->Attack(m_creature->getVictim(), true);
					pWarder->RemoveAura(SPELL_STONED, SpellEffectIndex::EFFECT_INDEX_0);
                    DoScriptText(SAY_AWAKE_WARDERS, m_creature);
                    m_bWardersAwaken = true;
                }
            }
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_archaeras_addAI : public ScriptedAI
{
    npc_archaeras_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset()
    {
    }

    void UpdateAI(const uint32)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_archaedas(Creature* pCreature)
{
    return new boss_archaedasAI(pCreature);
}

CreatureAI* GetAI_npc_archaeras_add(Creature* pCreature)
{
    return new npc_archaeras_addAI(pCreature);
}

void AddSC_boss_archaedas()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_archaedas";
    pNewScript->GetAI = &GetAI_boss_archaedas;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_archaeras_add";
    pNewScript->GetAI = &GetAI_npc_archaeras_add;
    pNewScript->RegisterSelf();
}
