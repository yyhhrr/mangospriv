/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Tanaris
SD%Complete: 80
SDComment: Quest support: 648, 1560, 2882, 2954, 4005, 10277, 10279(Special flight path). Noggenfogger vendor
SDCategory: Tanaris
EndScriptData */

/* ContentData
mob_aquementas
npc_marin_noggenfogger
npc_oox17tn
npc_stone_watcher_of_norgannon
npc_tooga
npc_yehkinya
go_landmark_treasure
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "follower_ai.h"

/*######
## mob_aquementas
######*/

#define AGGRO_YELL_AQUE     -1000168

#define SPELL_AQUA_JET      13586
#define SPELL_FROST_SHOCK   15089

struct MANGOS_DLL_DECL mob_aquementasAI : public ScriptedAI
{
    mob_aquementasAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 SendItem_Timer;
    uint32 SwitchFaction_Timer;
    bool isFriendly;

    uint32 FrostShock_Timer;
    uint32 AquaJet_Timer;

    void Reset()
    {
        SendItem_Timer = 0;
        SwitchFaction_Timer = 10000;
        m_creature->setFaction(35);
        isFriendly = true;

        AquaJet_Timer = 5000;
        FrostShock_Timer = 1000;
    }

    void SendItem(Unit* receiver)
    {
        if (((Player*)receiver)->HasItemCount(11169,1,false) &&
            ((Player*)receiver)->HasItemCount(11172,11,false) &&
            ((Player*)receiver)->HasItemCount(11173,1,false) &&
            !((Player*)receiver)->HasItemCount(11522,1,true))
        {
            if (Item* pItem = ((Player*)receiver)->StoreNewItemInInventorySlot(11522, 1))
                ((Player*)receiver)->SendNewItem(pItem, 1, true, false);
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(AGGRO_YELL_AQUE, m_creature, who);
    }

    void UpdateAI(const uint32 diff)
    {
        if (isFriendly)
        {
            if (SwitchFaction_Timer < diff)
            {
                m_creature->setFaction(91);
                isFriendly = false;
            }else SwitchFaction_Timer -= diff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!isFriendly)
        {
            if (SendItem_Timer < diff)
            {
                if (m_creature->getVictim()->GetTypeId() == TYPEID_PLAYER)
                    SendItem(m_creature->getVictim());
                SendItem_Timer = 5000;
            }else SendItem_Timer -= diff;
        }

        if (FrostShock_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROST_SHOCK);
            FrostShock_Timer = 15000;
        }else FrostShock_Timer -= diff;

        if (AquaJet_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_AQUA_JET);
            AquaJet_Timer = 15000;
        }else AquaJet_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_aquementas(Creature* pCreature)
{
    return new mob_aquementasAI(pCreature);
}


/*######
## npc_marin_noggenfogger
######*/

bool GossipHello_npc_marin_noggenfogger(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pCreature->isVendor() && pPlayer->GetQuestRewardStatus(2662))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_marin_noggenfogger(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());

    return true;
}

/*######
## npc_oox17tn
######*/

enum
{
    SAY_OOX_START           = -1000287,
    SAY_OOX_AGGRO1          = -1000288,
    SAY_OOX_AGGRO2          = -1000289,
    SAY_OOX_AMBUSH          = -1000290,
    SAY_OOX17_AMBUSH_REPLY  = -1000291,
    SAY_OOX_END             = -1000292,

    QUEST_RESCUE_OOX_17TN   = 648,

    NPC_SCORPION            = 7803,
    NPC_SCOFFLAW            = 7805,
    NPC_SHADOW_MAGE         = 5617
};

struct MANGOS_DLL_DECL npc_oox17tnAI : public npc_escortAI
{
    npc_oox17tnAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    void WaypointReached(uint32 i)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch (i)
        {
            //1. Ambush: 3 scorpions
            case 22:
                DoScriptText(SAY_OOX_AMBUSH, m_creature);
                m_creature->SummonCreature(NPC_SCORPION, -8340.70f, -4448.17f, 9.17f, 3.10f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                m_creature->SummonCreature(NPC_SCORPION, -8343.18f, -4444.35f, 9.44f, 2.35f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                m_creature->SummonCreature(NPC_SCORPION, -8348.70f, -4457.80f, 9.58f, 2.02f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                break;
            //2. Ambush: 2 Rogues & 1 Shadow Mage
            case 28:
                DoScriptText(SAY_OOX_AMBUSH, m_creature);

                m_creature->SummonCreature(NPC_SCOFFLAW, -7488.02f, -4786.56f, 10.67f, 3.74f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                m_creature->SummonCreature(NPC_SHADOW_MAGE, -7486.41f, -4791.55f, 10.54f, 3.26f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);

                if (Creature* pCreature = m_creature->SummonCreature(NPC_SCOFFLAW, -7488.47f, -4800.77f, 9.77f, 2.50f,TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                    DoScriptText(SAY_OOX17_AMBUSH_REPLY,pCreature);

                break;
            case 34:
                DoScriptText(SAY_OOX_END, m_creature);
                // Award quest credit
                pPlayer->GroupEventHappens(QUEST_RESCUE_OOX_17TN, m_creature);
                break;
        }
    }

    void Reset() { }

    void Aggro(Unit*)
    {
        //For an small probability he say something when it aggros
        switch(urand(0, 9))
        {
           case 0: DoScriptText(SAY_OOX_AGGRO1, m_creature); break;
           case 1: DoScriptText(SAY_OOX_AGGRO2, m_creature); break;
        }
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }
};

CreatureAI* GetAI_npc_oox17tn(Creature* pCreature)
{
    return new npc_oox17tnAI(pCreature);
}

bool QuestAccept_npc_oox17tn(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_RESCUE_OOX_17TN)
    {
        DoScriptText(SAY_OOX_START, pCreature);

        pCreature->SetStandState(UNIT_STAND_STATE_STAND);

        if (pPlayer->GetTeam() == ALLIANCE)
            pCreature->setFaction(FACTION_ESCORT_A_PASSIVE);

        if (pPlayer->GetTeam() == HORDE)
            pCreature->setFaction(FACTION_ESCORT_H_PASSIVE);

        if (npc_oox17tnAI* pEscortAI = dynamic_cast<npc_oox17tnAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest);
    }
    return true;
}

/*######
## npc_stone_watcher_of_norgannon
######*/

#define GOSSIP_ITEM_NORGANNON_1     "What function do you serve?"
#define GOSSIP_ITEM_NORGANNON_2     "What are the Plates of Uldum?"
#define GOSSIP_ITEM_NORGANNON_3     "Where are the Plates of Uldum?"
#define GOSSIP_ITEM_NORGANNON_4     "Excuse me? We've been \"reschedueled for visitations\"? What does that mean?!"
#define GOSSIP_ITEM_NORGANNON_5     "So, what's inside Uldum?"
#define GOSSIP_ITEM_NORGANNON_6     "I will return when i have the Plates of Uldum."

bool GossipHello_npc_stone_watcher_of_norgannon(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(2954) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(1674, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_stone_watcher_of_norgannon(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(1675, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(1676, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(1677, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(1678, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(1679, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(2954);
            break;
    }
    return true;
}

/*####
# npc_tooga
####*/

enum
{
    SAY_TOOG_THIRST             = -1000391,
    SAY_TOOG_WORRIED            = -1000392,
    SAY_TOOG_POST_1             = -1000393,
    SAY_TORT_POST_2             = -1000394,
    SAY_TOOG_POST_3             = -1000395,
    SAY_TORT_POST_4             = -1000396,
    SAY_TOOG_POST_5             = -1000397,
    SAY_TORT_POST_6             = -1000398,

    QUEST_TOOGA                 = 1560,
    NPC_TORTA                   = 6015,

    POINT_ID_TO_WATER           = 1
};

const float m_afToWaterLoc[] = {-7032.664551f, -4906.199219f, -1.606446f};

struct MANGOS_DLL_DECL npc_toogaAI : public FollowerAI
{
    npc_toogaAI(Creature* pCreature) : FollowerAI(pCreature) { Reset(); }

    uint32 m_uiCheckSpeechTimer;
    uint32 m_uiPostEventTimer;
    uint32 m_uiPhasePostEvent;

    Unit* pTorta;

    void Reset()
    {
        m_uiCheckSpeechTimer = 2500;
        m_uiPostEventTimer = 1000;
        m_uiPhasePostEvent = 0;

        pTorta = NULL;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        FollowerAI::MoveInLineOfSight(pWho);

        if (!m_creature->getVictim() && !HasFollowState(STATE_FOLLOW_COMPLETE | STATE_FOLLOW_POSTEVENT) && pWho->GetEntry() == NPC_TORTA)
        {
            if (m_creature->IsWithinDistInMap(pWho, INTERACTION_DISTANCE))
            {
                if (Player* pPlayer = GetLeaderForFollower())
                {
                    if (pPlayer->GetQuestStatus(QUEST_TOOGA) == QUEST_STATUS_INCOMPLETE)
                        pPlayer->GroupEventHappens(QUEST_TOOGA, m_creature);
                }

                pTorta = pWho;
                SetFollowComplete(true);
            }
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        FollowerAI::MovementInform(uiMotionType, uiPointId);

        if (uiMotionType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_ID_TO_WATER)
            SetFollowComplete();
    }

    void UpdateFollowerAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            //we are doing the post-event, or...
            if (HasFollowState(STATE_FOLLOW_POSTEVENT))
            {
                if (m_uiPostEventTimer < uiDiff)
                {
                    m_uiPostEventTimer = 5000;

                    if (!pTorta || !pTorta->isAlive())
                    {
                        //something happened, so just complete
                        SetFollowComplete();
                        return;
                    }

                    switch(m_uiPhasePostEvent)
                    {
                        case 1:
                            DoScriptText(SAY_TOOG_POST_1, m_creature);
                            break;
                        case 2:
                            DoScriptText(SAY_TORT_POST_2, pTorta);
                            break;
                        case 3:
                            DoScriptText(SAY_TOOG_POST_3, m_creature);
                            break;
                        case 4:
                            DoScriptText(SAY_TORT_POST_4, pTorta);
                            break;
                        case 5:
                            DoScriptText(SAY_TOOG_POST_5, m_creature);
                            break;
                        case 6:
                            DoScriptText(SAY_TORT_POST_6, pTorta);
                            m_creature->GetMotionMaster()->MovePoint(POINT_ID_TO_WATER, m_afToWaterLoc[0], m_afToWaterLoc[1], m_afToWaterLoc[2]);
                            break;
                    }

                    ++m_uiPhasePostEvent;
                }
                else
                    m_uiPostEventTimer -= uiDiff;
            }
            //...we are doing regular speech check
            else if (HasFollowState(STATE_FOLLOW_INPROGRESS))
            {
                if (m_uiCheckSpeechTimer < uiDiff)
                {
                    m_uiCheckSpeechTimer = 5000;

                    switch(urand(0, 50))
                    {
                        case 10: DoScriptText(SAY_TOOG_THIRST, m_creature); break;
                        case 25: DoScriptText(SAY_TOOG_WORRIED, m_creature); break;
                    }
                }
                else
                    m_uiCheckSpeechTimer -= uiDiff;
            }

            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_tooga(Creature* pCreature)
{
    return new npc_toogaAI(pCreature);
}

bool QuestAccept_npc_tooga(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_TOOGA)
    {
        if (npc_toogaAI* pToogaAI = dynamic_cast<npc_toogaAI*>(pCreature->AI()))
            pToogaAI->StartFollow(pPlayer, FACTION_ESCORT_N_FRIEND_PASSIVE, pQuest);
    }

    return true;
}

/*######
## npc_yehkinya
######*/

#define GOSSIP_SCROLL     "I'd like to be able to perform the ritual again."

enum eYehkinya
{
    SPELL_CREATE_ITEM_YEHKINYAS_SCROLL    = 12998,
    ITEM_YEHKINYAS_SCROLL                 = 10818,
    QUEST_THE_GOD_HAKKAR                  = 3528
};

bool GossipHello_npc_yehkinya(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (!pPlayer->HasItemCount(ITEM_YEHKINYAS_SCROLL, 1, true) && pPlayer->GetQuestRewardStatus(QUEST_THE_GOD_HAKKAR))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SCROLL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_yehkinya(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, SPELL_CREATE_ITEM_YEHKINYAS_SCROLL, false);
    }

    return true;
}

/*######
## go_landmark_treasure
######*/

enum eGold
{
    NPC_BUCCANEER         = 7902,
    NPC_PIRATE            = 7899,
    NPC_SWASHBUCKLER      = 7901,
    QUEST_CUERGOS_GOLD    = 2882,
    GO_TREASURE           = 142194,
    MAX_PIRATE            = 6
};

static float Point[6][3] = 
{
    {-10117.2f,-4083.79f,4.09952f},
    {-10134.6f,-4090.37f,2.61774f},
    {-10148.5f,-4083.61f,2.12749f},
    {-10159.1f,-4063.23f,3.00599f},
    {-10160.4f,-4049.66f,3.04275f},
    {-10156.3f,-4031.97f,3.6349f}
};

bool GOUse_go_landmark_treasure(Player *pPlayer, GameObject*)
{
    if (pPlayer->GetQuestStatus(QUEST_CUERGOS_GOLD) == QUEST_STATUS_INCOMPLETE)
    {
        uint32 Entry = 0;
        for(uint32 i = 0; i < MAX_PIRATE; ++i)
        {
            switch (urand(0,2))
            {
                case 0:
                    Entry = NPC_PIRATE;
                  break;
                case 1:
                    Entry = NPC_SWASHBUCKLER;
                    break;
                case 2:
                    Entry = NPC_BUCCANEER;
                    break;
            }
			Creature* spawn = nullptr;
            if ((spawn = pPlayer->SummonCreature(Entry, -10038.2f, -4032.07f, 17.8228f, 3.42121f, TEMPSUMMON_TIMED_DESPAWN, 340000)) != nullptr)
                spawn->GetMotionMaster()->MovePoint(0, Point[i][0], Point[i][1], Point[i][2]);
        }
    }
    return true;
};


/*######
## npc_mux_manascrambler
######*/

enum
{
    QUEST_IN_SEARCH_OF_ANTHION_A  = 8929,
    QUEST_IN_SEARCH_OF_ANTHION_H  = 8930,

    ITEM_GHOST_REVEALER           = 22115,

    SPELL_CREATE_REVEALER         = 27754
};

bool GossipHello_npc_mux_manascrambler(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    // TODO: More strict checks are possible. Need to do some research to be sure when this option should be available.
    if ((pPlayer->IsActiveQuest(QUEST_IN_SEARCH_OF_ANTHION_A) || pPlayer->IsActiveQuest(QUEST_IN_SEARCH_OF_ANTHION_H))
        && !pPlayer->HasItemCount(ITEM_GHOST_REVEALER, 1))
        // TODO: Get the correct gossip text, afterwards move this to database.
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] I have lost my Extra-Dimensional Ghost Revealer. Could you give me one more?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_mux_manascrambler(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
    if (uiAction != GOSSIP_ACTION_INFO_DEF)
        return true;

    int32 uiRequiredMoney = 50000; // TODO: Possible differnt amount of money required, or even no money is required.
    if (pPlayer->GetMoney() >= uint32(uiRequiredMoney))
    {
        pPlayer->ModifyMoney(-uiRequiredMoney);
        pCreature->CastSpell(pPlayer, SPELL_CREATE_REVEALER, true);
    }
    else
        pPlayer->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, pCreature, 0, 0);

    pPlayer->CLOSE_GOSSIP_MENU(); // TODO: May the creature show up some gossip menu.
    return true;                  //       Need to get the right here and because of this
                                  //       the menu closing should be removed.
}


enum 
{
	QUEST_LOVE_SONG_FOR_NARAIN = 8599,
	SPELL_SIRENS_SONG = 25678, //25688 Narain!
	//25773 will of weavil
	//25784 Summon Narain's Special Bag DND
	//25785 Summon Arcanite Buoy DND
	//25806 creature of nightmare 
	//25813 conjure dream rift
	//25846 eranikus night elf dnd
	//26009 rotate 360
	//15553 Doctor Weavil's Flying Machine (NPC)
};


bool GossipHello_npc_siren(Player* pPlayer, Creature* pCreature)
{
	if (pCreature->isQuestGiver())
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

	if (pPlayer->GetQuestStatus(QUEST_LOVE_SONG_FOR_NARAIN) == QUEST_STATUS_COMPLETE && pPlayer->GetQuestRewardStatus(QUEST_LOVE_SONG_FOR_NARAIN))
		pCreature->CastSpell(pPlayer, SPELL_SIRENS_SONG, true);

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

	return true;
}

bool GossipHello_npc_narain(Player* pPlayer, Creature* pCreature)
{
	if (pCreature->isQuestGiver())
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

	if (pPlayer->GetQuestStatus(8620) == QUEST_STATUS_INCOMPLETE)
	{
		pPlayer->SEND_GOSSIP_MENU(90214, pCreature->GetObjectGuid());
		return true;
	}

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

	return true;
}

void AddSC_tanaris()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "mob_aquementas";
    pNewscript->GetAI = &GetAI_mob_aquementas;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_marin_noggenfogger";
    pNewscript->pGossipHello =  &GossipHello_npc_marin_noggenfogger;
    pNewscript->pGossipSelect = &GossipSelect_npc_marin_noggenfogger;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_oox17tn";
    pNewscript->GetAI = &GetAI_npc_oox17tn;
    pNewscript->pQuestAcceptNPC = &QuestAccept_npc_oox17tn;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_stone_watcher_of_norgannon";
    pNewscript->pGossipHello =  &GossipHello_npc_stone_watcher_of_norgannon;
    pNewscript->pGossipSelect = &GossipSelect_npc_stone_watcher_of_norgannon;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_tooga";
    pNewscript->GetAI = &GetAI_npc_tooga;
    pNewscript->pQuestAcceptNPC = &QuestAccept_npc_tooga;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_yehkinya";
    pNewscript->pGossipHello =  &GossipHello_npc_yehkinya;
    pNewscript->pGossipSelect = &GossipSelect_npc_yehkinya;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "go_landmark_treasure";
    pNewscript->pGOUse = &GOUse_go_landmark_treasure;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "npc_mux_manascrambler";
	pNewscript->pGossipHello = &GossipHello_npc_mux_manascrambler;
	pNewscript->pGossipSelect = &GossipSelect_npc_mux_manascrambler;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "npc_aq_opening_siren";
	pNewscript->pGossipHello = &GossipHello_npc_siren;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "npc_aq_narain";
	pNewscript->pGossipHello = &GossipHello_npc_narain;
	pNewscript->RegisterSelf();
}