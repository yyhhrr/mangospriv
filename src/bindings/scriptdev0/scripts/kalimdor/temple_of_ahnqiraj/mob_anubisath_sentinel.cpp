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
SDName: mob_anubisath_sentinel
SD%Complete: 75
SDComment: Abilities selection needs further improvements. Shadow storm is not properly implemented in core it should only target ppl outside of melee range.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

enum
{
    EMOTE_GENERIC_FRENZY            = -1000002,

    SPELL_PERIODIC_MANA_BURN        = 812,
    SPELL_MENDING                   = 2147,
    SPELL_PERIODIC_SHADOW_STORM     = 2148,
    SPELL_PERIODIC_THUNDERCLAP      = 2834,
    SPELL_MORTAL_STRIKE             = 9347,
    SPELL_FIRE_ARCANE_REFLECT       = 13022,
    SPELL_SHADOW_FROST_REFLECT      = 19595,
    SPELL_PERIODIC_KNOCK_AWAY       = 21737,
    SPELL_THORNS                    = 25777,

    SPELL_ENRAGE                    = 8599,

    MAX_BUDDY                       = 4
};

struct MANGOS_DLL_DECL npc_anubisath_sentinelAI : public ScriptedAI
{
    npc_anubisath_sentinelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_lAssistList.clear();
        Reset();
    }

    uint32 m_uiMyAbility;
    bool m_bEnraged;

    GuidList m_lAssistList;

    void Reset()
    {
        m_uiMyAbility = 0;
        m_bEnraged = false;
    }

    void GetAIInformation(ChatHandler& reader)
    {
        if (m_lAssistList.empty())
            reader.PSendSysMessage("Anubisath Sentinel - group not assigned, will be assigned OnAggro");
        if (m_lAssistList.size() == MAX_BUDDY)
            reader.PSendSysMessage("Anubisath Sentinel - proper group found");
        else
            reader.PSendSysMessage("Anubisath Sentinel - not correct number of mobs for group found. Number found %u, should be %u", m_lAssistList.size(), MAX_BUDDY);
    }

    void JustReachedHome()
    {
        for (GuidList::const_iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
        {
            if (*itr == m_creature->GetObjectGuid())
                continue;

            if (Creature* pBuddy = m_creature->GetMap()->GetCreature(*itr))
            {
                if (pBuddy->isDead())
                    pBuddy->Respawn();
            }
        }

        m_creature->RemoveAllAuras();
    }

    void Aggro(Unit* pWho)
    {
        InitSentinelsNear(pWho);
        if (!m_uiMyAbility)
        {
            std::vector<uint32> abilities = CreateAbilityList();
            for (GuidList::const_iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
            {
                if (Creature* pBuddy = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pBuddy->isDead())
                        pBuddy->Respawn();

                   ((npc_anubisath_sentinelAI*)pBuddy->AI())->SetAbility(abilities);
                }
            }
        }
    }

    void JustDied(Unit* /*pKiller*/)
    {
        DoTransferAbility();
    }

    void SetAbility(std::vector<uint32> &abilities)
    {
        if (m_uiMyAbility || !abilities.size())
            return;

        m_uiMyAbility = abilities.back();
        DoCastSpellIfCan(m_creature, abilities.back(), CAST_TRIGGERED);
        abilities.pop_back();
    }

    std::vector<uint32> CreateAbilityList()
    {
        std::vector<uint32> abilities;

        abilities.push_back(SPELL_MENDING);
        abilities.push_back(SPELL_PERIODIC_KNOCK_AWAY);
        abilities.push_back(SPELL_PERIODIC_MANA_BURN);
        abilities.push_back(SPELL_FIRE_ARCANE_REFLECT);
        abilities.push_back(SPELL_SHADOW_FROST_REFLECT);
        abilities.push_back(SPELL_THORNS);
        abilities.push_back(SPELL_PERIODIC_THUNDERCLAP);
        abilities.push_back(SPELL_MORTAL_STRIKE);
        abilities.push_back(SPELL_PERIODIC_SHADOW_STORM);

        std::random_shuffle(abilities.begin(), abilities.end());

        return abilities;
    }

    void DoTransferAbility()
    {
        for (GuidList::const_iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
        {
            if (Creature* pBuddy = m_creature->GetMap()->GetCreature(*itr))
            {
                if (*itr == m_creature->GetObjectGuid())
                    continue;

                if (!pBuddy->isAlive())
                    continue;

                pBuddy->SetHealth(pBuddy->GetHealth() + pBuddy->GetMaxHealth() / 2);
                DoCastSpellIfCan(pBuddy, m_uiMyAbility, CAST_TRIGGERED);
            }
        }
    }

    void InitSentinelsNear(Unit* pTarget)
    {
        if (!m_lAssistList.empty())
        {
            for (GuidList::const_iterator itr = m_lAssistList.begin(); itr != m_lAssistList.end(); ++itr)
            {
                if (*itr == m_creature->GetObjectGuid())
                    continue;

                if (Creature* pBuddy = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pBuddy->isAlive())
                        pBuddy->AI()->AttackStart(pTarget);
                }
            }

            return;
        }

        std::list<Creature*> lAssistList;
        GetCreatureListWithEntryInGrid(lAssistList, m_creature, m_creature->GetEntry(), 90.0f);

        for (std::list<Creature*>::iterator iter = lAssistList.begin(); iter != lAssistList.end(); ++iter)
        {
            m_lAssistList.push_back((*iter)->GetObjectGuid());

            if ((*iter)->GetObjectGuid() == m_creature->GetObjectGuid())
                continue;

            (*iter)->AI()->AttackStart(pTarget);
        }

        if (m_lAssistList.size() != MAX_BUDDY)
            error_log("npc_anubisath_sentinel for %s found too few/too many buddies, expected %u.", m_creature->GetGuidStr().c_str(), MAX_BUDDY);
    }

    void UpdateAI(const uint32)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bEnraged && m_creature->GetHealthPercent() < 30.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
            {
                DoScriptText(EMOTE_GENERIC_FRENZY, m_creature);
                m_bEnraged = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_anubisath_sentinel(Creature* pCreature)
{
    return new npc_anubisath_sentinelAI(pCreature);
}

struct npc_obsidian_eradicatorAI : public ScriptedAI
{
    npc_obsidian_eradicatorAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uManaDrainTimer;

    void Reset()
    {
        m_creature->SetMaxPower(POWER_MANA, 0);
        m_uManaDrainTimer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetMaxPower(POWER_MANA, m_creature->GetCreatureInfo()->maxmana);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetPower(POWER_MANA) == m_creature->GetMaxPower(POWER_MANA))
            if (DoCastSpellIfCan(m_creature, 26458, CAST_TRIGGERED) == CAST_OK)
                m_creature->SetPower(POWER_MANA, 0);

        if (m_uManaDrainTimer <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_POWER_MANA))
                if (DoCastSpellIfCan(pTarget, 25671) == CAST_OK)
                    m_uManaDrainTimer = 2500;
        }
        else
            m_uManaDrainTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_obsidian_eradicator(Creature* pCreature)
{
    return new npc_obsidian_eradicatorAI(pCreature);
}

enum
{
    SPELL_IMPALE = 26025
};

struct npc_vekniss_guardianAI : public ScriptedAI
{
    npc_vekniss_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_charge;
    bool m_enraged;
    float m_fSpeedRate;
    std::list<ObjectGuid> lChargeList;

    void Reset()
    {
        m_charge = false;
        m_enraged = false;
        m_fSpeedRate = 1.f;
    }

    void Aggro(Unit* pWho)
    {
        lChargeList.clear();

        std::list<Creature*> lAssistList;
        GetCreatureListWithEntryInGrid(lAssistList, m_creature, m_creature->GetEntry(), 30.0f);

        for (std::list<Creature*>::iterator iter = lAssistList.begin(); iter != lAssistList.end(); ++iter)
        {
            if ((*iter)->isDead() || (*iter)->GetObjectGuid() == m_creature->GetObjectGuid() || std::abs((*iter)->GetPositionZ() - m_creature->GetPositionZ()) > 4.f)
                continue;

            lChargeList.push_back((*iter)->GetObjectGuid());

            if (!(*iter)->isInCombat())
                (*iter)->AI()->AttackStart(pWho);
        }
    }

    void JustDied(Unit* /*pKiller*/)
    {
        for (auto itr : lChargeList)
        {
            if (Creature* pBuddy = m_creature->GetMap()->GetCreature(itr))
                if (pBuddy->isAlive())
                    ((npc_vekniss_guardianAI*)pBuddy->AI())->Charge(m_creature);
        }
    }

    void Charge(Unit* target)
    {
        if (m_charge)
            return;
        m_charge = true;

        // Remove Slow/Stun-Effects
        m_creature->RemoveAurasAtMechanicImmunity(67110976, 0, true);

        float x, y, z;
        target->GetNearPoint(m_creature, x, y, z, target->GetObjectBoundingRadius(), target->GetCombatReach(m_creature) * 0.25f, target->GetAngle(m_creature));
        ((Creature*)m_creature)->StopMoving();
        m_creature->SetCanChase(false);
        
        m_fSpeedRate = m_creature->GetSpeedRate(MOVE_RUN);
        m_creature->SetSpeedRate(MOVE_RUN, m_creature->GetDistance(x, y, z) / 2.f, true);
        
        m_creature->GetMotionMaster()->MovePoint(0, x, y, z, true);
        m_creature->SetTargetGuid(ObjectGuid());
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData) 
    {
        m_charge = false;
        m_creature->SetSpeedRate(MOVE_RUN, m_fSpeedRate, true);
        DoCastSpellIfCan(m_creature, SPELL_IMPALE, CAST_TRIGGERED);
        m_creature->SetCanChase(true);

        if (m_creature->getVictim())
        {
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            m_creature->SetTargetGuid(m_creature->getVictim()->GetObjectGuid());
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_charge)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_enraged && m_creature->GetHealthPercent() < 20.f)
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
                m_enraged = true;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_vekniss_guardian(Creature* pCreature)
{
    return new npc_vekniss_guardianAI(pCreature);
}

void AddSC_mob_anubisath_sentinel()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_anubisath_sentinel";
    pNewScript->GetAI = &GetAI_npc_anubisath_sentinel;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_obsidian_eradicator";
    pNewScript->GetAI = &GetAI_npc_obsidian_eradicator;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_vekniss_guardian";
    pNewScript->GetAI = &GetAI_npc_vekniss_guardian;
    pNewScript->RegisterSelf();
}
