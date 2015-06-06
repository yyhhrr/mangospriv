/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Majordomo_Executus
SD%Complete: 30
SDComment: Correct spawning and Event NYI
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "temporarysummon.h"
#include "../system/ProgressMonitor.h"

#define SAY_AGGRO           -1409003
#define SAY_SPAWN           -1409004
#define SAY_SLAY            -1409005
#define SAY_SPECIAL         -1409006
#define SAY_DEFEAT          -1409007

#define SAY_SUMMON_MAJ      -1409008
#define SAY_ARRIVAL1_RAG    -1409009
#define SAY_ARRIVAL2_MAJ    -1409010
#define SAY_ARRIVAL3_RAG    -1409011
#define SAY_ARRIVAL4_MAJ    -1409022
#define SAY_ARRIVAL5_RAG    -1409012

#define SAY_LAST_ADD        -1409019

#define SPAWN_RAG_X         838.51f
#define SPAWN_RAG_Y         -829.84f
#define SPAWN_RAG_Z         -232.00f
#define SPAWN_RAG_O         1.70f

#define SPELL_MAGIC_REFLECTION      20619
#define SPELL_DAMAGE_REFLECTION     21075
#define SPELL_BLASTWAVE             20229
#define SPELL_AEGIS                 20620                   //This is self casted whenever we are below 50%
#define SPELL_TELEPORT              20618
#define SPELL_SUMMON_RAGNAROS       19774

//Adds
#define SPELL_FIRE_BLAST			20623
#define SPELL_SHADOW_BOLT			21077
#define	SPELL_GREATER_HEAL			24208

#define ENTRY_FLAMEWALKER_HEALER    11663
#define ENTRY_FLAMEWALKER_ELITE     11664

enum
{
    GOSSIP_ITEM_SUMMON_1    = -3409000,
    GOSSIP_ITEM_SUMMON_2    = -3409001,
    GOSSIP_ITEM_SUMMON_3    = -3409002,

    TEXT_ID_SUMMON_1        = 4995,
    TEXT_ID_SUMMON_2        = 5011,
    TEXT_ID_SUMMON_3        = 5012,

	NUM_ADDS				= 8,
};

#define SAFE_GUARD_ACTION(guard, action) \
	if(guard != nullptr) action

struct MANGOS_DLL_DECL boss_majordomoAI : public ScriptedAI
{
    boss_majordomoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		m_bHasEncounterFinished = false;
		m_AddsAnwesend = false;
		guard_one = guard_two = guard_three = guard_four = guard_five = guard_six = guard_seven = guard_eight = nullptr;

        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 MagicReflection_Timer;
    uint32 DamageReflection_Timer;
    uint32 Blastwave_Timer;

	uint32 m_uiTeleportTimer;
	uint32 m_uiAegisTimer;
    uint32 m_uiSpeechTimer;
	GUIDList m_luiMajordomoAddsGUIDs;

	uint8 m_uiAddsKilled;
	uint8 m_uiAddsKilledElite;
    uint8 m_uiSpeech;

    uint32 hittimer;

    Creature* guard_one;
    uint64 guard_oneGUID;
    Creature* guard_two;
    uint64 guard_twoGUID;
    Creature* guard_three;
    uint64 guard_threeGUID;
    Creature* guard_four;
    uint64 guard_fourGUID;
    Creature* guard_five;
    uint64 guard_fiveGUID;
    Creature* guard_six;
    uint64 guard_sixGUID;
    Creature* guard_seven;
    uint64 guard_sevenGUID;
    Creature* guard_eight;
    uint64 guard_eightGUID;

    uint32 end_event_timer, end_event_step;

	bool m_bHasEncounterFinished;
	bool m_AddsAnwesend;

    void Reset()
    {
        MagicReflection_Timer	= 30000;                     //Damage reflection first so we alternate
        DamageReflection_Timer	= 15000;
        Blastwave_Timer			= 10000;

		m_uiAegisTimer			= 8000;
        m_uiSpeechTimer			= 1000;
		m_uiTeleportTimer		= 20000;
		m_uiAddsKilled			= 0;
		m_uiAddsKilledElite		= 0;
        m_uiSpeech				= 0;

        end_event_timer			= 0;
        end_event_step			= 0;

        hittimer				= 1500;

		m_creature->setFaction(734);
		m_creature->DeleteThreatList();
		m_creature->RemoveAllAuras();
		m_creature->GetMotionMaster()->MoveTargetedHome();
	
		m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

		if (pInstance)
			pInstance->SetData(TYPE_MAJORDOMO, NOT_STARTED);
    }

    void KilledUnit(Unit* victim)
    {
        if (urand(0, 4))
            return;
		
		Map::PlayerList const& players = pInstance->instance->GetPlayers();
		for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
		{
			if (Player* pPlayer = itr->getSource())
			{
				if(!pPlayer->isAlive())
					pInstance->SetData(TYPE_MAJORDOMO, FAIL);
			}
		}

        DoScriptText(SAY_SLAY, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        SAFE_GUARD_ACTION(guard_one, guard_one->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_two, guard_two->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_three, guard_three->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_four, guard_four->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_five, guard_five->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_six, guard_six->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_seven, guard_seven->AI()->AttackStart(m_creature->getVictim()));
        SAFE_GUARD_ACTION(guard_eight, guard_eight->AI()->AttackStart(m_creature->getVictim()));

		if (pInstance)
			pInstance->SetData(TYPE_MAJORDOMO, IN_PROGRESS);
    }

	void EliteAddsDown(Creature* pSummoned)
	{
		if (pSummoned->GetEntry() == ENTRY_FLAMEWALKER_ELITE)
			m_uiAddsKilledElite += 1;
		
		if (m_uiAddsKilledElite == NUM_ADDS / 2)
		{
			if (pSummoned->GetEntry() == ENTRY_FLAMEWALKER_HEALER && pSummoned->isAlive())
			{
				pSummoned->RemoveAllAuras();
				for(int i = 0; i < MECHANIC_SAPPED + 1; ++i)
					pSummoned->ApplySpellImmune(0, IMMUNITY_MECHANIC, i, true); //gegen alle Arten von CC immun
				//pSummoned->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
			}
		}
	}

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
		EliteAddsDown(pSummoned);
        if (pSummoned->GetEntry() == ENTRY_FLAMEWALKER_HEALER || pSummoned->GetEntry() == ENTRY_FLAMEWALKER_ELITE)
        {
            m_uiAddsKilled += 1;

            // Yell if only one Add alive
            if (m_uiAddsKilled == NUM_ADDS - 1)
                DoScriptText(SAY_LAST_ADD, m_creature);

            // All adds are killed, retreat
            else if (m_uiAddsKilled == NUM_ADDS)
				StopAttacking();
        }
    }

	void StopAttacking()
	{
        m_bHasEncounterFinished = true;

		if (pInstance)
			pInstance->SetData(TYPE_MAJORDOMO, DONE);

		
		auto plr = m_creature->getVictim();
		using namespace ProgressMonitor;
        if (plr && plr->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(plr), Raids::MoltenCore, Bosses::Majordomus);

		end_event_timer	= 8000;
		end_event_step	= 0;

		m_creature->RemoveAllAuras();
		m_creature->CombatStop(true);
		m_creature->DeleteThreatList();
		m_creature->setFaction(35);
		m_creature->LoadCreatureAddon(false);
		m_creature->GetMotionMaster()->MovePoint(0, 765.24f, -1175.28f, -119.23f);

		end_event_step = 1;
	}
	
    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            DoCastSpellIfCan(m_creature, SPELL_AEGIS, CAST_TRIGGERED);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            uint32 uidata=pInstance->GetData(TYPE_MAJORDOMO);

			if (uidata == NOT_STARTED && m_AddsAnwesend && !m_bHasEncounterFinished)
			{
                Map* pMap;
                pMap = m_creature->GetMap();

				guard_one = pMap->GetCreature(guard_oneGUID);
				guard_two = pMap->GetCreature(guard_twoGUID);
				guard_three = pMap->GetCreature(guard_threeGUID);
				guard_four = pMap->GetCreature(guard_fourGUID);
				guard_five = pMap->GetCreature(guard_fiveGUID);
				guard_six = pMap->GetCreature(guard_sixGUID);
				guard_seven = pMap->GetCreature(guard_sevenGUID);
				guard_eight = pMap->GetCreature(guard_eightGUID);

				SAFE_GUARD_ACTION(guard_one, static_cast<TemporarySummon*>(guard_one)->UnSummon());
				SAFE_GUARD_ACTION(guard_two, static_cast<TemporarySummon*>(guard_two)->UnSummon());
				SAFE_GUARD_ACTION(guard_three, static_cast<TemporarySummon*>(guard_three)->UnSummon());
				SAFE_GUARD_ACTION(guard_four, static_cast<TemporarySummon*>(guard_four)->UnSummon());
				SAFE_GUARD_ACTION(guard_five, static_cast<TemporarySummon*>(guard_five)->UnSummon());
				SAFE_GUARD_ACTION(guard_six, static_cast<TemporarySummon*>(guard_six)->UnSummon());
				SAFE_GUARD_ACTION(guard_seven, static_cast<TemporarySummon*>(guard_seven)->UnSummon());
				SAFE_GUARD_ACTION(guard_eight, static_cast<TemporarySummon*>(guard_eight)->UnSummon());

				guard_one = guard_two = guard_three = guard_four = guard_five = guard_six = guard_seven = guard_eight = nullptr;

				m_AddsAnwesend = false;
			}

            if (uidata == DONE)
            {
                if (end_event_timer < diff)
                {
                    switch (end_event_step)
                    {
                    case 0:
                        m_creature->MonsterMoveWithSpeed(766.22f,-1176.0f,-119.0f, 5000);
                        end_event_step++;
                        end_event_timer = 5000;
                        break;
                    case 1:
                        m_creature->SetFacingTo(2.825f);
                        end_event_step++;
                        end_event_timer = 1000;
                        break;
                    case 2:
                        DoScriptText(SAY_ARRIVAL4_MAJ, m_creature);
						end_event_step++;
                        end_event_timer = 8000;
                        break;
                    case 3:
						end_event_step++;
                        end_event_timer = 4000;
                        break;
                    case 4:
                        m_creature->SetVisibility(VISIBILITY_OFF);
						m_creature->ForcedDespawn();
                        end_event_timer = 5000;
                        break;
                    }

                } else end_event_timer-=diff;

            }

            if (uidata == SPECIAL)
            {
                Map* pmap;
                pmap= m_creature->GetMap();

				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
				m_creature->NearTeleportTo(m_aMajordomoLocations[0].m_fX, m_aMajordomoLocations[0].m_fY, m_aMajordomoLocations[0].m_fZ, m_aMajordomoLocations[0].m_fO, true);

                m_creature->setFaction(54);

                if (guard_one = pmap->GetCreature(guard_oneGUID)) {}
                else
                {
                    guard_one=m_creature->SummonCreature(11663,754.913f,-1189.64f,-118.469f,2.54519f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_one->SetRespawnDelay(1800000);
                    guard_oneGUID = guard_one->GetGUID();
                }

                if (guard_two = pmap->GetCreature(guard_twoGUID)) {}
                else
                {
                    guard_two=m_creature->SummonCreature(11664,750.455f,-1193.86f,-118.16f,2.23496f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_two->SetRespawnDelay(1800000);
                    guard_twoGUID = guard_two->GetGUID();
                }

                if (guard_three = pmap->GetCreature(guard_threeGUID)) {}
                else
                {
                    guard_three=m_creature->SummonCreature(11663,743.943f,-1198,-118.037f,2.0818f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_three->SetRespawnDelay(1800000);
                    guard_threeGUID = guard_three->GetGUID();
                }

                if (guard_four = pmap->GetCreature(guard_fourGUID)) {}
                else
                {
                    guard_four=m_creature->SummonCreature(11664,738.084f,-1198.98f,-118.217f,1.68125f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_four->SetRespawnDelay(1800000);
                    guard_fourGUID = guard_four->GetGUID();
                }

                if (guard_five = pmap->GetCreature(guard_fiveGUID)) {}
                else
                {
                    guard_five=m_creature->SummonCreature(11663,757.528f,-1175.57f,-118.714f,3.10282f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_five->SetRespawnDelay(1800000);
                    guard_fiveGUID = guard_five->GetGUID();
                }

                if (guard_six = pmap->GetCreature(guard_sixGUID)) {}
                else
                {
                    guard_six=m_creature->SummonCreature(11664,757.065f,-1170.06f,-118.874f,3.52694f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_six->SetRespawnDelay(1800000);
                    guard_sixGUID = guard_six->GetGUID();
                }

                if (guard_seven = pmap->GetCreature(guard_sevenGUID)) {}
                else
                {
                    guard_seven=m_creature->SummonCreature(11663,754.254f,-1163.4f,-119.084f,3.74685f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_seven->SetRespawnDelay(1800000);
                    guard_sevenGUID = guard_seven->GetGUID();
                }

                if (guard_eight = pmap->GetCreature(guard_eightGUID)) {}
                else
                {
                    guard_eight=m_creature->SummonCreature(11664,749.444f,-1158.55f,-119.103f,3.97854f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1800000);
                    guard_eight->SetRespawnDelay(1800000);
                    guard_eightGUID = guard_eight->GetGUID();
                }
				
                if ( guard_one->isInCombat() ) m_creature->AI()->AttackStart(guard_one->getVictim());
                if ( guard_two->isInCombat() ) m_creature->AI()->AttackStart(guard_two->getVictim());
                if ( guard_three->isInCombat() ) m_creature->AI()->AttackStart(guard_three->getVictim());
                if ( guard_four->isInCombat() ) m_creature->AI()->AttackStart(guard_four->getVictim());
                if ( guard_five->isInCombat() ) m_creature->AI()->AttackStart(guard_five->getVictim());
                if ( guard_six->isInCombat() ) m_creature->AI()->AttackStart(guard_six->getVictim());
                if ( guard_seven->isInCombat() ) m_creature->AI()->AttackStart(guard_seven->getVictim());
                if ( guard_eight->isInCombat() ) m_creature->AI()->AttackStart(guard_eight->getVictim());

				m_AddsAnwesend = true;
            }
            return;
		}

		if (m_uiAddsKilled == NUM_ADDS)
			StopAttacking();

		if(m_creature->HasAura(20620) == false)
			m_creature->CastSpell(m_creature, 20620, true);

		EliteAddsDown(guard_one);
		EliteAddsDown(guard_three);
		EliteAddsDown(guard_five);
		EliteAddsDown(guard_seven);

        if (m_uiAegisTimer <= diff)
            m_uiAegisTimer = 0;
        else
            m_uiAegisTimer -= diff;

        if (m_creature->GetHealthPercent() < 90.0f && !m_uiAegisTimer)
        {
            DoCastSpellIfCan(m_creature, SPELL_AEGIS);
            m_uiAegisTimer = 9000;
        }

        if (MagicReflection_Timer < diff)
		{
			DoCastSpellIfCan(m_creature, SPELL_MAGIC_REFLECTION);
            MagicReflection_Timer = 30000;
        }
		else
			MagicReflection_Timer -= diff;

        if (DamageReflection_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DAMAGE_REFLECTION);
            DamageReflection_Timer = 30000;
        }
		else
			DamageReflection_Timer -= diff;

        if (Blastwave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLASTWAVE);
            Blastwave_Timer = 12000;
        }
		else
			Blastwave_Timer -= diff;

        if (m_uiTeleportTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
				m_creature->getThreatManager().modifyThreatPercent(pTarget, -100);

                if (DoCastSpellIfCan(pTarget, SPELL_TELEPORT) == CAST_OK)
                    m_uiTeleportTimer = 20000;
            }
        }
        else
            m_uiTeleportTimer -= diff;

        if (hittimer < diff)
        {
            DoMeleeAttackIfReady();
            hittimer = 1500;
        }
		else
			hittimer -= diff;
    }
};

bool GossipHello_boss_majordomo(Player* pPlayer, Creature* pCreature)
{
   ScriptedInstance* pInstance;
   pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
   
    if (pInstance->GetData(TYPE_SULFURON) == SPECIAL && pInstance->GetData(TYPE_GEDDON) == SPECIAL && pInstance->GetData(TYPE_SHAZZRAH) == SPECIAL && pInstance->GetData(TYPE_GOLEMAGG) == SPECIAL && pInstance->GetData(TYPE_GARR) == SPECIAL && pInstance->GetData(TYPE_MAGMADAR) == SPECIAL && pInstance->GetData(TYPE_GEHENNAS) == SPECIAL && pInstance->GetData(TYPE_MAJORDOMO) != DONE || pPlayer->isGameMaster())
	{
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SUMMON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(TEXT_ID_SUMMON_1, pCreature->GetObjectGuid());
			return true;
	}

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_boss_majordomo(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
   ScriptedInstance* pInstance;
   pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
		pInstance->SetData(TYPE_MAJORDOMO, SPECIAL);
    }

    return true;
}

CreatureAI* GetAI_boss_majordomo(Creature* pCreature)
{
    return new boss_majordomoAI(pCreature);
}

struct MANGOS_DLL_DECL npc_domus_heilerAI : public ScriptedAI
{
    npc_domus_heilerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
    }

    uint32 m_uiShadowBoltTimer;
    uint32 m_uiGreaterHealTimer;

    void Reset()
    {
        m_uiShadowBoltTimer		= urand(8000, 12000);
        m_uiGreaterHealTimer	= 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShadowBoltTimer <= diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_BOLT);
            m_uiShadowBoltTimer = urand(6500, 8000);
        }
        else
            m_uiShadowBoltTimer -= diff;

        if (m_uiGreaterHealTimer <= diff)
        {
            if (Unit* pTarget = DoSelectLowestHpFriendly(40.0f, 1000))
                DoCastSpellIfCan(pTarget, SPELL_GREATER_HEAL);

            m_uiGreaterHealTimer = urand(10000, 15000);
        }
        else
            m_uiGreaterHealTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_domus_heiler(Creature *_Creature)
{
    return new npc_domus_heilerAI (_Creature);
}

struct MANGOS_DLL_DECL npc_domus_eliteAI : public ScriptedAI
{
    npc_domus_eliteAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
    }

    uint32 m_uiBlastWaveTimer;
    uint32 m_uiFireBlastTimer;

    void Reset()
    {
        m_uiBlastWaveTimer = 10000;
        m_uiFireBlastTimer = 20000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBlastWaveTimer <= diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIRE_BLAST);
            m_uiBlastWaveTimer = urand(10000, 18000);
        }
        else
            m_uiBlastWaveTimer -= diff;

        if (m_uiFireBlastTimer <= diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLASTWAVE);
            m_uiFireBlastTimer = urand(12000, 15000);
        }
        else
            m_uiFireBlastTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_domus_elite(Creature *_Creature)
{
    return new npc_domus_eliteAI (_Creature);
}

void AddSC_boss_majordomo()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_majordomo";
    newscript->GetAI = &GetAI_boss_majordomo;
    newscript->pGossipHello = &GossipHello_boss_majordomo;
    newscript->pGossipSelect = &GossipSelect_boss_majordomo;
    newscript->GetAI = &GetAI_boss_majordomo;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_domus_heiler";
    newscript->GetAI = &GetAI_npc_domus_heiler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_domus_elite";
    newscript->GetAI = &GetAI_npc_domus_elite;
    newscript->RegisterSelf();
}

