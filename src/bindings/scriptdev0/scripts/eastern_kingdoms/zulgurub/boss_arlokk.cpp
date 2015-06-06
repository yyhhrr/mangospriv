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
SDName: Boss_Arlokk
SD%Complete: 95
SDComment: Wrong cleave and red aura is missing.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"
#include "../system/ProgressMonitor.h"
#include "TemporarySummon.h"

bool GOUse_go_gong_of_bethekk(Player* plr, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
		if (pInstance->GetData(TYPE_ARLOKK) != NOT_STARTED)
            return true;

        pInstance->SetData(TYPE_ARLOKK, IN_PROGRESS);
		plr->SummonCreature(14515, -11540.7f, -1627.71f, 41.27f, 0.1f, TEMPSUMMON_MANUAL_DESPAWN, 0);
    }

    return false;
}

enum
{
    SAY_AGGRO                   = -1309011,
    SAY_FEAST_PANTHER           = -1309012,
    SAY_DEATH                   = -1309013,

    SPELL_SHADOWWORDPAIN        = 23952,
    SPELL_GOUGE                 = 24698,
    SPELL_MARK                  = 24210,
    SPELL_CLEAVE                = 26350,                    //Perhaps not right. Not a red aura...
    SPELL_PANTHER_TRANSFORM     = 24190,
	SPELL_BACKSTAB				= 15582,

	SPELL_THRASH				= 3391,

    MODEL_ID_NORMAL             = 15218,
    MODEL_ID_PANTHER            = 15215,
    MODEL_ID_BLANK              = 11686,

    NPC_ZULIAN_PROWLER          = 15101
};

struct MANGOS_DLL_DECL boss_arlokkAI : public ScriptedAI
{
    boss_arlokkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowWordPain_Timer;
    uint32 m_uiGouge_Timer;
    uint32 m_uiMark_Timer;
    uint32 m_uiCleave_Timer;
    uint32 m_uiVanish_Timer;
    uint32 m_uiVisible_Timer;

    uint32 m_uiSummon_Timer;
    uint32 m_uiSummonCount;

    ObjectGuid m_markedGuid;
	ObjectGuid m_uiBackstabGUID;

    bool m_bIsPhaseTwo;
    bool m_bIsVanished;

    void Reset()
    {
        m_bIsPhaseTwo = false;
        m_bIsVanished = false;

        m_uiShadowWordPain_Timer = 8000;
        m_uiGouge_Timer = 14000;
        m_uiMark_Timer = 35000;
        m_uiCleave_Timer = 4000;
        m_uiVanish_Timer = 30000;
        m_uiVisible_Timer = 6000;

        m_uiSummon_Timer = 5000;
        m_uiSummonCount = 0;

        m_markedGuid.Clear();
		m_uiBackstabGUID.Clear();

        m_creature->SetDisplayId(MODEL_ID_NORMAL);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit*)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
			m_pInstance->SetData(TYPE_ARLOKK, NOT_STARTED);

        //we should be summoned, so despawn
		static_cast<TemporarySummon*>(m_creature)->UnSummon();
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        m_creature->SetDisplayId(MODEL_ID_NORMAL);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (m_creature->GetVisibility() != VISIBILITY_ON)
        {
            m_creature->SetVisibility(VISIBILITY_ON);
        }

        if (m_pInstance)
			m_pInstance->SetData(TYPE_ARLOKK, DONE);

		m_creature->SetRespawnDelay(3 * DAY);
		m_creature->SetRespawnTime(MONTH);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::ZulGurub, Bosses::Arlokk);
    }

    void DoSummonPhanters()
    {
        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_markedGuid))
        {
            if (pPlayer->isAlive())
                DoScriptText(SAY_FEAST_PANTHER, m_creature, pPlayer);
        }

        m_creature->SummonCreature(NPC_ZULIAN_PROWLER, -11532.7998f, -1649.6734f, 41.4800f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
        m_creature->SummonCreature(NPC_ZULIAN_PROWLER, -11532.9970f, -1606.4840f, 41.2979f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
    }

    void JustSummoned(Creature* pSummoned)
    {
		if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_markedGuid))
		{
			if (pPlayer->isAlive())
				pSummoned->AI()->AttackStart(pPlayer);
		} else
			pSummoned->SetInCombatWithZone();

        ++m_uiSummonCount;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsPhaseTwo)
        {
            if (m_uiShadowWordPain_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SHADOWWORDPAIN);
                m_uiShadowWordPain_Timer = 15000;
            }
            else
                m_uiShadowWordPain_Timer -= uiDiff;

            if (m_uiMark_Timer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (Player* pMark = pTarget->GetCharmerOrOwnerPlayerOrPlayerItself())
                    {
                        DoCastSpellIfCan(pMark, SPELL_MARK);
                        m_markedGuid = pMark->GetObjectGuid();
                    }
                    else
                    {
                        m_markedGuid.Clear();
                        error_log("SD0: boss_arlokk could not accuire a new target to mark.");
                    }
                }

                m_uiMark_Timer = 15000;
            }
            else
                m_uiMark_Timer -= uiDiff;
        }
        else
        {
            //Cleave_Timer
            if (m_uiCleave_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleave_Timer = 16000;
            }
            else
                m_uiCleave_Timer -= uiDiff;

            //Gouge_Timer
            if (m_uiGouge_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_GOUGE);

                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);

                m_uiGouge_Timer = urand(17000, 27000);
            }
            else
                m_uiGouge_Timer -= uiDiff;
        }

        if (m_uiSummonCount <= 60)
        {
            if (m_uiSummon_Timer < uiDiff)
            {
                DoSummonPhanters();
                m_uiSummon_Timer = 5000;
            }
            else
                m_uiSummon_Timer -= uiDiff;
        }

        if (m_uiVanish_Timer < uiDiff)
        {
            //Invisble Model
			m_creature->SetVisibility(VISIBILITY_OFF);

			m_creature->SetDisplayId(MODEL_ID_BLANK);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            m_creature->AttackStop();
            DoResetThreat();

            m_bIsVanished = true;
			
            m_uiVanish_Timer = 20000;
            m_uiVisible_Timer = 8000;
        }
        else
            m_uiVanish_Timer -= uiDiff;

        if (m_bIsVanished)
        {
            if (m_uiVisible_Timer < uiDiff)
            {
                //The Panther Model
                if (m_creature->GetDisplayId() != MODEL_ID_PANTHER)
                    m_creature->SetDisplayId(MODEL_ID_PANTHER);

                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);
                m_creature->SetVisibility(VISIBILITY_ON);

                const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 35)));
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 35)));
                m_creature->UpdateDamagePhysical(BASE_ATTACK);

                // Backstab
                if (m_uiBackstabGUID)
                {
                    Unit* pBackstabUnit = m_creature->GetMap()->GetUnit(m_uiBackstabGUID);
                    
                    if (pBackstabUnit && m_creature->IsWithinDist(pBackstabUnit, ATTACK_DISTANCE) && m_creature->HasInArc((const float)M_PI, pBackstabUnit))
                    {
                        m_uiBackstabGUID.Clear();
                        DoCastSpellIfCan(pBackstabUnit, SPELL_BACKSTAB, CAST_INTERRUPT_PREVIOUS);
                        AttackStart(pBackstabUnit);
                    }
                }

                m_bIsPhaseTwo = true;
                m_bIsVanished = false;
            }
            else
                m_uiVisible_Timer -= uiDiff;
        }
        else
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_arlokk(Creature* pCreature)
{
    return new boss_arlokkAI(pCreature);
}

/*######
## mob_zulian_prowler
######*/

struct MANGOS_DLL_DECL mob_zulian_prowlerAI : public ScriptedAI
{
    mob_zulian_prowlerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_zulgurub*)pCreature->GetInstanceData();
        Reset();
    }

    instance_zulgurub* m_pInstance;

    uint32 m_uiCheckTimer;
    uint32 m_uiThrashTimer;

    void Reset()
    {
        m_uiCheckTimer = 1000;
        m_uiThrashTimer = urand(6000, 8000);

		m_creature->addUnitState(UNIT_STAT_IGNORE_PATHFINDING);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Attack marked target
        if (m_uiCheckTimer <= uiDiff)
        {
            if (m_pInstance && m_pInstance->GetData(TYPE_ARLOKK) == IN_PROGRESS)
            {
                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const& lPlayers = m_pInstance->instance->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                        if (Player* pPlayer = itr->getSource())
                            if (pPlayer->HasAura(SPELL_MARK) && m_creature->IsWithinDistInMap(pPlayer, DEFAULT_VISIBILITY_INSTANCE) && pPlayer->IsWithinLOSInMap(m_creature))
                                if (!m_creature->getVictim() || (m_creature->getVictim() && !m_creature->getVictim()->HasAura(SPELL_MARK)))
                                {
                                    DoResetThreat();
                                    m_creature->AddThreat(pPlayer, 10000.0f);
                                }
                }
            }

            m_uiCheckTimer = 1000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Thrash
        if (m_uiThrashTimer <= uiDiff)
        {
			Unit* pTarget = m_creature->getVictim();
			if (pTarget && pTarget->isAlive())
				m_creature->AttackerStateUpdate(pTarget, BASE_ATTACK, true);

            m_uiThrashTimer = urand(8000, 10000);
        }
        else
            m_uiThrashTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_zulian_prowler(Creature* pCreature)
{
    return new mob_zulian_prowlerAI(pCreature);
}

void AddSC_boss_arlokk()
{
    Script *newscript;
	newscript = new Script;
    newscript->Name = "boss_arlokk";
    newscript->GetAI = &GetAI_boss_arlokk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zulian_prowler";
    newscript->GetAI = &GetAI_mob_zulian_prowler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_gong_of_bethekk";
    newscript->pGOUse = &GOUse_go_gong_of_bethekk;
    newscript->RegisterSelf();
}
