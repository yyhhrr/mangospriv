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
SDName: instance_blackrock_spire
SD%Complete: 50
SDComment: To really get this instance working, many encounters will need more love - and also the DB content is surely not yet perfect.
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"
#include "blackrock_spire.h"

enum
{
    AREATRIGGER_ENTER_UBRS      = 2046,
    AREATRIGGER_STADIUM         = 2026
};

/* Areatrigger
1470 Instance Entry
1628 LBRS, between Spiders and Ogres
1946 LBRS, ubrs pre-quest giver (1)
1986 LBRS, ubrs pre-quest giver (2)
1987 LBRS, ubrs pre-quest giver (3)
2026 UBRS, stadium event trigger
2046 UBRS, way to upper
2066 UBRS, The Beast - Exit (to the dark chamber)
2067 UBRS, The Beast - Entry
2068 LBRS, fall out of map
3726 UBRS, entrance to BWL
*/

instance_blackrock_spire::instance_blackrock_spire(Map* pMap) : ScriptedInstance(pMap),
    m_uiEmberseerGUID(0),
    m_uiGythGUID(0),
    m_uiInfiltratorGUID(0),

    m_uiEmberseerInDoorGUID(0),
    m_uiEmberseerCombatDoorGUID(0),
    m_uiEmberseerOutDoorGUID(0),

    m_uiGythEntryDoorGUID(0),
    m_uiGythCombatDoorGUID(0),
    m_uiGythExitDoorGUID(0)
	{
		Initialize();
	}

void instance_blackrock_spire::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiRoomRuneGUID, 0, sizeof(m_auiRoomRuneGUID));
}

void instance_blackrock_spire::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_EMBERSEER_IN:
            if (m_auiEncounter[TYPE_ROOM_EVENT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiEmberseerInDoorGUID = pGo->GetObjectGuid();
            break;

        case GO_DOORS:
            m_uiEmberseerCombatDoorGUID = pGo->GetObjectGuid();
            break;

        case GO_EMBERSEER_OUT:
            if (m_auiEncounter[TYPE_EMBERSEER] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiEmberseerOutDoorGUID = pGo->GetObjectGuid();
            break;

        case GO_GYTH_ENTRY_DOOR:
            if (m_auiEncounter[TYPE_ROOM_EVENT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiGythEntryDoorGUID = pGo->GetObjectGuid();
            break;

        case GO_GYTH_COMBAT_DOOR:
            if (m_auiEncounter[TYPE_ROOM_EVENT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiGythCombatDoorGUID = pGo->GetObjectGuid();
            break;

        case GO_GYTH_EXIT_DOOR:
            if (m_auiEncounter[TYPE_GYTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            m_uiGythExitDoorGUID = pGo->GetObjectGuid();
            break;

        case GO_ROOM_1_RUNE:
			m_auiRoomRuneGUID[0] = pGo->GetObjectGuid();
			break;
        case GO_ROOM_2_RUNE:
			m_auiRoomRuneGUID[1] = pGo->GetObjectGuid();
			break;
        case GO_ROOM_3_RUNE:
			m_auiRoomRuneGUID[2] = pGo->GetObjectGuid();
			break;
        case GO_ROOM_4_RUNE:
			m_auiRoomRuneGUID[3] = pGo->GetObjectGuid();
			break;
		case GO_ROOM_5_RUNE:
			m_auiRoomRuneGUID[4] = pGo->GetObjectGuid();
			break;
        case GO_ROOM_6_RUNE:
			m_auiRoomRuneGUID[5] = pGo->GetObjectGuid();
			break;
        case GO_ROOM_7_RUNE:
			m_auiRoomRuneGUID[6] = pGo->GetObjectGuid();
			break;

        case GO_ROOKERY_EGG:
			m_lRookeryEggGUIDList.push_back(pGo->GetObjectGuid());
			break;

        case GO_EMBERSEER_RUNE_1:
        case GO_EMBERSEER_RUNE_2:
        case GO_EMBERSEER_RUNE_3:
        case GO_EMBERSEER_RUNE_4:
        case GO_EMBERSEER_RUNE_5:
        case GO_EMBERSEER_RUNE_6:
        case GO_EMBERSEER_RUNE_7:
            m_lEmberseerRunesGUIDList.push_back(pGo->GetObjectGuid());
			break;
    }
}

void instance_blackrock_spire::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_PYROGUARD_EMBERSEER:
			m_uiEmberseerGUID = pCreature->GetObjectGuid();
			break;

        case NPC_GYTH:
			m_uiGythGUID = pCreature->GetObjectGuid();
			break;

		case NPC_REND_SCHWARZFAUST:
			m_uiRendGUID = pCreature->GetGUID();
			break;

        case NPC_SCARSHIELD_INFILTRATOR:
			m_uiInfiltratorGUID = pCreature->GetObjectGuid();
			break;

        case NPC_BLACKHAND_SUMMONER:
		case NPC_BLACKHAND_VETERAN:
			m_lRoomEventMobGUIDList.push_back(pCreature->GetObjectGuid());
			break;

        case NPC_BLACKHAND_INCANCERATOR:
			m_lIncanceratorGUIDList.push_back(pCreature->GetObjectGuid());
			break;

        case NPC_DRAKKISATH:
        case NPC_ELITE_GUARD:
            m_lDrakkisathNpcGUIDList.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_blackrock_spire::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ROOM_EVENT:
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiEmberseerInDoorGUID);
            m_auiEncounter[0] = uiData;
            break;

        case TYPE_EMBERSEER:
			if (uiData == IN_PROGRESS)
			{
				if (Creature* pEmberseer = instance->GetCreature(m_uiEmberseerGUID))
				{
					pEmberseer->setFaction(14);
					pEmberseer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					pEmberseer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
					pEmberseer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
					pEmberseer->SetInCombatWithZone();
				}
				DoUseEmberseerRunes();
				DoUseDoorOrButton(m_uiEmberseerCombatDoorGUID);
			}

            if (uiData == FAIL)
            {
				if (Creature* pEmberseer = instance->GetCreature(m_uiEmberseerGUID))
				{
					pEmberseer->setFaction(35);
					pEmberseer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					pEmberseer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
					pEmberseer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
				}
                for (GUIDList::const_iterator itr = m_lIncanceratorGUIDList.begin(); itr != m_lIncanceratorGUIDList.end(); itr++)
                {
                    if (Creature* pCreature = instance->GetCreature(*itr))
                    {
                        if (!pCreature->isAlive())
                            pCreature->Respawn();
							pCreature->setFaction(35);
                    }
                }

				DoUseEmberseerRunes();
				DoUseDoorOrButton(m_uiEmberseerCombatDoorGUID);
            }

            if (uiData == DONE)
			{
				DoUseEmberseerRunes();
				DoUseDoorOrButton(m_uiEmberseerCombatDoorGUID);
                DoUseDoorOrButton(m_uiEmberseerOutDoorGUID);
			}

            m_auiEncounter[1] = uiData;
            break;
        case TYPE_FLAMEWREATH:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_GYTH:
            if (uiData == IN_PROGRESS)
                DoUseDoorOrButton(m_uiGythEntryDoorGUID);
			if (uiData == FAIL)
			{
				if (auto* rend = instance->GetCreature(m_uiRendGUID))
				{
					rend->ForcedDespawn();
					rend->RemoveFromWorld();
				}

				if(auto* gyth = instance->GetCreature(m_uiGythGUID))
				{
					gyth->Respawn();
				}

				DoUseDoorOrButton(m_uiGythEntryDoorGUID);
			}
			if (uiData == DONE)
            {
                DoUseDoorOrButton(m_uiGythEntryDoorGUID);
                DoUseDoorOrButton(m_uiGythExitDoorGUID);
            }
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_VALTHALAK:
            m_auiEncounter[4] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_blackrock_spire::SetData64(uint32 uiType, uint64 uiData)
{
    if (uiType == TYPE_ROOM_EVENT && GetData(TYPE_ROOM_EVENT) == IN_PROGRESS)
    {
        uint8 uiNotEmptyRoomsCount = 0;
        for (uint8 i = 0; i< MAX_ROOMS; i++)
        {
            if (m_auiRoomRuneGUID[i])
            {
                m_alRoomEventMobGUIDSorted[i].remove(uiData);
                if (m_alRoomEventMobGUIDSorted[i].empty())
                {
                    DoUseDoorOrButton(m_auiRoomRuneGUID[i]);
                    m_auiRoomRuneGUID[i] = 0;
                }
                else
                    uiNotEmptyRoomsCount++;
            }
        }
        if (!uiNotEmptyRoomsCount)
            SetData(TYPE_ROOM_EVENT, DONE);
    }
}

void instance_blackrock_spire::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_blackrock_spire::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ROOM_EVENT:
			return m_auiEncounter[0];

        case TYPE_EMBERSEER:
			return m_auiEncounter[1];

        case TYPE_FLAMEWREATH:
			return m_auiEncounter[2];

        case TYPE_GYTH:
			return m_auiEncounter[3];

        case TYPE_VALTHALAK:
			return m_auiEncounter[4];
    }
    return 0;
}

uint64 instance_blackrock_spire::GetData64(uint32 uiType)
{
    switch (uiType)
    {
        case NPC_PYROGUARD_EMBERSEER:
			return m_uiEmberseerGUID;

        case NPC_GYTH:
			return m_uiGythGUID;

        case NPC_SCARSHIELD_INFILTRATOR:
			return m_uiInfiltratorGUID;

        case GO_GYTH_COMBAT_DOOR:
			return m_uiGythCombatDoorGUID;
    }
    return 0;
}

void instance_blackrock_spire::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_BLACKHAND_INCANCERATOR:
            m_blackhand_incanceratorGUIDs.remove(pCreature->GetObjectGuid());
            if (m_blackhand_incanceratorGUIDs.empty())
                SetData(TYPE_EMBERSEER, IN_PROGRESS);
            break;
    }
}

void instance_blackrock_spire::OnCreatureEnterCombat(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_BLACKHAND_INCANCERATOR:
			break;

        case NPC_DRAKKISATH:
        case NPC_ELITE_GUARD:
            for (GUIDList::const_iterator itr = m_lDrakkisathNpcGUIDList.begin(); itr != m_lDrakkisathNpcGUIDList.end(); itr++)
            {
                if (Creature* pTemp = instance->GetCreature(*itr))
                {
                    if (pCreature->getVictim())
                        pTemp->AI()->AttackStart(pCreature->getVictim());
                }
            }
            break;
    }
}

void instance_blackrock_spire::DoSortRoomEventMobs()
{
    if (GetData(TYPE_ROOM_EVENT) != NOT_STARTED)
        return;
    for (uint8 i = 0; i < MAX_ROOMS; i++)
        if (GameObject* pRune = instance->GetGameObject(m_auiRoomRuneGUID[i]))
            for (GUIDList::const_iterator itr = m_lRoomEventMobGUIDList.begin(); itr != m_lRoomEventMobGUIDList.end(); itr++)
                if (Creature* pCreature = instance->GetCreature(*itr))
                    if (pCreature->isAlive() && pCreature->GetDistance(pRune) < 10.0f)
                        m_alRoomEventMobGUIDSorted[i].push_back(*itr);

    SetData(TYPE_ROOM_EVENT, IN_PROGRESS);
}

void instance_blackrock_spire::ProcessEmberseerEvent()
{
    if (GetData(TYPE_EMBERSEER) == DONE || GetData(TYPE_EMBERSEER) == IN_PROGRESS)
        return;

    if (!m_lIncanceratorGUIDList.empty())
    {
        for (GUIDList::const_iterator itr = m_lIncanceratorGUIDList.begin(); itr != m_lIncanceratorGUIDList.end(); itr++)
        {
            if (Creature* pCaster = instance->GetCreature(*itr))
            {
                if (pCaster->isAlive())
                {
                    pCaster->CastStop();
                    pCaster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    pCaster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
					pCaster->setFaction(14);
					pCaster->SetInCombatWithZone();
                }
            }
        }
    }
}

void instance_blackrock_spire::DoUseEmberseerRunes()
{
    if (m_lEmberseerRunesGUIDList.empty())
        return;

    for (GUIDList::const_iterator itr = m_lEmberseerRunesGUIDList.begin(); itr != m_lEmberseerRunesGUIDList.end(); itr++)
        DoUseDoorOrButton(*itr);
}

InstanceData* GetInstanceData_instance_blackrock_spire(Map* pMap)
{
    return new instance_blackrock_spire(pMap);
}

bool AreaTrigger_at_blackrock_spire(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isDead())
        return false;

    switch (pAt->id)
    {
        case AREATRIGGER_ENTER_UBRS:
            if (instance_blackrock_spire* pInstance = (instance_blackrock_spire*) pPlayer->GetInstanceData())
                pInstance->DoSortRoomEventMobs();
            break;

        case AREATRIGGER_STADIUM:
            if (instance_blackrock_spire* pInstance = (instance_blackrock_spire*) pPlayer->GetInstanceData())
                if (Creature* pGyth = pInstance->instance->GetCreature(pInstance->GetData64(NPC_GYTH)))
                    if (pGyth->isAlive() && !pGyth->isInCombat())
                        pGyth->AI()->AttackStart(pPlayer);
            break;
    }
    return false;
}

bool GOHello_schwarzfelsaltar(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        Player* p;
        p=pInstance->instance->GetPlayer(pInstance->GetData64(DATA_PLAYER));
        if (!p || p->isDead())
        {
            pInstance->SetData64(DATA_PLAYER,pPlayer->GetGUID());
        }
        if (pInstance->GetData(TYPE_EMBERSEER) == IN_PROGRESS || pInstance->GetData(TYPE_EMBERSEER) == DONE)
        {
            return false;
        }
        else
        {
			if (instance_blackrock_spire* pInstance = (instance_blackrock_spire*)((Player*)pGo)->GetInstanceData())
			{
				pInstance->ProcessEmberseerEvent();
				return true;
			}
            return false;
        }
    }
    return false;
}

void AddSC_instance_blackrock_spire()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_blackrock_spire";
    pNewScript->GetInstanceData = &GetInstanceData_instance_blackrock_spire;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_blackrock_spire";
    pNewScript->pAreaTrigger = &AreaTrigger_at_blackrock_spire;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "schwarzfelsaltar";
    pNewScript->pGOUse = &GOHello_schwarzfelsaltar;
    pNewScript->RegisterSelf();
}