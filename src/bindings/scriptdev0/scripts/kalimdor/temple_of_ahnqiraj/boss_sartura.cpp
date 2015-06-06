/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Sartura
SD%Complete: 95
SDComment:
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

enum
{
    NPC_GUARD                   = 15984,

    SAY_AGGRO                   = -1531008,
    SAY_SLAY                    = -1531009,
    SAY_DEATH                   = -1531010,

    SPELL_WHIRLWIND             = 26083,
    SPELL_ENRAGE                = 8269,
    SPELL_ENRAGEHARD            = 28798,

    // Guard Spell
    SPELL_WHIRLWIND_ADD         = 26038,
    SPELL_KNOCKBACK             = 26027,
};

void EvadeAdds(Creature* pSearcher)
{
    std::list<Creature*> pList;
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_GUARD, 1000.f);
    
    for (auto itr : pList)
        if (!itr->IsInEvadeMode())
        {
            itr->NearTeleportTo(-8344.f, 1598.f, -30.81f, 1.3f);
            itr->Evade();
        }
}

struct MANGOS_DLL_DECL boss_sarturaAI : public ScriptedAI
{
    boss_sarturaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiWhirlWindTimer;
    uint32 m_uiWhirlWindRandomTimer;
    uint32 m_uiWhirlWindEndTimer;
    uint32 m_uiAggroResetTimer;
    uint32 m_uiAggroResetEndTimer;
    uint32 m_uiEnrageHardTimer;

    bool m_bIsEnraged;

    void Reset()
    {
        m_creature->SetWalk(false);

        m_uiWhirlWindTimer = 30000;
        m_uiWhirlWindRandomTimer = urand(3000, 7000);
        m_uiWhirlWindEndTimer = 0;
        m_uiAggroResetTimer = urand(15000, 20000);
        m_uiAggroResetEndTimer = 0;
        m_uiEnrageHardTimer = 10 * 60000;

        m_bIsEnraged = false;
    }

    void Aggro(Unit*)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTURA, IN_PROGRESS);
    }

    void KilledUnit(Unit* /*pVictim*/)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* /*pKiller*/)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTURA, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTURA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiWhirlWindEndTimer)                          // Is in Whirlwind
        {
            auto taunts = m_creature->GetAurasByType(AuraType::SPELL_AURA_MOD_TAUNT);
            for (auto itr : taunts)
                if (Unit* taunter = itr->GetCaster())
                    m_creature->FixateTarget(taunter);

            // While whirlwind, switch to random targets often
            if (m_uiWhirlWindRandomTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->FixateTarget(pTarget);

                m_uiWhirlWindRandomTimer = urand(3000, 7000);
            }
            else
                m_uiWhirlWindRandomTimer -= uiDiff;

            // End Whirlwind Phase
            if (m_uiWhirlWindEndTimer <= uiDiff)
            {
                m_creature->FixateTarget(NULL);
                m_uiWhirlWindEndTimer = 0;
                m_uiWhirlWindTimer = urand(10000, 15000);
            }
            else
                m_uiWhirlWindEndTimer -= uiDiff;
        }
        else // if (!m_uiWhirlWindEndTimer)                 // Is not in whirlwind
        {
            // Enter Whirlwind Phase
            if (m_uiWhirlWindTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND) == CAST_OK)
                {
                    m_uiWhirlWindEndTimer = 15000;
                    m_uiWhirlWindRandomTimer = 500;
                }
            }
            else
                m_uiWhirlWindTimer -= uiDiff;

            // Aquire a new target sometimes
            if (!m_uiAggroResetEndTimer)                    // No random target picket
            {
                if (m_uiAggroResetTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                        m_creature->FixateTarget(pTarget);

                    m_uiAggroResetEndTimer = 8000;
                }
                else
                    m_uiAggroResetTimer -= uiDiff;
            }
            else                                            // Reset from recent random target
            {
                // Remove remaining taunts
                if (m_uiAggroResetEndTimer <= uiDiff)
                {
                    m_creature->FixateTarget(NULL);
					DoResetThreat();
                    m_uiAggroResetEndTimer = 0;
                    m_uiAggroResetTimer = urand(30000, 35000);
                }
                else
                    m_uiAggroResetEndTimer -= uiDiff;
            }
        }

        // If she is 20% enrage
        if (!m_bIsEnraged && m_creature->GetHealthPercent() <= 20.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE, m_uiWhirlWindEndTimer ? CAST_TRIGGERED : 0) == CAST_OK)
                m_bIsEnraged = true;
        }

        // After 10 minutes hard enrage
        if (m_uiEnrageHardTimer)
        {
            if (m_uiEnrageHardTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ENRAGEHARD, m_uiWhirlWindEndTimer ? CAST_TRIGGERED : 0) == CAST_OK)
                    m_uiEnrageHardTimer = 0;
            }
            else
                m_uiEnrageHardTimer -= uiDiff;
        }

        // No melee damage while in whirlwind
        if (!m_uiWhirlWindEndTimer)
            DoMeleeAttackIfReady();

        if (EnterEvadeIfOutOfCombatArea(uiDiff))
        {
            m_creature->NearTeleportTo(-8344.f, 1598.f, -30.81f, 1.3f);
            EvadeAdds(m_creature);
        }
    }
};

struct MANGOS_DLL_DECL mob_sartura_royal_guardAI : public ScriptedAI
{
    mob_sartura_royal_guardAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiWhirlWindTimer;
    uint32 m_uiWhirlWindRandomTimer;
    uint32 m_uiWhirlWindEndTimer;
    uint32 m_uiAggroResetTimer;
    uint32 m_uiAggroResetEndTimer;
    uint32 m_uiKnockBackTimer;

    void Reset()
    {
        m_creature->SetWalk(false);

        m_uiWhirlWindTimer = urand(5000, 7000);
        m_uiWhirlWindRandomTimer = urand(3000, 7000);
        m_uiWhirlWindEndTimer = 0;
		m_uiAggroResetTimer = urand(15000, 20000);
        m_uiAggroResetEndTimer = 0;
        m_uiKnockBackTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiWhirlWindEndTimer)                          // Is in Whirlwind
        {
            auto taunts = m_creature->GetAurasByType(AuraType::SPELL_AURA_MOD_TAUNT);
            for (auto itr : taunts)
                if (Unit* taunter = itr->GetCaster())
                    m_creature->FixateTarget(taunter);

            // While whirlwind, switch to random targets often
            if (m_uiWhirlWindRandomTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->FixateTarget(pTarget);

                m_uiWhirlWindRandomTimer = urand(3000, 7000);
            }
            else
                m_uiWhirlWindRandomTimer -= uiDiff;

            // End Whirlwind Phase
            if (m_uiWhirlWindEndTimer <= uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
                m_creature->FixateTarget(NULL);
                m_uiWhirlWindEndTimer = 0;
                m_uiWhirlWindTimer = urand(25000, 40000);
            }
            else
                m_uiWhirlWindEndTimer -= uiDiff;
        }
        else // if (!m_uiWhirlWindEndTimer)                 // Is not in Whirlwind
        {
            // Enter Whirlwind Phase
            if (m_uiWhirlWindTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND_ADD) == CAST_OK)
                {
                    m_uiWhirlWindEndTimer = 8000;
                    m_uiWhirlWindRandomTimer = 500;
                }
            }
            else
                m_uiWhirlWindTimer -= uiDiff;

            // Aquire a new target sometimes
            if (!m_uiAggroResetEndTimer)                    // No random target picket
            {
                if (m_uiAggroResetTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                        m_creature->FixateTarget(pTarget);

                    m_uiAggroResetEndTimer = 8000;
                }
                else
                    m_uiAggroResetTimer -= uiDiff;
            }
            else                                            // Reset from recent random target
            {
                if (m_uiAggroResetEndTimer <= uiDiff)
                {
                    m_creature->FixateTarget(NULL);
                    DoResetThreat();
                    m_uiAggroResetEndTimer = 0;
					m_uiAggroResetTimer = urand(30000, 35000);
                }
                else
                    m_uiAggroResetEndTimer -= uiDiff;
            }

            // Knockback nearby enemies
            if (m_uiKnockBackTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_KNOCKBACK) == CAST_OK)
                    m_uiKnockBackTimer = urand(10000, 20000);
            }
            else
                m_uiKnockBackTimer -= uiDiff;
        }

        // No melee damage while in whirlwind
        if (!m_uiWhirlWindEndTimer)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sartura(Creature* pCreature)
{
    return new boss_sarturaAI(pCreature);
}

CreatureAI* GetAI_mob_sartura_royal_guard(Creature* pCreature)
{
    return new mob_sartura_royal_guardAI(pCreature);
}

void AddSC_boss_sartura()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_sartura";
    pNewScript->GetAI = &GetAI_boss_sartura;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_sartura_royal_guard";
    pNewScript->GetAI = &GetAI_mob_sartura_royal_guard;
    pNewScript->RegisterSelf();
}