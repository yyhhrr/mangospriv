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
SDName: bug_trio
SD%Complete: 75
SDComment: Summon Player spell NYI; Poison Cloud damage spell NYI; Timers need adjustments
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

enum
{
    // kri
    SPELL_CLEAVE            = 26350,
    SPELL_TOXIC_VOLLEY      = 25812,
    SPELL_SUMMON_CLOUD      = 26590,            // summons 15933
    SPELL_TOXIC_VAPORS      = 25786,

    // vem
    SPELL_CHARGE            = 26561,
    SPELL_VENGEANCE         = 25790,
    SPELL_KNOCKBACK         = 26027,

    // yauj
    SPELL_HEAL              = 25807,
    SPELL_FEAR              = 26580,
	SPELL_SUMMON_BROOD      = 25789, //On Death

    SPELL_ENRAGEHARD        = 28798,

    NPC_KRI                 = 15511,
    NPC_YAUJ                = 15543,
    NPC_VEM                 = 15544,
    NPC_YAUJ_BROOD          = 15621
};

void RespawnDeadBugs(Creature* pSearcher)
{
    bool Evade = false;

    std::list<Creature*> pList;
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_KRI, 1000.f);
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_VEM, 1000.f);
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_YAUJ, 1000.f);

    for (auto itr : pList)
        if (!itr->isAlive())
            itr->Respawn();
}

void SetInCombatIfNecessary(Creature* pSearcher)
{
    std::list<Creature*> pList;
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_KRI, 1000.f);
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_VEM, 1000.f);
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_YAUJ, 1000.f);

    for (auto itr : pList)
        if (itr->isInCombat())
        {
            pSearcher->SetInCombatWithZone();
            return;
        }
}

void EvadeAllBugs(Creature* pSearcher)
{
    std::list<Creature*> pList;
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_KRI, 1000.f);
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_VEM, 1000.f);
    GetCreatureListWithEntryInGrid(pList, pSearcher, NPC_YAUJ, 1000.f);

    for (auto itr : pList)
        if (!itr->IsInEvadeMode())
            itr->Evade();
}

struct MANGOS_DLL_DECL boss_kriAI : public ScriptedAI
{
    boss_kriAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiCleaveTimer;
    uint32 m_uiToxicVolleyTimer;
    uint32 m_uiEnrageTimer;

    void Reset()
    {
        m_uiCleaveTimer      = urand(4000, 8000);
        m_uiToxicVolleyTimer = urand(6000, 12000);
        m_uiEnrageTimer      = 15 * MINUTE * IN_MILLISECONDS;
    }

    void JustRespawned()
    {
        SetInCombatIfNecessary(m_creature);
    }

    void EnterCombat(Unit* /*pEnemy*/)
    {
        RespawnDeadBugs(m_creature);
    }

    void JustDied(Unit* /*pKiller*/)
    {
        // poison cloud on death
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_CLOUD, CAST_TRIGGERED);

        if (!m_pInstance)
            return;

        // If the other 2 bugs are still alive, make unlootable
        if (m_pInstance->GetData(TYPE_BUG_TRIO) != DONE)
        {
            m_pInstance->SetData(TYPE_BUG_TRIO, SPECIAL);
            if (m_pInstance->GetData(TYPE_BUG_TRIO) != DONE)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            else
            {
                Map *m = m_creature->GetMap();
                if (m->IsDungeon())
                    if (Player* plr = m_pInstance->GetPlayerInMap(false, false))
                        ((DungeonMap *)m)->PermBindAllPlayers(plr);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BUG_TRIO, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Cleave_Timer
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(5000, 12000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // ToxicVolley_Timer
        if (m_uiToxicVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TOXIC_VOLLEY) == CAST_OK)
                m_uiToxicVolleyTimer = urand(6000, 10000);
        }
        else
            m_uiToxicVolleyTimer -= uiDiff;

        if (m_uiEnrageTimer && m_uiEnrageTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGEHARD, CAST_TRIGGERED);
            m_uiEnrageTimer = 0;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (EnterEvadeIfOutOfCombatArea(uiDiff))
            EvadeAllBugs(m_creature);
    }
};

struct MANGOS_DLL_DECL boss_vemAI : public ScriptedAI
{
    boss_vemAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiChargeTimer;
    uint32 m_uiKnockBackTimer;
    uint32 m_uiEnrageTimer;

    void Reset()
    {
        m_creature->SetWalk(false);

        m_uiChargeTimer     = urand(15000, 27000);
        m_uiKnockBackTimer  = urand(8000, 20000);
        m_uiEnrageTimer     = 15 * MINUTE * IN_MILLISECONDS;
    }

    void JustRespawned()
    {
        SetInCombatIfNecessary(m_creature);
    }

    void EnterCombat(Unit* /*pEnemy*/)
    {
        RespawnDeadBugs(m_creature);
    }

    void JustDied(Unit* /*pKiller*/)
    {
        // Enrage the other bugs
        // DoCastSpellIfCan(m_creature, SPELL_VENGEANCE, CAST_TRIGGERED);

        // since faction 14 is not friendly towards 14 we need this
        if (Creature* kri = GetClosestCreatureWithEntry(m_creature, NPC_KRI, 1000.f))
            kri->CastSpell(kri, SPELL_VENGEANCE, true);
        if (Creature* yauj = GetClosestCreatureWithEntry(m_creature, NPC_YAUJ, 1000.f))
            yauj->CastSpell(yauj, SPELL_VENGEANCE, true);

        if (!m_pInstance)
            return;

        // If the other 2 bugs are still alive, make unlootable
        if (m_pInstance->GetData(TYPE_BUG_TRIO) != DONE)
        {
            m_pInstance->SetData(TYPE_BUG_TRIO, SPECIAL);
            if (m_pInstance->GetData(TYPE_BUG_TRIO) != DONE)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            else
            {
                Map *m = m_creature->GetMap();
                if (m->IsDungeon())
                    if (Player* plr = m_pInstance->GetPlayerInMap(false, false))
                        ((DungeonMap *)m)->PermBindAllPlayers(plr);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BUG_TRIO, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Charge_Timer
        if (m_uiChargeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_CHARGE, SELECT_FLAG_NOT_IN_MELEE_RANGE))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CHARGE) == CAST_OK)
                    m_uiChargeTimer = urand(8000, 16000);
            }
        }
        else
            m_uiChargeTimer -= uiDiff;

        // KnockBack_Timer
        if (m_uiKnockBackTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_KNOCKBACK) == CAST_OK)
            {
                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(), -80);

                m_uiKnockBackTimer = urand(15000, 25000);
            }
        }
        else
            m_uiKnockBackTimer -= uiDiff;

        if (m_uiEnrageTimer && m_uiEnrageTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGEHARD, CAST_TRIGGERED);
            m_uiEnrageTimer = 0;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (EnterEvadeIfOutOfCombatArea(uiDiff))
            EvadeAllBugs(m_creature);
    }
};

struct MANGOS_DLL_DECL boss_yaujAI : public ScriptedAI
{
    boss_yaujAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiHealTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiEnrageTimer;

    void Reset()
    {
        m_uiHealTimer       = urand(25000, 40000);
        m_uiFearTimer       = urand(12000, 24000);
        m_uiEnrageTimer     = 15 * MINUTE * IN_MILLISECONDS;
    }

    void JustRespawned()
    {
        SetInCombatIfNecessary(m_creature);
    }

    void EnterCombat(Unit* /*pEnemy*/)
    {
        RespawnDeadBugs(m_creature);
    }

    void JustDied(Unit*)
    {
        // Spawn 10 yauj brood on death
        float fX, fY, fZ;
        for (int i = 0; i < 10; ++i)
        {
            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 10.0f, fX, fY, fZ);
            m_creature->SummonCreature(NPC_YAUJ_BROOD, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
        }

        if (!m_pInstance)
            return;

        // If the other 2 bugs are still alive, make unlootable
        if (m_pInstance->GetData(TYPE_BUG_TRIO) != DONE)
        {
            m_pInstance->SetData(TYPE_BUG_TRIO, SPECIAL);
            if (m_pInstance->GetData(TYPE_BUG_TRIO) != DONE)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            else
            {
                Map *m = m_creature->GetMap();
                if (m->IsDungeon())
                    if (Player* plr = m_pInstance->GetPlayerInMap(false, false))
                        ((DungeonMap *)m)->PermBindAllPlayers(plr);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BUG_TRIO, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Fear_Timer
        if (m_uiFearTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FEAR) == CAST_OK)
            {
                auto Targets = m_creature->getThreatManager().getThreatList();
                for (auto itr : Targets)
                {
                    if (Unit* pUnit = m_creature->GetMap()->GetUnit(itr->getUnitGuid()))
                        if (pUnit->IsWithinDist(m_creature, 45.f))
                            m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
                        else
                            m_creature->getThreatManager().modifyThreatPercent(pUnit, -80);
                }
                // Select new aggro Target
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;
                m_uiFearTimer = 20000;
            }
        }
        else
            m_uiFearTimer -= uiDiff;

        // Heal
        if (m_uiHealTimer < uiDiff)
        {
#if AQ_PATCH >= 10
            if (Unit* pTarget = DoSelectLowestHpFriendly(100.0f))
#else
            if (Unit* pTarget = m_creature)
#endif
            {
                if (DoCastSpellIfCan(pTarget, SPELL_HEAL) == CAST_OK)
                    m_uiHealTimer = urand(15000, 30000);
            }
        }
        else
            m_uiHealTimer -= uiDiff;

        if (m_uiEnrageTimer && m_uiEnrageTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGEHARD, CAST_TRIGGERED);
            m_uiEnrageTimer = 0;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (EnterEvadeIfOutOfCombatArea(uiDiff))
            EvadeAllBugs(m_creature);
    }
};

struct npc_kri_poison_cloudAI : public ScriptedAI
{
    npc_kri_poison_cloudAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset()
    {
        DoCastSpellIfCan(m_creature, SPELL_TOXIC_VAPORS, CAST_TRIGGERED);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI(const uint32 uiDiff)
    {

    }
};

CreatureAI* GetAI_npc_kri_poison_cloud(Creature* pCreature)
{
    return new npc_kri_poison_cloudAI(pCreature);
}

CreatureAI* GetAI_boss_yauj(Creature* pCreature)
{
    return new boss_yaujAI(pCreature);
}

CreatureAI* GetAI_boss_vem(Creature* pCreature)
{
    return new boss_vemAI(pCreature);
}

CreatureAI* GetAI_boss_kri(Creature* pCreature)
{
    return new boss_kriAI(pCreature);
}

void AddSC_bug_trio()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_kri";
    pNewScript->GetAI = &GetAI_boss_kri;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_vem";
    pNewScript->GetAI = &GetAI_boss_vem;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_yauj";
    pNewScript->GetAI = &GetAI_boss_yauj;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_kri_poison_cloud";
    pNewScript->GetAI = &GetAI_npc_kri_poison_cloud;
    pNewScript->RegisterSelf();
}
