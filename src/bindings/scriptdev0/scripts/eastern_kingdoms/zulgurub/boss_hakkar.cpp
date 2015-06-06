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
SDName: Boss_Hakkar
SD%Complete: 95
SDComment:
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"
#include "../system/ProgressMonitor.h"

enum
{
    SAY_AGGRO                   = -1309020,
    SAY_FLEEING                 = -1309021,

    SPELL_BLOOD_SIPHON          = 24324,        // doesn't sort targets yet - figure out how to do it in core
    SPELL_BLOOD_SIPHON_DMG      = 24323,
    SPELL_BLOOD_SIPHON_HEAL     = 24322,
    SPELL_CORRUPTED_BLOOD       = 24328,
    SPELL_CAUSE_INSANITY        = 24327,//= 24327, //5727
    SPELL_WILL_OF_HAKKAR        = 24178,
	SPELL_ENRAGE                = 24318,
	SPELL_ROOT					= 31366,
    SPELL_POISONOUS_BLOOD       = 24321,

    // The Aspects of all High Priests
    SPELL_ASPECT_OF_JEKLIK      = 24687,
    SPELL_ASPECT_OF_VENOXIS     = 24688,
    SPELL_ASPECT_OF_MARLI       = 24686,
    SPELL_ASPECT_OF_THEKAL      = 24689,
    SPELL_ASPECT_OF_ARLOKK      = 24690,
};

struct MANGOS_DLL_DECL boss_hakkarAI : public ScriptedAI
{
    boss_hakkarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bJeklikAlive;
    bool m_bVenoxisAlive;
    bool m_bMarliAlive;
    bool m_bThekalAlive;
    bool m_bArlokkAlive;
	bool m_bHasTauntedEnter;
	bool m_bHasTauntedAltar;
	bool m_Aggro;

	float m_fTargetThreat;

    uint32 m_uiBloodSiphonTimer;
    uint32 m_uiCorruptedBloodTimer;
    uint32 m_uiCauseInsanityTimer;
    uint32 m_uiWillOfHakkarTimer;
    uint32 m_uiEnrageTimer;
	uint32 m_uiSummonSonsTimer;
	uint32 m_uiAggroTimer;

    uint32 m_uiAspectOfJeklikTimer;
    uint32 m_uiAspectOfVenoxisTimer;
    uint32 m_uiAspectOfMarliTimer;
    uint32 m_uiAspectOfThekalTimer;
    uint32 m_uiAspectOfArlokkTimer;

	Unit* m_MostHated;

    void Reset()
    {
		m_fTargetThreat = 0.0f;

        m_bJeklikAlive	= false;
        m_bVenoxisAlive = false;
        m_bMarliAlive	= false;
        m_bThekalAlive	= false;
        m_bArlokkAlive	= false;
		m_Aggro			= false;

        m_uiBloodSiphonTimer       = 90000;
        m_uiCorruptedBloodTimer    = 25000;
        m_uiCauseInsanityTimer     = 20000;
        m_uiWillOfHakkarTimer      = 17000;
        m_uiEnrageTimer            = 10*MINUTE*IN_MILLISECONDS;
		m_uiSummonSonsTimer		   = 30000;
		m_uiAggroTimer			   = 29500;

        m_uiAspectOfJeklikTimer    = 4000;
        m_uiAspectOfVenoxisTimer   = 7000;
        m_uiAspectOfMarliTimer     = 12000;
        m_uiAspectOfThekalTimer    = 8000;
        m_uiAspectOfArlokkTimer    = 18000;

		m_MostHated = nullptr;

		m_creature->addUnitState(UNIT_STAT_IGNORE_PATHFINDING);
    }

	void JustDied(Unit* killer) override
	{
		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::ZulGurub, Bosses::Hakkar);
	}
	/*
	void MoveInLineOfSight(Unit* pWho)
	{
		if (!m_bHasTauntedEnter && m_creature->IsWithinDistInMap(pWho, 100.0f))
		{
			DoScriptText(SAY_HAKKAR_MINION_DESTROY, m_creature);
			m_bHasTauntedEnter = true;
		}

		if (!m_bHasTauntedAltar && m_creature->IsWithinDistInMap(pWho, 30.0f))
		{
			DoScriptText(SAY_HAKKAR_PROTECT_ALTAR, m_creature);
			m_bHasTauntedAltar = true;
		}

		ScriptedAI::MoveInLineOfSight(pWho);
	}*/

    void Aggro(Unit*)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_JEKLIK) != DONE)
                m_bJeklikAlive = true;
            if (m_pInstance->GetData(TYPE_VENOXIS) != DONE)
                m_bVenoxisAlive = true;
            if (m_pInstance->GetData(TYPE_MARLI) != DONE)
                m_bMarliAlive = true;
            if (m_pInstance->GetData(TYPE_THEKAL) != DONE)
                m_bThekalAlive = true;
            if (m_pInstance->GetData(TYPE_ARLOKK) != DONE)
                m_bArlokkAlive = true;
        }
    }

    bool BloodSiphon()
    {
        Map* pMap = m_creature->GetMap();

        if (!pMap || !pMap->IsRaid())
            return false;

        Map::PlayerList const& PlayerList = pMap->GetPlayers();

        if (PlayerList.isEmpty())
            return false;

        std::list<Player*> lPlayers;

        for(Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();

            if (!pPlayer || !pPlayer->isAlive() || pPlayer->GetDistance(m_creature) > 100.0f)
                continue;

            if (!pPlayer->HasAura(SPELL_POISONOUS_BLOOD, EFFECT_INDEX_0))
                lPlayers.push_front(pPlayer);
            else
                lPlayers.push_back(pPlayer);
        }

        if (lPlayers.empty())
            return false;

        for(std::list<Player*>::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (!(*itr))
                continue;

            Player* pPlayer = (*itr);

            pPlayer->StopMoving();
            pPlayer->AttackStop();
            pPlayer->InterruptNonMeleeSpells(false);

            if (pPlayer->HasAura(SPELL_POISONOUS_BLOOD, EFFECT_INDEX_0))
            {
                pPlayer->RemoveAurasDueToSpell(SPELL_POISONOUS_BLOOD);
                pPlayer->CastSpell(m_creature, SPELL_BLOOD_SIPHON_DMG, true);
            }
            else
                pPlayer->CastSpell(m_creature, SPELL_BLOOD_SIPHON_HEAL, true);
        }

        return true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
			m_creature->CastSpell(m_creature, SPELL_ROOT, true);
		else
			m_creature->RemoveAura(31366, EFFECT_INDEX_0);
		
        // m_uiSummonSonsTimer
        if (m_uiSummonSonsTimer < uiDiff)
        {
			m_creature->SummonCreature(11357, -11839.157227f, -1699.534058f, 40.748497f, 0.052288f, TEMPSUMMON_TIMED_DESPAWN, 59000);
			m_creature->SummonCreature(11357, -11733.922852f, -1696.073486f, 40.750301f, 3.137941f, TEMPSUMMON_TIMED_DESPAWN, 59000);

			 m_uiSummonSonsTimer = 60000;
        }
        else
            m_uiSummonSonsTimer -= uiDiff;

        // Blood Siphon
        if (m_uiBloodSiphonTimer <= uiDiff)
        {
            // Free debuff slots
            for(int i = 0; i < TOTAL_AURAS; ++i)
            {
				m_creature->RemoveAurasOfTypeIf(static_cast<AuraType>(i), [](Aura* aura) -> bool {
					return aura->IsPositive();
				});
            }

            // Don't know how to sort players in core by presence of aura yet -> so we make it here
            // If something goes wrong, we cast core effect without sorting players
            if (!BloodSiphon())
                DoCastSpellIfCan(m_creature, SPELL_BLOOD_SIPHON, CAST_INTERRUPT_PREVIOUS | CAST_TRIGGERED);
            else
            {
                int32 spellId = SPELL_BLOOD_SIPHON;
                m_creature->CastCustomSpell(m_creature, SPELL_BLOOD_SIPHON, &spellId, NULL, NULL, true);
            }

            m_uiBloodSiphonTimer = 90000;
        }
        else
            m_uiBloodSiphonTimer -= uiDiff;

        // Corrupted Blood Timer
        if (m_uiCorruptedBloodTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CORRUPTED_BLOOD) == CAST_OK)
                    m_uiCorruptedBloodTimer = urand(30000, 45000);
            }
        }
        else
            m_uiCorruptedBloodTimer -= uiDiff;
		
        // Cause Insanity Timer
        if (m_uiCauseInsanityTimer < uiDiff)
		{
			m_MostHated = m_creature->getVictim();
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            {
				m_fTargetThreat = m_creature->getThreatManager().getThreat(m_MostHated);

				if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CAUSE_INSANITY) == CAST_OK)
				{
					m_creature->getThreatManager().modifyThreatPercent(m_MostHated, -100);
					m_uiAggroTimer = 9500;
					m_uiCauseInsanityTimer = 500000;
				}
			}
        }
        else
            m_uiCauseInsanityTimer -= uiDiff;

		if (m_uiAggroTimer < uiDiff)
		{
			m_creature->AddThreat(m_MostHated);
			m_creature->getThreatManager().addThreatDirectly(m_MostHated, m_fTargetThreat);
			m_uiCauseInsanityTimer = 15000;
			m_uiAggroTimer = 500000;
		}
		else
			m_uiAggroTimer -= uiDiff;
		
        // Will Of Hakkar Timer
        if (m_uiWillOfHakkarTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_WILL_OF_HAKKAR) == CAST_OK)
					DoScriptText(SAY_FLEEING, m_creature);

                    m_uiWillOfHakkarTimer = urand(25000, 35000);
            }
            else
                m_uiWillOfHakkarTimer = 25000;
        }
        else
            m_uiWillOfHakkarTimer -= uiDiff;

        if (m_uiEnrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
                m_uiEnrageTimer = 10*MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiEnrageTimer -= uiDiff;
			
        // Checking if Jeklik is dead. If not we cast her Aspect
        if (m_bJeklikAlive)
        {
            if (m_uiAspectOfJeklikTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ASPECT_OF_JEKLIK) == CAST_OK)
                    m_uiAspectOfJeklikTimer = urand(10000, 14000);
            }
            else
                m_uiAspectOfJeklikTimer -= uiDiff;
        }

        // Checking if Venoxis is dead. If not we cast his Aspect
        if (m_bVenoxisAlive)
        {
            if (m_uiAspectOfVenoxisTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ASPECT_OF_VENOXIS) == CAST_OK)
                    m_uiAspectOfVenoxisTimer = urand(6000, 12000);
            }
            else
                m_uiAspectOfVenoxisTimer -= uiDiff;
        }

        // Checking if Marli is dead. If not we cast her Aspect
        if (m_bMarliAlive)
        {
            if (m_uiAspectOfMarliTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ASPECT_OF_MARLI) == CAST_OK)
                    m_uiAspectOfMarliTimer = 8000;
            }
            else
                m_uiAspectOfMarliTimer -= uiDiff;
        }

        // Checking if Thekal is dead. If not we cast his Aspect
        if (m_bThekalAlive)
        {
            if (m_uiAspectOfThekalTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ASPECT_OF_THEKAL) == CAST_OK)
                    m_uiAspectOfThekalTimer = 15000;
            }
            else
                m_uiAspectOfThekalTimer -= uiDiff;
        }

        // Checking if Arlokk is dead. If yes we cast her Aspect
        if (m_bArlokkAlive)
        {
            if (m_uiAspectOfArlokkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ASPECT_OF_ARLOKK) == CAST_OK)
                {
                    DoResetThreat();
                    m_uiAspectOfArlokkTimer = urand(8000, 12000);
                }
            }
            else
                m_uiAspectOfArlokkTimer -= uiDiff;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hakkar(Creature* pCreature)
{
    return new boss_hakkarAI(pCreature);
}


/*######
## mob_son_of_hakkar
######*/

enum eSonOfHakkar
{
    SPELL_KNOCKDOWN                 = 16790,
    SPELL_THRASH                    = 3391,
    SPELL_SUMMON_POISONOUS_CLOUD    = 24319,
};

struct MANGOS_DLL_DECL mob_son_of_hakkarAI : public ScriptedAI
{
    mob_son_of_hakkarAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

    uint32 m_uiKnockdownTimer;
    uint32 m_uiThrashTimer;

    void Reset()
    {
        m_uiKnockdownTimer = urand(0,2000);
        m_uiThrashTimer = 3000;
    }

    void JustDied(Unit* /*pKiller*/)
    {
        // Summon Poisonous Cloud
        //DoCastSpellIfCan(m_creature, SPELL_SUMMON_POISONOUS_CLOUD, CAST_TRIGGERED);
        DoSpawnCreature(NPC_POISONOUS_CLOUD, NULL, NULL, NULL, NULL, TEMPSUMMON_TIMED_DESPAWN, 30000);
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
		if (m_pInstance)
		{
			if (m_pInstance->GetData(TYPE_HAKKAR) != IN_PROGRESS)
				return;
		
        // Forced respawn.
        if (pSummoned->GetEntry() == NPC_POISONOUS_CLOUD)
            if (m_creature->isDead())
                m_creature->Respawn();
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Knockdown
        if (m_uiKnockdownTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_KNOCKDOWN);
            m_uiKnockdownTimer = urand(7000, 11000);
        }
        else
            m_uiKnockdownTimer -= uiDiff;

        // Thrash
        if (m_uiThrashTimer <= uiDiff)
        {
			Unit* pTarget = m_creature->getVictim();
			if (pTarget && pTarget->isAlive())
				m_creature->AttackerStateUpdate(pTarget, BASE_ATTACK, true);

            m_uiThrashTimer = urand(2500, 7000);
        }
        else
            m_uiThrashTimer -= uiDiff;            

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_son_of_hakkar(Creature* pCreature)
{
    return new mob_son_of_hakkarAI(pCreature);
}

/*######
## mob_poisonous_cloud
######*/

enum ePoisonousCloud
{
	SPELL_POISON_CLOUD		= 26744,
    MODELID_INVISIBLE       = 11686,
};

struct MANGOS_DLL_DECL mob_poisonous_cloudAI : public ScriptedAI
{
    mob_poisonous_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        pCreature->SetDisplayId(MODELID_INVISIBLE);
        Reset();
    }

    uint32 m_uiPoisonCloudTimer;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_creature->HasAura(SPELL_POISON_CLOUD) == false)
			m_creature->CastSpell(m_creature, SPELL_POISON_CLOUD, true);

		static uint32 timer = 0;
		if (timer > 2 * MINUTE * IN_MILLISECONDS)
			m_creature->CastSpell(m_creature, 5, true);
		else
			timer += uiDiff;
    }
};

CreatureAI* GetAI_mob_poisonous_cloud(Creature* pCreature)
{
    return new mob_poisonous_cloudAI(pCreature);
}

void AddSC_boss_hakkar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_hakkar";
    pNewScript->GetAI = &GetAI_boss_hakkar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_son_of_hakkar";
    pNewScript->GetAI = &GetAI_mob_son_of_hakkar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_poisonous_cloud";
    pNewScript->GetAI = &GetAI_mob_poisonous_cloud;
    pNewScript->RegisterSelf();
}
