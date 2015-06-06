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
SDName: Areatrigger_Scripts
SD%Complete: 100
SDComment: Quest support: 6681
SDCategory: Areatrigger
EndScriptData */

/* ContentData
at_ravenholdt
at_childrens_week_spot          3546,3547,3548,3552,3549,3550
at_scent_larkorwi
EndContentData */

#include "precompiled.h"

/*######
## at_childrens_week_spot
######*/

uint32 TriggerOrphanSpell[6][3] =
{
    {3546, 14305, 65056},   // The Bough of the Eternals
    {3547, 14444, 65059},   // Lordaeron Throne Room
    {3548, 14305, 65055},   // The Stonewrought Dam
    {3549, 14444, 65058},   // Gateway to the Frontier
    {3550, 14444, 65057},   // Down at the Docks
    {3552, 14305, 65054}    // Spooky Lighthouse
};

bool AreaTrigger_at_childrens_week_spot(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    for (uint8 i = 0; i < 6; ++i)
    {
        if (pAt->id == TriggerOrphanSpell[i][0] &&
            pPlayer->GetMiniPet() && pPlayer->GetMiniPet()->GetEntry() == TriggerOrphanSpell[i][1])
        {
            pPlayer->CastSpell(pPlayer, TriggerOrphanSpell[i][2], true);
            return true;
        }
    }
    return false;
}

/*######
## at_ravenholdt
######*/

enum
{
    QUEST_MANOR_RAVENHOLDT   = 6681,
    NPC_RAVENHOLDT           = 13936
};

bool AreaTrigger_at_ravenholdt(Player* pPlayer, AreaTriggerEntry const*)
{
    if (pPlayer->GetQuestStatus(QUEST_MANOR_RAVENHOLDT) == QUEST_STATUS_INCOMPLETE)
        pPlayer->KilledMonsterCredit(NPC_RAVENHOLDT);

    return false;
}

/*######
## at_scent_larkorwi
######*/

enum

{
    QUEST_SCENT_OF_LARKORWI   = 4291,
    NPC_LARKORWI_MATE         = 9683
};

bool AreaTrigger_at_scent_larkorwi(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (!pPlayer->isDead() && pPlayer->GetQuestStatus(QUEST_SCENT_OF_LARKORWI) == QUEST_STATUS_INCOMPLETE)
    {
        if (!GetClosestCreatureWithEntry(pPlayer, NPC_LARKORWI_MATE, 15))
            pPlayer->SummonCreature(NPC_LARKORWI_MATE, pAt->x, pAt->y, pAt->z, 3.3f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 100000);
    }

    return false;
}

bool Areatrigger_OfficerLounges(Player* pPlayer, AreaTriggerEntry const* pAt)
{
	auto rank = pPlayer->GetHonorRankInfo().visualRank;
	if(rank < 6 && !pPlayer->isGameMaster())
		return true;

	
	if(pAt->id == 2527 && (pPlayer->GetTeam() == HORDE || pPlayer->isGameMaster()))
	{
		pPlayer->TeleportTo(450, 221.322f, 74.4933f, 25.7195f, 0.484836f);
		return true;
	}
	else if(pAt->id == 2532 && (pPlayer->GetTeam() == ALLIANCE || pPlayer->isGameMaster()))
	{
		pPlayer->TeleportTo(449, -0.299116f, 4.39156f, -0.255884f, 1.54805f);
		return true;
	}

	return true;
}

enum 
{
	NPC_TWILIGHT_CORRUPTOR = 15625,
	TEXT_WHIPSER = -1540213,
	ITEM_DUSKWOOD_FRAGMENT = 21149,
	QUEST_NIGHTMARES_CORRUPTION = 8735,
};

const float spawnX = -10328.16f, spawnY = -486.24f, spawnZ = 50.02f, angle = 5.97f;

bool Areatrigger_Duskwood(Player* pPlayer, AreaTriggerEntry const* pAt)
{
	std::list<Creature*> creatureList;
	GetCreatureListWithEntryInGrid(creatureList, pPlayer, NPC_TWILIGHT_CORRUPTOR, 350.f);

	for (auto creature : creatureList)
	{
		if (creature->isAlive())
			return true;
	}

	if (pPlayer->GetQuestStatus(QUEST_NIGHTMARES_CORRUPTION) != QUEST_STATUS_COMPLETE && 
		!pPlayer->GetQuestRewardStatus(QUEST_NIGHTMARES_CORRUPTION) && 
		!pPlayer->HasItemCount(ITEM_DUSKWOOD_FRAGMENT, 1, true))
	{
		auto twilightCorruptor = pPlayer->SummonCreature(NPC_TWILIGHT_CORRUPTOR, spawnX, spawnY, spawnZ, angle, TEMPSUMMON_DEAD_DESPAWN, 0);
		if (twilightCorruptor == nullptr)
			return true;

		twilightCorruptor->MonsterWhisper(TEXT_WHIPSER, pPlayer, true);
	}
	return true;
}

void AddSC_areatrigger_scripts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "at_childrens_week_spot";
    pNewScript->pAreaTrigger = &AreaTrigger_at_childrens_week_spot;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_ravenholdt";
    pNewScript->pAreaTrigger = &AreaTrigger_at_ravenholdt;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_scent_larkorwi";
    pNewScript->pAreaTrigger = &AreaTrigger_at_scent_larkorwi;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "officier_lounges";
	pNewScript->pAreaTrigger = &Areatrigger_OfficerLounges;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "aq_opening_duskwood";
	pNewScript->pAreaTrigger = &Areatrigger_Duskwood;
	pNewScript->RegisterSelf();
}
