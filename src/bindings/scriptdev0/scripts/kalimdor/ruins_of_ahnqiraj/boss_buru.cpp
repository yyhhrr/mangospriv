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
SDName: Boss_Buru
SD%Complete: 80
SDComment: 
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

enum Spells
{
    EMOTE_TARGET            = -1509002,

    // Buru
    SPELL_CREEPING_PLAGUE   = 20512,
    SPELL_DISMEMBER         = 96,
    SPELL_FULL_SPEED        = 1557,
    SPELL_THORNS            = 25640,
    SPELL_GATHERING_SPEED   = 1834,
    SPELL_BURU_TRANSFORM    = 24721,
    SPELL_BURU_EGG_TRIGGER  = 26646,

    // Buru Egg
    SPELL_SUMMON_HATCHLING  = 1881,
    SPELL_EXPLODE           = 19593,
};

struct MANGOS_DLL_DECL boss_buruAI : public ScriptedAI
{
    boss_buruAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ruins_of_ahnqiraj*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ruins_of_ahnqiraj* m_pInstance;

    bool m_bIsEnraged;

    uint32 m_uiDismemberTimer;
    uint32 m_uiCreepingPlagueTimer;
    uint32 m_uiGatheringSpeedTimer;
    uint32 m_uiFullSpeedTimer;

    void Reset() 
    {
        m_bIsEnraged = false;
        m_creature->RemoveAllAuras();
        m_uiDismemberTimer = 5000;
        m_uiCreepingPlagueTimer = 6000;
        m_uiGatheringSpeedTimer = 9000;
        m_uiFullSpeedTimer = 60000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_BURU, FAIL);

            std::list<Creature*> EggList;
            GetCreatureListWithEntryInGrid(EggList, m_creature, NPC_BURU_EGG, 500.f);
            for (auto egg : EggList)
            {
                if (!egg->isAlive())
                    continue;
                egg->Respawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BURU, IN_PROGRESS);

        DoCastSpellIfCan(m_creature, SPELL_THORNS);
        m_creature->SetArmor(20000);
        DoAttackNewTarget();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BURU, DONE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_bIsEnraged && ((m_creature->GetHealth() - ((uiDamage * m_creature->GetResilience()) / 100)) / (float)m_creature->GetMaxHealth()) <= .22f)
        {
            DoCastSpellIfCan(m_creature, SPELL_BURU_TRANSFORM);
            m_creature->SetArmor(0);
            m_bIsEnraged = true;

            if (m_pInstance)
            {
                std::list<Creature*> EggList;
                GetCreatureListWithEntryInGrid(EggList, m_creature, NPC_BURU_EGG, 500.f);
                for (auto egg : EggList)
                {
                    if (!egg->isAlive())
                        continue;
                    egg->ForcedDespawn();
                    egg->CastSpell(egg, SPELL_SUMMON_HATCHLING, true);
                }
            }
        }
    }

    void DoAttackNewTarget()
    {
        if (m_bIsEnraged)
            return;

        m_creature->RemoveAurasDueToSpell(SPELL_FULL_SPEED);
        m_creature->RemoveAurasDueToSpell(SPELL_GATHERING_SPEED);
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER))
        {
            m_creature->FixateTarget(pTarget);
            DoScriptText(EMOTE_TARGET, m_creature, pTarget);
        }
        m_uiFullSpeedTimer = 60000;
        m_uiGatheringSpeedTimer = 9000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDismemberTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DISMEMBER);
            m_uiDismemberTimer = 5000;
        }
        else
            m_uiDismemberTimer -= uiDiff;

        if (m_bIsEnraged)
            if (m_uiCreepingPlagueTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_CREEPING_PLAGUE);
                m_uiCreepingPlagueTimer = 6000;
            }
            else
                m_uiCreepingPlagueTimer -= uiDiff;
        else
        {
            if (m_uiFullSpeedTimer)
            {
                if (m_uiGatheringSpeedTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_GATHERING_SPEED) == CAST_OK)
                        m_uiGatheringSpeedTimer = 7000;
                }
                else
                    m_uiGatheringSpeedTimer -= uiDiff;

                if (m_uiFullSpeedTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FULL_SPEED) == CAST_OK)
                        m_uiFullSpeedTimer = 0;
                }
                else
                    m_uiFullSpeedTimer -= uiDiff;
            }
        }

        if (m_bIsEnraged || m_creature->getVictim()->GetDistance2d(m_creature) == 0.f)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_buru(Creature* pCreature)
{
    return new boss_buruAI(pCreature);
}

struct MANGOS_DLL_DECL mob_buru_eggAI : public ScriptedAI
{
    mob_buru_eggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ruins_of_ahnqiraj*)pCreature->GetInstanceData();
        Reset();
    }
    
    uint32 m_respawn;
    instance_ruins_of_ahnqiraj* m_pInstance;

    void Reset()
    {
        m_respawn = urand(55 * IN_MILLISECONDS, 65 * IN_MILLISECONDS);
        SetCombatMovement(false);
        m_creature->addUnitState(UNIT_STAT_CAN_NOT_TARGET);
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance)
            return;

        m_creature->CastSpell(m_creature, 23973, true);

		Creature* pBuru = m_pInstance->GetSingleCreatureFromStorage(NPC_BURU);
        if (pBuru && pBuru->isAlive())
        {
            pBuru->GetMotionMaster()->MoveChase(pWho);
            pBuru->AI()->AttackStart(pWho);
            pBuru->SetInCombatWithZone();
        }
    }

    void JustSummoned(Creature* pSummoned) override
    {
        // The purpose of this is unk for the moment
        if (pSummoned->GetEntry() == NPC_BURU_EGG_TRIGGER)
        {
            pSummoned->CastSpell(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), SPELL_BURU_EGG_TRIGGER, true);
            pSummoned->addUnitState(UNIT_STAT_CAN_NOT_TARGET);
            pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            pSummoned->SetObjectScale(0.f);
            pSummoned->ForcedDespawn(30 * IN_MILLISECONDS);
            pSummoned->HandleEmoteState(EMOTE_STATE_DEAD);
        }
        // The Hatchling should attack a random target
        else if (pSummoned->GetEntry() == NPC_HATCHLING)
        {
            if (m_pInstance)
            {
                if (Creature* pBuru = m_pInstance->GetSingleCreatureFromStorage(NPC_BURU))
                {
                    if (Unit* pTarget = pBuru->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        pSummoned->AI()->AttackStart(pTarget);
                }
            }
        }
    }

    void MoveInLineOfSight(Unit* /*pWho*/) override {}

    void UpdateAI(const uint32 uiDiff) 
    {
        if (m_creature->IsInEvadeMode() || !m_creature->isInCombat())
            return;

        if (!m_creature->SelectRandomUnfriendlyTarget(0, 30.f))
            EnterEvadeMode();
    }

    void UpdateAIDead(const uint32 uiDiff) 
    {
        if (!m_pInstance)
            return;

        Creature* pBuru = m_pInstance->GetSingleCreatureFromStorage(NPC_BURU);
        if (pBuru && pBuru->isAlive() && pBuru->GetArmor())
        {
            if (!pBuru->isInCombat() || m_respawn <= uiDiff)
                m_creature->Respawn();
            else
                m_respawn -= uiDiff;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        m_creature->CastSpell(m_creature, SPELL_EXPLODE, true);
        m_creature->CastSpell(m_creature, SPELL_SUMMON_HATCHLING, true);

        Map::PlayerList const& players = m_pInstance->instance->GetPlayers();
        for (auto& player : players)
        {
            Player* pPlayer = player.getSource();
            if (pPlayer && pPlayer->isAlive())
            {
                float dist = sqrt(m_creature->GetDistanceSqr(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ()));
                if (dist >= 20.f)
                    continue;

                uint32 dam = 3000 * ((20 - dist) / 20.f);
                pPlayer->EnvironmentalDamage(DAMAGE_SLIME, dam);
            }
        }

        Creature* pBuru = m_pInstance->GetSingleCreatureFromStorage(NPC_BURU);
        if (pBuru && pBuru->isAlive())
        {
            // get exact distance
            float dist = sqrt(m_creature->GetDistanceSqr(pBuru->GetPositionX(), pBuru->GetPositionY(), pBuru->GetPositionZ()));
            if (dist >= 20.f)
                return;

            uint32 dam = 105000 * ((20 - dist) / 20.f);
            pBuru->AI()->DamageTaken(m_creature, dam);
            m_creature->DealDamage(pBuru, dam, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
            
            if (boss_buruAI* pBuruAI = dynamic_cast<boss_buruAI*>(pBuru->AI()))
                pBuruAI->DoAttackNewTarget();
        }
    }
};

CreatureAI* GetAI_mob_buru_egg(Creature* pCreature)
{
    return new mob_buru_eggAI(pCreature);
}

void AddSC_boss_buru()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_buru";
    pNewscript->GetAI = &GetAI_boss_buru;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_buru_egg";
    pNewscript->GetAI = &GetAI_mob_buru_egg;
    pNewscript->RegisterSelf();
}
