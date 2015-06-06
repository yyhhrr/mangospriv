/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################

Copyright (C) 2013 Dotox <http://www.mmobase.de>

*/

#include "precompiled.h"
#include "LFGHandler.h"

bool GossipHello_npc_meetingstone_innkeeper(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->getLevel() >= 13)
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "K\303\266nnt Ihr mir helfen eine Gruppe zu finden?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_meetingstone_innkeeper(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
	{
        case GOSSIP_ACTION_INFO_DEF+1:
			{
			if (pPlayer->getLevel() >= 13 && pPlayer->getLevel() <= 16)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Flammenschlund", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 17 && pPlayer->getLevel() <= 19)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Flammenschlund", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Todesmine", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "H\303\266hle des Wehklagens", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 20 && pPlayer->getLevel() <= 21)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Flammenschlund", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Todesmine", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "H\303\266hle des Wehklagens", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Burg Schattenfang", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tiefschwarze Grotte", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 24 && pPlayer->getLevel() <= 28)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Todesmine", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "H\303\266hle des Wehklagens", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Burg Schattenfang", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tiefschwarze Grotte", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gnomeregan", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schalachrote Kloster", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 33 && pPlayer->getLevel() <= 39)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tiefschwarze Grotte", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gnomeregan", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schalachrote Kloster", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Kral der Klingenhauer", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+9);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 40 && pPlayer->getLevel() <= 41)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schalachrote Kloster", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "H\303\274gel der Klingenhauer", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+10);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Maraudon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Uldaman", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+12);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 43 && pPlayer->getLevel() <= 47)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schalachrote Kloster", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "H\303\274gel der Klingenhauer", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+10);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Maraudon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Uldaman", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+12);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zul'Farrak", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+13);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 48 && pPlayer->getLevel() <= 49)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schalachrote Kloster", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "H\303\274gel der Klingenhauer", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+10);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Maraudon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Uldaman", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+12);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zul'Farrak", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+13);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schwarzfelstiefen", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+14);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 55 && pPlayer->getLevel() <= 57)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zul'Farrak", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+13);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schwarzfelstiefen", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+14);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "D\303\274sterbruch", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+15);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			if (pPlayer->getLevel() >= 58)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Schwarzfelstiefen", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+14);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "D\303\274sterbruch", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+15);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Scholomance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+16);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Stratholme", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+17);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Blackrockspitze", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+18);

				pPlayer->SEND_GOSSIP_MENU(7180, pCreature->GetObjectGuid());
			}
			break;

		case GOSSIP_ACTION_INFO_DEF+2:								// Flammenschlund
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(2437, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+3:								// Todesmine
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(1581, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+4:								// Höhle des Wehklagens
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(718, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+5:								// Burg Schattenfang
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(209, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+6:								// Grotte
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(719, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+7:								// Gnomeregan
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(721, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+8:								// Kloster
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(796, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+9:								// Kral der Klingenhauer
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(491, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+10:								// Hügel der Klingenhauer
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(722, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+11:								// Maraudon
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(2100, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+12:								// Uldaman
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(1337, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+13:								// Zul'Farrak
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(978, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+14:								// Schwarzfelstiefen
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(1584, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+15:								// Diremaul
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(2557, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+16:								// Scholomance
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(2057, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+17:								// Stratholme
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(2017, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;

		case GOSSIP_ACTION_INFO_DEF+18:								// Schwarzfelsspitze
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->GetSession()->SendMeetingstoneSetqueue(1583, MEETINGSTONE_STATUS_JOINED_QUEUE);
			break;
	}
    return true;
}

void AddSC_meetingstone_innkeeper()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "npc_meetingstone_innkeeper";
    pNewScript->pGossipHello = &GossipHello_npc_meetingstone_innkeeper;
    pNewScript->pGossipSelect = &GossipSelect_npc_meetingstone_innkeeper;
    pNewScript->RegisterSelf();
}