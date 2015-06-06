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
SDName: Blasted_Lands
SD%Complete: 90
SDComment: Quest support: 2784, 2801, 3628. Missing some texts for Fallen Hero. Teleporter to Rise of the Defiler missing group support.
SDCategory: Blasted Lands
EndScriptData */

/* ContentData
npc_deathly_usher
npc_fallen_hero_of_horde
EndContentData */

#include "precompiled.h"

/*######
## npc_deathly_usher
######*/

#define GOSSIP_ITEM_USHER "I wish to to visit the Rise of the Defiler."

enum
{
    SPELL_TELEPORT_SINGLE            = 12885,
    SPELL_TELEPORT_SINGLE_IN_GROUP   = 13142,
    SPELL_TELEPORT_GROUP             = 27686
};

bool GossipHello_npc_deathly_usher(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(3628) == QUEST_STATUS_INCOMPLETE && pPlayer->HasItemCount(10757, 1))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_USHER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

     pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_deathly_usher(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, SPELL_TELEPORT_SINGLE, true);
    }

    return true;
}

/*######
## npc_fallen_hero_of_horde
######*/

#define GOSSIP_ITEM_FALLEN "Continue..."

#define GOSSIP_ITEM_FALLEN1 "What could be worse than death?"
#define GOSSIP_ITEM_FALLEN2 "Subordinates?"
#define GOSSIP_ITEM_FALLEN3 "What are the stones of binding?"
#define GOSSIP_ITEM_FALLEN4 "You can count on me, Hero"
#define GOSSIP_ITEM_FALLEN5 "I shall"

bool GossipHello_npc_fallen_hero_of_horde(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(2784) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Why are you here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (pPlayer->GetQuestStatus(2801) == QUEST_STATUS_INCOMPLETE && pPlayer->GetTeam() == HORDE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Continue story...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    if (pPlayer->GetQuestStatus(2801) == QUEST_STATUS_INCOMPLETE && pPlayer->GetTeam() == ALLIANCE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Why are you here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_fallen_hero_of_horde(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            pPlayer->SEND_GOSSIP_MENU(1392, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+11:
            pPlayer->SEND_GOSSIP_MENU(1411, pCreature->GetObjectGuid());
            if (pPlayer->GetQuestStatus(2784) == QUEST_STATUS_INCOMPLETE)
                pPlayer->AreaExploredOrEventHappens(2784);
            if (pPlayer->GetTeam() == ALLIANCE)
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                pPlayer->SEND_GOSSIP_MENU(1411, pCreature->GetObjectGuid());
            }
            break;

        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            pPlayer->SEND_GOSSIP_MENU(1451, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+21:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            pPlayer->SEND_GOSSIP_MENU(1452, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+22:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
            pPlayer->SEND_GOSSIP_MENU(1453, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+23:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
            pPlayer->SEND_GOSSIP_MENU(1454, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+24:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
            pPlayer->SEND_GOSSIP_MENU(1455, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+25:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FALLEN5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 26);
            pPlayer->SEND_GOSSIP_MENU(1456, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+26:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(2801);
            break;
    }
    return true;
}

/*
### Felguard Elite - ID: 8717 ###
*/

#define SPELL_STRIKE		13737
#define SPELL_CLEAVE		15284
#define SPELL_BLAST_WAVE	16046

struct MANGOS_DLL_DECL npc_felguardAI : public ScriptedAI
{
    npc_felguardAI(Creature *c) : ScriptedAI(c)
	{
        Reset();
    }

	ScriptedInstance* pInstance;
    Unit* target;

	uint32 m_uiStrikeTimer;
	uint32 m_uiCleaveTimer;
	uint32 m_uiBlastWaveTimer;

    void Reset()
    {
        m_uiStrikeTimer = urand(3000,5000);
		m_uiCleaveTimer = urand(7000,9000);
		m_uiBlastWaveTimer	= urand(8000,10000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiStrikeTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_STRIKE);
            m_uiStrikeTimer = random(8000,10000);
        } else m_uiStrikeTimer -= diff;

		if (m_uiCleaveTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = random(12000,14000);
        } else m_uiCleaveTimer -= diff;

        if (m_uiBlastWaveTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLAST_WAVE);
            m_uiBlastWaveTimer = random(14000,18000);
        } else m_uiBlastWaveTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_felguard(Creature *_Creature)
{
    return new npc_felguardAI (_Creature);
}


enum
{
	QUEST_SUPPRESSION = 7583,

	ITEM_IMPRISONED_DOOMGUARD = 18605,
};

void Prision_FillLoot(Loot& loot, Player* owner)
{
	if(owner->GetQuestStatus(QUEST_SUPPRESSION) == QUEST_STATUS_INCOMPLETE && owner->HasItemCount(ITEM_IMPRISONED_DOOMGUARD, 1, true) == false)
		loot.AddItem(LootStoreItem(ITEM_IMPRISONED_DOOMGUARD, 100, 0, 0, 1, 1));
}


void AddSC_blasted_lands()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_deathly_usher";
    pNewScript->pGossipHello = &GossipHello_npc_deathly_usher;
    pNewScript->pGossipSelect = &GossipSelect_npc_deathly_usher;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_fallen_hero_of_horde";
    pNewScript->pGossipHello = &GossipHello_npc_fallen_hero_of_horde;
    pNewScript->pGossipSelect = &GossipSelect_npc_fallen_hero_of_horde;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name="npc_felguard";
    pNewScript->GetAI = &GetAI_npc_felguard;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->LootId = 16668;
	pNewScript->pFillLoot = Prision_FillLoot;
	pNewScript->RegisterAsLootScript();
}
