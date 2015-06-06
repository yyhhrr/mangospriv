/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Zulfarrak
SD%Complete: 50
SDComment: Consider it temporary, no instance script made for this instance yet.
SDCategory: Zul'Farrak
EndScriptData */

/* ContentData
npc_sergeant_bly
npc_weegli_blastfuse
EndContentData */

#include "precompiled.h"
#include "zulfarrak.h"

/*######
## npc_sergeant_bly
######*/

enum blyAndCrewFactions
{
    FACTION_HOSTILE           = 14,
    FACTION_FRIENDLY          = 35,  //while in cages (so the trolls won't attack them while they're caged)
    FACTION_FREED             = 250  //after release (so they'll be hostile towards trolls)
};

enum blySays
{
    SAY_1 = -1209002,
    SAY_2 = -1209003
};

enum blySpells
{
    SPELL_SHIELD_BASH          = 11972,
    SPELL_REVENGE              = 12170
};

#define GOSSIP_BLY -3209001

struct MANGOS_DLL_DECL npc_sergeant_blyAI : public ScriptedAI
{
    npc_sergeant_blyAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();	
	}

	ScriptedInstance* pInstance;

       uint32 postGossipStep;
       uint32 Text_Timer;
       uint32 ShieldBash_Timer;

       uint32 Revenge_Timer;                                   //this is wrong, spell should never be used unless me->getVictim() dodge, parry or block attack. Trinity support required.
       uint64 PlayerGUID;

        void Reset()
        {
            ShieldBash_Timer	= 5000;
            Revenge_Timer		= 8000;

			m_creature->GetMaxHealth();
        }

        void UpdateAI(const uint32 diff)
        {
            if (postGossipStep>0 && postGossipStep<4)
            {
                if (Text_Timer<diff)
				{
                    switch (postGossipStep)
                    {
                        case 1:
                            //weegli doesn't fight - he goes & blows up the door
                            if (Creature* pWeegli = m_creature->GetMap()->GetCreature(pInstance->GetData64(ENTRY_WEEGLI)))
                                m_creature->AI()->SendAIEvent(AI_EVENT_CUSTOM_EVENTAI_A, m_creature, pWeegli);
                            DoScriptText(SAY_1, m_creature);
                            Text_Timer = 5000;
                            break;
                        case 2:
                            DoScriptText(SAY_2, m_creature);
                            Text_Timer = 5000;
                            break;
                        case 3:
                            m_creature->setFaction(FACTION_HOSTILE);
							Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
								m_creature->AI()->AttackStart(target);

                            if (pInstance)
                            {
                                switchFactionIfAlive(pInstance, ENTRY_RAVEN);
                                switchFactionIfAlive(pInstance, ENTRY_ORO);
                                switchFactionIfAlive(pInstance, ENTRY_MURTA);
                            }
                    }
                    postGossipStep++;
                }
                else Text_Timer -= diff;
            }

			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

            if (ShieldBash_Timer <= diff)
            {
                DoCast(m_creature->getVictim(), SPELL_SHIELD_BASH);
                ShieldBash_Timer = 15000;
            }
            else
                ShieldBash_Timer -= diff;

            if (Revenge_Timer <= diff)
            {
                DoCast(m_creature->getVictim(), SPELL_REVENGE);
                Revenge_Timer = 10000;
            }
            else
                Revenge_Timer -= diff;

            DoMeleeAttackIfReady();
        }

        void DoAction(const int32 /*param*/)
        {
            postGossipStep = 1;
            Text_Timer = 0;
        }

        void switchFactionIfAlive(ScriptedInstance* pInstance, uint32 entry)
        {
           if (Creature* crew = pInstance->instance->GetCreature(pInstance->GetData64(entry)))
               if (crew->isAlive())
                   crew->setFaction(FACTION_HOSTILE);
        }
};

CreatureAI* GetAI_npc_sergeant_bly(Creature* pCreature)
{
    return new npc_sergeant_blyAI(pCreature);
}

bool GossipSelect_npc_sergeant_bly(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();

            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			npc_sergeant_blyAI* pEscortAI = dynamic_cast<npc_sergeant_blyAI*>(pCreature->AI());
			pEscortAI->DoAction(0);
		}
        return true;
}

bool GossipHello_npc_sergeant_bly(Player* pPlayer, Creature* pCreature)
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
        {
            if (pInstance->GetData(EVENT_PYRAMID) == PYRAMID_KILLED_ALL_TROLLS)
            {
                pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_BLY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->SEND_GOSSIP_MENU(1517, pCreature->GetGUID());
            }
            else
                if (pInstance->GetData(EVENT_PYRAMID) == PYRAMID_NOT_STARTED)
                    pPlayer->SEND_GOSSIP_MENU(1515, pCreature->GetGUID());
				else if (pInstance->GetData(EVENT_PYRAMID) == PYRAMID_CAGES_OPEN)
                    pPlayer->SEND_GOSSIP_MENU(1516, pCreature->GetGUID());
            return true;
        }
        return false;
    }

/*######
## npc_weegli_blastfuse
######*/

enum weegliSpells
{
    SPELL_BOMB                 = 8858,
    SPELL_GOBLIN_LAND_MINE     = 21688,
    SPELL_SHOOT                = 6660,
    SPELL_WEEGLIS_BARREL       = 10772
};

enum weegliSays
{
    SAY_WEEGLI_OHNO		= -1209000,
    SAY_WEEGLI_OK_I_GO	= -1209001
};

#define GOSSIP_WEEGLI -3209000

struct MANGOS_DLL_DECL npc_weegli_blastfuseAI : public ScriptedAI
{
    npc_weegli_blastfuseAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;

        uint32 Bomb_Timer;
        uint32 LandMine_Timer;
        bool destroyingDoor;

        void Reset()
        {   
			destroyingDoor = false;
            Bomb_Timer = 10000;
            LandMine_Timer = 30000;
        }

        void UpdateAI(const uint32 diff)
        {
			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

            if (Bomb_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_BOMB);
                Bomb_Timer = 10000;
            }
            else
                Bomb_Timer -= diff;

            if (m_creature->isAttackReady() && !m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
            {
                DoCast(m_creature->getVictim(), SPELL_SHOOT);
                m_creature->SetSheath(SHEATH_STATE_RANGED);
            }
            else
            {
                m_creature->SetSheath(SHEATH_STATE_MELEE);
                DoMeleeAttackIfReady();
            }
        }

        void MovementInform(uint32 /*type*/, uint32 /*id*/)
        {
            if (pInstance)
            {
                if (pInstance->GetData(EVENT_PYRAMID) == PYRAMID_CAGES_OPEN)
                {
                    pInstance->SetData(EVENT_PYRAMID, PYRAMID_ARRIVED_AT_STAIR);
                    DoScriptText(SAY_WEEGLI_OHNO, m_creature);
					m_creature->GetMotionMaster()->MoveTargetedHome();
                }
                else if (destroyingDoor)
                {
                    DoScriptText(SAY_WEEGLI_OK_I_GO, m_creature);
                    pInstance->DoUseDoorOrButton(pInstance->GetData64(GO_END_DOOR));
                    m_creature->ForcedDespawn();
                }
            }
        }

        void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* /*pInvoker*/, uint32 /*miscValue*/) 
        {
            if (eventType == AI_EVENT_CUSTOM_EVENTAI_A)
                DestroyDoor();
        }

        void DestroyDoor()
        {
            if (m_creature->isAlive() && !destroyingDoor)
            {
                m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                m_creature->setFaction(FACTION_FRIENDLY);
                m_creature->GetMotionMaster()->MovePoint(0, 1858.57f, 1146.35f, 14.745f);
                destroyingDoor = true;
            }
        }
};

bool GossipSelect_npc_weegli_blastfuse(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            //here we make him run to door, set the charge and run away off to nowhere
			npc_weegli_blastfuseAI* pEscortAI = dynamic_cast<npc_weegli_blastfuseAI*>(pCreature->AI());
			pEscortAI->DestroyDoor();
        }
        return true;
    }

bool GossipHello_npc_weegli_blastfuse(Player* pPlayer, Creature* pCreature)
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
        {
            switch (pInstance->GetData(EVENT_PYRAMID))
            {
                case PYRAMID_KILLED_ALL_TROLLS:
                    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_WEEGLI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    pPlayer->SEND_GOSSIP_MENU(1514, pCreature->GetGUID());  //if event can proceed to end
                    break;
                case PYRAMID_NOT_STARTED:
                    pPlayer->SEND_GOSSIP_MENU(1511, pCreature->GetGUID());  //if event not started
                    break;
                default:
                    pPlayer->SEND_GOSSIP_MENU(1513, pCreature->GetGUID());  //if event are in progress
            }
            return true;
        }
        return false;
    }

CreatureAI* GetAI_npc_weegli_blastfuse(Creature* pCreature)
{
    return new npc_weegli_blastfuseAI(pCreature);
}

/*######
## go_troll_cage
######*/

void initBlyCrewMember(ScriptedInstance* pInstance, uint32 entry, float x, float y, float z)
{
	if (Creature* crew = pInstance->instance->GetCreature(pInstance->GetData64(entry)))
	{
		crew->SetReactState(REACT_AGGRESSIVE);
		crew->SetWalk(true);
		crew->GetMotionMaster()->MovePoint(0, x, y, z, false);
		CreatureCreatePos pos(crew->GetMap(), x, y, z, 4.723239f);
		crew->SetRespawnCoord(pos);
		crew->setFaction(FACTION_FREED);
	}
}

bool GOUse_go_troll_cage(Player*, GameObject* pGo)
{
	if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
		pInstance->SetData(EVENT_PYRAMID, PYRAMID_CAGES_OPEN);

        //set bly & co to aggressive & start moving to top of stairs
        initBlyCrewMember(pInstance, ENTRY_BLY, 1884.99f, 1263.0f, 41.52f);
        initBlyCrewMember(pInstance, ENTRY_RAVEN, 1882.5f, 1263.0f, 41.52f);
        initBlyCrewMember(pInstance, ENTRY_ORO, 1886.47f, 1266.04f, 41.53f);
        initBlyCrewMember(pInstance, ENTRY_WEEGLI, 1890, 1263.0f, 41.52f);
        initBlyCrewMember(pInstance, ENTRY_MURTA, 1891.19f, 1267.48f, 41.44f);
    }
    return false;
};

/*######
## go_shallow_grave
######*/

enum go_shallow_grave
{
    ZOMBIE           = 7286,
    DEAD_HERO        = 7276,
    ZOMBIE_CHANCE    = 65,
    DEAD_HERO_CHANCE = 10
};

bool GOUse_go_shallow_grave(Player*, GameObject* pGo)
{
        // randomly summon a zombie or dead hero the first time a grave is used
        if (pGo->GetUseCount() == 0)
        {
            uint32 randomchance = urand(0, 100);
            if (randomchance < ZOMBIE_CHANCE)
                pGo->SummonCreature(ZOMBIE, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
            else
                if ((randomchance - ZOMBIE_CHANCE) < DEAD_HERO_CHANCE)
                    pGo->SummonCreature(DEAD_HERO, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
        }
        pGo->AddUse();
        return false;
};

/*######
## at_zumrah
######*/

enum zumrahConsts
{
    ZUMRAH_ID				= 7271,
    ZUMRAH_HOSTILE_FACTION	= 37
};
/*
bool AreaTrigger_at_zumrah(Player* pPlayer, AreaTriggerEntry const* pAt)
{
	Creature* pZumrah = pPlayer->GetClosestCreatureWithEntry(ZUMRAH_ID, 10.0f);

	if (!pZumrah)
		return false;

	pZumrah->setFaction(ZUMRAH_HOSTILE_FACTION);
	return true;
}
*/
void AddSC_zulfarrak()
{
	Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_sergeant_bly";
    pNewScript->pGossipHello =  &GossipHello_npc_sergeant_bly;
    pNewScript->pGossipSelect = &GossipSelect_npc_sergeant_bly;
    pNewScript->GetAI = &GetAI_npc_sergeant_bly;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_weegli_blastfuse";
    pNewScript->pGossipHello =  &GossipHello_npc_weegli_blastfuse;
    pNewScript->pGossipSelect = &GossipSelect_npc_weegli_blastfuse;
    pNewScript->GetAI = &GetAI_npc_weegli_blastfuse;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_shallow_grave";
    pNewScript->pGOUse = &GOUse_go_shallow_grave;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_troll_cage";
    pNewScript->pGOUse = &GOUse_go_troll_cage;
    pNewScript->RegisterSelf();
/*
    pNewScript = new Script;
    pNewScript->Name = "at_zumrah";
    pNewScript->pAreaTrigger = &AreaTrigger_at_zumrah;
    pNewScript->RegisterSelf();*/
}