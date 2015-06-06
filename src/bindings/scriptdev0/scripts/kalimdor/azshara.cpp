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
SDName: Azshara
SD%Complete: 90
SDComment: Quest support: 2744, 3141, 9364
SDCategory: Azshara
EndScriptData */

/* ContentData
go_southfury_moonstone
mobs_spitelashes
npc_loramus_thalipedes
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "World.h"

/*######
## mobs_spitelashes
######*/

enum
{
    QUEST_FRAGMENTED_MAGIC     = 9364,

    SPELL_CLONE_SHEEP          = 28406,
    SPELL_VISUAL_SHEEP_END     = 6924
};

struct MANGOS_DLL_DECL mobs_spitelashesAI : public ScriptedAI
{
    mobs_spitelashesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        m_uiMorphTimer = 0;
        m_bIsPolymorphed = false;
        m_bHasClones = false;
    }

    uint32 m_uiMorphTimer;
    bool m_bIsPolymorphed, m_bHasClones;

    void Reset() {}

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        uint32 uiSpellID = pSpell->Id;
        if (!m_bIsPolymorphed &&
            pCaster->GetTypeId() == TYPEID_PLAYER && ((Player*)pCaster)->GetQuestStatus(QUEST_FRAGMENTED_MAGIC) == QUEST_STATUS_INCOMPLETE &&
            (uiSpellID == 118 || uiSpellID == 12824 || uiSpellID == 12825 || uiSpellID == 12826)) // Polymorph: Sheep spells
            m_bIsPolymorphed = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsPolymorphed)
        {
            if (m_uiMorphTimer >= 5000 && !m_bHasClones)
            {
                m_creature->CastSpell(m_creature, SPELL_CLONE_SHEEP, false); // Summon clones
                m_creature->CastSpell(m_creature, SPELL_VISUAL_SHEEP_END, false); // Visual explosion
                m_bHasClones = true;
                m_uiMorphTimer = 0;
                return;
            }
            else if (!m_bHasClones)
                m_uiMorphTimer += uiDiff;

            if (m_bHasClones)
            {
                m_bIsPolymorphed = false;
                m_bHasClones = false;
                m_creature->ForcedDespawn();
            }
            return; // We do not need to continue when polymorphed
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //TODO: Add abilities for each creature
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mobs_spitelashes(Creature* pCreature)
{
    return new mobs_spitelashesAI(pCreature);
}

/*######
## npc_loramus_thalipedes
######*/

bool GossipHello_npc_loramus_thalipedes(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(2744) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Can you help me?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (pPlayer->GetQuestStatus(3141) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tell me your story", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_loramus_thalipedes(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(2744);
            break;

        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Please continue", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            pPlayer->SEND_GOSSIP_MENU(1813, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+21:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I do not understand", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            pPlayer->SEND_GOSSIP_MENU(1814, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+22:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Indeed", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);
            pPlayer->SEND_GOSSIP_MENU(1815, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+23:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I will do this with or your help, Loramus", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);
            pPlayer->SEND_GOSSIP_MENU(1816, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+24:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);
            pPlayer->SEND_GOSSIP_MENU(1817, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+25:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(3141);
            break;
    }
    return true;
}

/*######
## npc_fuest_hydraxis
######*/

bool GossipHello_npc_fuest_hydraxis(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    bool canBuy;
    canBuy = false;
    uint32 vendor = pCreature->GetEntry();

	switch (vendor)
    {
		case 13278: 
		if (pPlayer->GetQuestStatus(6824) == QUEST_STATUS_COMPLETE && pPlayer->GetReputationRank(749) >= REP_HONORED)
			canBuy = true;
		else
			pPlayer->SEND_GOSSIP_MENU(90000, pCreature->GetObjectGuid());
		break;
	}

	if (canBuy)
    {
        if (pCreature->isVendor())
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        pPlayer->SEND_GOSSIP_MENU(90001, pCreature->GetObjectGuid());
    }
    return true;
}

bool GossipSelect_npc_fuest_hydraxis(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());

    return true;
}


enum
{
	QUEST_CHARGE_OF_THE_DRAGONFLIGHTS = 8555,
	SPELL_CREATE_MAGICAL_LEDGER = 25600,
	ITEM_MAGICAL_LEDGER = 20949,
};


bool GossipHello_npc_ghost_of_azuregos(Player* pPlayer, Creature* pCreature)
{
	if (pCreature->isQuestGiver())
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

	if (pPlayer->GetQuestStatus(QUEST_CHARGE_OF_THE_DRAGONFLIGHTS) == QUEST_STATUS_COMPLETE && pPlayer->GetQuestRewardStatus(QUEST_CHARGE_OF_THE_DRAGONFLIGHTS))
	{
		if (pPlayer->HasItemCount(ITEM_MAGICAL_LEDGER, 1))
		{
			pPlayer->SEND_GOSSIP_MENU(90213, pCreature->GetObjectGuid());
			return true;
		}

		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -1540200, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90200, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_ghost_of_azuregos(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
	auto a = uiAction - (GOSSIP_ACTION_INFO_DEF + 1);
	int gossip_id = -1540201 - a;
	int menu_id = 90201 + a;

	if (gossip_id > -1540213)
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, gossip_id, GOSSIP_SENDER_MAIN, uiAction + 1);
	else
	{
		pCreature->MonsterSay("I SAID GOODBYE!", 0);
		pPlayer->CLOSE_GOSSIP_MENU();
		pCreature->CastSpell(pPlayer, SPELL_CREATE_MAGICAL_LEDGER, true);
		return true;
	}

	pPlayer->SEND_GOSSIP_MENU(menu_id, pCreature->GetGUID());
	return true;
}

enum
{
	NPC_MAWS = 15571,

	SPELL_FRENZY = 28371,
	SPELL_DARK_WATER = 25743,
	SPELL_RAMPAGE = 25744,
};

struct MANGOS_DLL_DECL mobs_maws : public ScriptedAI
{
	uint32 enrageTimer = 25000;
	uint32 rampageTimer = 3000;
	uint32 darkWaterTimer = 5500;

	mobs_maws(Creature* pCreature) : ScriptedAI(pCreature)
	{
	}

	void Reset() override 
	{
		enrageTimer = 25000;
		rampageTimer = 3000;
		darkWaterTimer = 5500;
	}

	void JustDied(Unit* killer) override
	{
		BroadcastMessage("The beast returns from whence it came. The wrath of Neptulon has subsided.", BroadcastColor::Emote);
		if (killer->GetTypeId() != TYPEID_PLAYER)
			return;

		SendMail(MAIL_CREATURE, static_cast<Player*>(killer), 11811, "Champion of the Blue Dragonflight", "Champion,$B$BThe Blue Dragonflight believes that you are fated for greater things than even I could predict.$B$BDespite what some would have you believe, our world needs heroes. In your darkest hour, when you face the blight of Kalimdor, recall the epic journies, trials, and tribulations that you undertook for an old gnome and let those memories lift your spirits.$B$BHumbly,$B$BNarain Soothfancy$B$BP.S. Sorry about the repair costs.", 1000000);
	}

	void UpdateAI(uint32 timeDelta) override
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		auto victim = m_creature->getVictim();

		if (enrageTimer < timeDelta)
		{
			DoCastSpellIfCan(victim, SPELL_FRENZY);
			if (m_creature->GetHealthPercent() <= 25.f)
				enrageTimer = 15000;
			else
				enrageTimer = 25000;
		}
		else
			enrageTimer -= timeDelta;

		if (rampageTimer < timeDelta)
		{
			DoCastSpellIfCan(victim, SPELL_RAMPAGE);
			rampageTimer = 15000;
		}
		else
			rampageTimer -= timeDelta;

		if (darkWaterTimer < timeDelta)
		{
			DoCastSpellIfCan(victim, SPELL_DARK_WATER);
			darkWaterTimer = 5000;
		}
		else
			darkWaterTimer -= timeDelta;

		DoMeleeAttackIfReady();
	}
};

bool OnEvent_PlaceArcaniteBuoy(uint32 eventId, Object* pSource, Object* pTarget, bool isStart)
{
	if (pSource->GetTypeId() != TYPEID_PLAYER)
		return true;

	auto plr = static_cast<Player*>(pSource);
	const float summonX = 0.0f, summonY = 0.0f, summonZ = 0.0f, angle = 0.0f;
	plr->SummonCreature(NPC_MAWS, summonX, summonY, summonZ, angle, TEMPSUMMON_DEAD_DESPAWN, 0);

	return true;
}

void AddSC_azshara()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mobs_spitelashes";
    pNewScript->GetAI = &GetAI_mobs_spitelashes;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_loramus_thalipedes";
    pNewScript->pGossipHello = &GossipHello_npc_loramus_thalipedes;
    pNewScript->pGossipSelect = &GossipSelect_npc_loramus_thalipedes;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_fuest_hydraxis";
	pNewScript->pGossipHello = &GossipHello_npc_fuest_hydraxis;
	pNewScript->pGossipSelect = &GossipSelect_npc_fuest_hydraxis;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "AQ_OPENING_AZUREGOS";
	pNewScript->pGossipHello = &GossipHello_npc_ghost_of_azuregos;
	pNewScript->pGossipSelect = &GossipSelect_npc_ghost_of_azuregos;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "aq_opening_maws";
	pNewScript->GetAI = [](Creature* c) -> CreatureAI* { return new mobs_maws(c); };
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "AQ_SUMMON_MAW";
	pNewScript->pProcessEventId = &OnEvent_PlaceArcaniteBuoy;
	pNewScript->RegisterSelf();
}
