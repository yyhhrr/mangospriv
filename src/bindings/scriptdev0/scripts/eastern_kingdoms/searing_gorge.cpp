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
SDName: Searing_Gorge
SD%Complete: 80
SDComment: Quest support: 3367, 3377, 3441 (More accurate info on Kalaran needed). Lothos Riftwaker teleport to Molten Core.
SDCategory: Searing Gorge
EndScriptData */

/* ContentData
npc_kalaran_windblade
npc_zamael_lunthistle
npc_lothos_riftwaker
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "./ObjectMgr.h"

/*######
## npc_kalaran_windblade
######*/

bool GossipHello_npc_kalaran_windblade(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(3441) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tell me what drives this vengance?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_kalaran_windblade(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Continue please", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(1954, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Let me confer with my colleagues", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(1955, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(3441);
            break;
    }
    return true;
}

/*######
## npc_zamael_lunthistle
######*/

bool GossipHello_npc_zamael_lunthistle(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(3377) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tell me your story", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(1920, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_zamael_lunthistle(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Please continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(1921, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Goodbye", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(1922, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(3377);
            break;
    }
    return true;
}

/*######
## npc_lothos_riftwaker
######*/

bool GossipHello_npc_lothos_riftwaker(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestRewardStatus(7487) || pPlayer->GetQuestRewardStatus(7848))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleportiert mich zum Geschmolzenen Kern", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_lothos_riftwaker(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

		if(!pPlayer->isAlive())
		{
			pPlayer->ResurrectPlayer(0.5f);
			pPlayer->SpawnCorpseBones();
		}

		pPlayer->TeleportTo(409, 1096.0f, -467.0f, -104.6f, 3.64f);
    }

    return true;
}

/*######
## npc_dorius_stonetender
######*/

enum
{
	SAY_DORIUS_AGGRO_1 = -1000993,
	SAY_DORIUS_AGGRO_2 = -1000994,

	NPC_DARK_IRON_STEELSHIFTER = 8337,
	MAX_STEELSHIFTERS = 4,

	QUEST_ID_SUNTARA_STONES = 3367,
};

struct MANGOS_DLL_DECL npc_dorius_stonetenderAI : public npc_escortAI
{
	npc_dorius_stonetenderAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

	void Reset() override { }

	void Aggro(Unit* pWho) override
	{
		DoScriptText(urand(0, 1) ? SAY_DORIUS_AGGRO_1 : SAY_DORIUS_AGGRO_2, m_creature, pWho);
	}

	void ReceiveAIEvent(AIEventType eventType, Creature* /*pSender*/, Unit* pInvoker, uint32 uiMiscValue) override
	{
		if (eventType == AI_EVENT_START_ESCORT && pInvoker->GetTypeId() == TYPEID_PLAYER)
		{
			// ToDo: research if there is any text here
			m_creature->SetStandState(UNIT_STAND_STATE_STAND);
			m_creature->SetFactionTemporary(FACTION_ESCORT_A_NEUTRAL_PASSIVE, TEMPFACTION_RESTORE_RESPAWN);
			Start(false, (Player*)pInvoker, GetQuestTemplateStore(uiMiscValue), true);
		}
	}

	void WaypointReached(uint32 uiPointId) override
	{
		switch (uiPointId)
		{
		case 20:
			// ToDo: research if there is any text here!
			float fX, fY, fZ;
			for (uint8 i = 0; i < MAX_STEELSHIFTERS; ++i)
			{
				m_creature->GetNearPoint(m_creature, fX, fY, fZ, 0, 15.0f, i * M_PI_F / 2);
				m_creature->SummonCreature(NPC_DARK_IRON_STEELSHIFTER, fX, fY, fZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000);
			}
			break;
		case 33:
			// ToDo: research if there is any event and text here!
			if (Player* pPlayer = GetPlayerForEscort())
				pPlayer->GroupEventHappens(QUEST_ID_SUNTARA_STONES, m_creature);
			m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
			break;
		}
	}

	void JustSummoned(Creature* pSummoned) override
	{
		if (pSummoned->GetEntry() == NPC_DARK_IRON_STEELSHIFTER)
			pSummoned->AI()->AttackStart(m_creature);
	}

	void UpdateEscortAI(const uint32 uiDiff) override
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_npc_dorius_stonetender(Creature* pCreature)
{
	return new npc_dorius_stonetenderAI(pCreature);
}

bool QuestAccept_npc_dorius_stonetender(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
	if (pQuest->GetQuestId() == QUEST_ID_SUNTARA_STONES)
	{
		pCreature->AI()->SendAIEvent(AI_EVENT_START_ESCORT, pPlayer, pCreature, pQuest->GetQuestId());
		return true;
	}

	return false;
}

void AddSC_searing_gorge()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_kalaran_windblade";
    pNewScript->pGossipHello =  &GossipHello_npc_kalaran_windblade;
    pNewScript->pGossipSelect = &GossipSelect_npc_kalaran_windblade;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_zamael_lunthistle";
    pNewScript->pGossipHello =  &GossipHello_npc_zamael_lunthistle;
    pNewScript->pGossipSelect = &GossipSelect_npc_zamael_lunthistle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_lothos_riftwaker";
    pNewScript->pGossipHello =  &GossipHello_npc_lothos_riftwaker;
    pNewScript->pGossipSelect = &GossipSelect_npc_lothos_riftwaker;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_dorius_stonetender";
	pNewScript->GetAI = &GetAI_npc_dorius_stonetender;
	pNewScript->pQuestAcceptNPC = &QuestAccept_npc_dorius_stonetender;
	pNewScript->RegisterSelf();
}
