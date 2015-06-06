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
SDName: Instance_Ruins_of_Ahnqiraj
SD%Complete: 80
SDComment:
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

instance_ruins_of_ahnqiraj::instance_ruins_of_ahnqiraj(Map* pMap) : ScriptedInstance(pMap), m_hasResetRajaxx(false), m_uiWipeCheckTimer(10000)
{
    Initialize();
}

void instance_ruins_of_ahnqiraj::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    // Test-Server only
    for (int i = 0; i < MAX_ENCOUNTER; i++)
        m_auiEncounter[i] = DONE;
    SetData(TYPE_BURU, NOT_STARTED);
    SetData(TYPE_MOAM, NOT_STARTED);
}

void instance_ruins_of_ahnqiraj::OnPlayerEnter(Player*)
{
    // Spawn andorov if necessary
    if (m_auiEncounter[TYPE_KURINNAXX] == DONE)
        DoSapwnAndorovIfCan();
}


bool instance_ruins_of_ahnqiraj::IsEncounterInProgress() const
{
    return m_auiEncounter[TYPE_RAJAXX] == IN_PROGRESS;
}

void instance_ruins_of_ahnqiraj::OnCreatureRespawn(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
    case NPC_COLONEL_ZERRAN:
    case NPC_MAJOR_PAKKON:
    case NPC_MAJOR_YEGGETH:
    case NPC_CAPTAIN_XURREM:
    case NPC_CAPTAIN_DRENN:
    case NPC_CAPTAIN_TUUBID:
    case NPC_CAPTAIN_QEEZ:
    case NPC_QIRAJI_WARRIOR:
    case NPC_SWARMGUARD_NEEDLER:
        // prevent loot farming
        pCreature->lootForBody = true;
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureCreate(Creature* pCreature)
{
	if (!pCreature->GetResilience())
		pCreature->SetResilience(60);

    pCreature->m_aggroRadius = 1.2f;

    switch(pCreature->GetEntry())
    {
        case NPC_KURINNAXX:
            pCreature->m_aggroRadius = 3.f;
            break;
        case NPC_OSSIRIAN_CRYSTAL_TRIGGER:
            // Only store static spawned
            if (pCreature->IsTemporarySummon())
                break;
        case NPC_BURU:
        case NPC_OSSIRIAN:
        case NPC_RAJAXX:
        case NPC_GENERAL_ANDOROV:
        case NPC_COLONEL_ZERRAN:
        case NPC_MAJOR_PAKKON:
        case NPC_MAJOR_YEGGETH:
        case NPC_CAPTAIN_XURREM:
        case NPC_CAPTAIN_DRENN:
        case NPC_CAPTAIN_TUUBID:
        case NPC_CAPTAIN_QEEZ:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
        default:
            switch (pCreature->GetAreaId())
            {
            case 3429:
            case 3448:
                if (GetData(TYPE_KURINNAXX) == DONE && pCreature->GetEntry() != NPC_GENERAL_ANDOROV)
                {
                    pCreature->SetRespawnDelay(3 * DAY);
                    pCreature->SetRespawnTime(MONTH);
                    pCreature->ForcedDespawn();
                    pCreature->SetVisibility(VISIBILITY_OFF);
                }
                break;
            case 3449:
                if (GetData(TYPE_RAJAXX) == DONE)
                {
                    pCreature->SetRespawnDelay(3 * DAY);
                    pCreature->SetRespawnTime(MONTH);
                    pCreature->ForcedDespawn();
                    pCreature->SetVisibility(VISIBILITY_OFF);
                }
                break;
            case 3450:
                if (GetData(TYPE_MOAM) == DONE)
                {
                    pCreature->SetRespawnDelay(3 * DAY);
                    pCreature->SetRespawnTime(MONTH);
                    pCreature->ForcedDespawn();
                    pCreature->SetVisibility(VISIBILITY_OFF);
                }
                break;
            case 3451:
                if (GetData(TYPE_BURU) == DONE)
                {
                    pCreature->SetRespawnDelay(3 * DAY);
                    pCreature->SetRespawnTime(MONTH);
                    pCreature->ForcedDespawn();
                    pCreature->SetVisibility(VISIBILITY_OFF);
                }
                break;
            case 3452:
                if (GetData(TYPE_AYAMISS) == DONE)
                {
                    pCreature->SetRespawnDelay(3 * DAY);
                    pCreature->SetRespawnTime(MONTH);
                    pCreature->ForcedDespawn();
                    pCreature->SetVisibility(VISIBILITY_OFF);
                }
                break;
            case 3453:
                if (GetData(TYPE_OSSIRIAN) == DONE)
                {
                    pCreature->SetRespawnDelay(3 * DAY);
                    pCreature->SetRespawnTime(MONTH);
                    pCreature->ForcedDespawn();
                    pCreature->SetVisibility(VISIBILITY_OFF);
                }
                break;
            }
            break;
        case NPC_KALDOREI_ELITE:
            m_lKaldoreiGuidList.push_back(pCreature->GetObjectGuid());
            return;
    }
}

void instance_ruins_of_ahnqiraj::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_OSSIRIAN_CRYSTAL:
            // Make them unusable temporarily
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            break;
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KURINNAXX: SetData(TYPE_KURINNAXX, IN_PROGRESS); break;
        //case NPC_RAJAXX:    SetData(TYPE_RAJAXX, IN_PROGRESS);    break;
        case NPC_MOAM:      SetData(TYPE_MOAM, IN_PROGRESS);      break;
        case NPC_BURU:      SetData(TYPE_BURU, IN_PROGRESS);      break;
        case NPC_AYAMISS:   SetData(TYPE_AYAMISS, IN_PROGRESS);   break;
        case NPC_OSSIRIAN:  SetData(TYPE_OSSIRIAN, IN_PROGRESS);  break;
        case NPC_COLONEL_ZERRAN:
        case NPC_MAJOR_PAKKON:
        case NPC_MAJOR_YEGGETH:
        case NPC_CAPTAIN_XURREM:
        case NPC_CAPTAIN_DRENN:
        case NPC_CAPTAIN_TUUBID:
        case NPC_CAPTAIN_QEEZ:
            if (GetData(TYPE_RAJAXX) == FAIL || GetData(TYPE_RAJAXX) == NOT_STARTED)
            {
                m_uiCurrentArmyWave = 0;
                DoSendNextArmyWave();
                SetData(TYPE_RAJAXX, IN_PROGRESS);
            }
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KURINNAXX: SetData(TYPE_KURINNAXX, FAIL); break;
        case NPC_MOAM:      SetData(TYPE_MOAM, FAIL);      break;
        case NPC_BURU:      SetData(TYPE_BURU, FAIL);      break;
        case NPC_AYAMISS:   SetData(TYPE_AYAMISS, FAIL);   break;
        case NPC_OSSIRIAN:  SetData(TYPE_OSSIRIAN, FAIL);  break;
        case NPC_RAJAXX:
            // Rajaxx yells on evade
            DoScriptText(SAY_DEAGGRO, pCreature);
            // no break;
        case NPC_COLONEL_ZERRAN:
        case NPC_MAJOR_PAKKON:
        case NPC_MAJOR_YEGGETH:
        case NPC_CAPTAIN_XURREM:
        case NPC_CAPTAIN_DRENN:
        case NPC_CAPTAIN_TUUBID:
        case NPC_CAPTAIN_QEEZ:
            pCreature->SetActiveObjectState(false);
            break;
    }
}

void instance_ruins_of_ahnqiraj::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_KURINNAXX: SetData(TYPE_KURINNAXX, DONE); break;
        case NPC_RAJAXX:    SetData(TYPE_RAJAXX, DONE);    break;
        case NPC_MOAM:      SetData(TYPE_MOAM, DONE);      break;
        case NPC_BURU:      SetData(TYPE_BURU, DONE);      break;
        case NPC_AYAMISS:   SetData(TYPE_AYAMISS, DONE);   break;
        case NPC_OSSIRIAN:  SetData(TYPE_OSSIRIAN, DONE);  break;
        case NPC_GENERAL_ANDOROV: SetData(TYPE_ANDOROV, FAIL); break;
        case NPC_COLONEL_ZERRAN:
        case NPC_MAJOR_PAKKON:
        case NPC_MAJOR_YEGGETH:
        case NPC_CAPTAIN_XURREM:
        case NPC_CAPTAIN_DRENN:
        case NPC_CAPTAIN_TUUBID:
        case NPC_CAPTAIN_QEEZ:
        case NPC_QIRAJI_WARRIOR:
        case NPC_SWARMGUARD_NEEDLER:
        {
            // If event isn't started by Andorov, return
            if (GetData(TYPE_RAJAXX) != IN_PROGRESS)
                return;

            Map::PlayerList const &PlayerList = instance->GetPlayers();
            for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            {
                if (itr->getSource() && itr->getSource()->IsInWorld())
                    itr->getSource()->SetCombatTimer(60 * IN_MILLISECONDS);
            }

            // Check if the dead creature belongs to the current wave
            if (m_sArmyWavesGuids[m_uiCurrentArmyWave - 1].find(pCreature->GetObjectGuid()) != m_sArmyWavesGuids[m_uiCurrentArmyWave - 1].end())
            {
                m_sArmyWavesGuids[m_uiCurrentArmyWave - 1].erase(pCreature->GetObjectGuid());
                // If all the soldiers from the current wave are dead, then send the next one
                if (m_sArmyWavesGuids[m_uiCurrentArmyWave - 1].empty())
                    DoSendNextArmyWave();
            }
            break;
        }
    }
}

void instance_ruins_of_ahnqiraj::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_KURINNAXX:
            if (uiData == DONE)
            {
                DoSapwnAndorovIfCan();

                // Yell after kurinnaxx
                if (m_auiEncounter[uiType] != DONE)
				    DoOrSimulateScriptTextForThisInstance(YELL_OSSIRIAN_BREACHED, NPC_OSSIRIAN);
            }
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_RAJAXX:
            m_auiEncounter[uiType] = uiData;
            if (uiData == IN_PROGRESS)
            {
                m_hasResetRajaxx = false;
                DoSortArmyWaves();
            }
            if (uiData == DONE)
            {
                Map::PlayerList const &PlayerList = instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                {
                    if (itr->getSource() && itr->getSource()->IsInWorld())
                        itr->getSource()->SetCombatTimer(0);
                }

                if (Creature* pAndorov = GetSingleCreatureFromStorage(NPC_GENERAL_ANDOROV))
                {
                    if (pAndorov->isAlive() && m_auiEncounter[TYPE_ANDOROV] != FAIL)
                    {
#if AQ_PATCH >= 10
                        pAndorov->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
#else
                        pAndorov->ForcedDespawn();
#endif
                        Map::PlayerList const& lPlayers = this->instance->GetPlayers();
                        
                        if (!lPlayers.isEmpty())
                            for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                            {
                                if (Player* pPlayer = itr->getSource())
                                    pPlayer->RewardReputation(609, 150);
                            }
                    }
                }
            }
            break;
        case TYPE_MOAM:
        case TYPE_BURU:
        case TYPE_AYAMISS:
        case TYPE_OSSIRIAN:
        case TYPE_ANDOROV:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE || (uiType == TYPE_RAJAXX && uiData == IN_PROGRESS) || (uiType == TYPE_ANDOROV && uiData == FAIL))
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2]
            << " " << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5]
            << " " << m_auiEncounter[6];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_ruins_of_ahnqiraj::GetData(uint32 uiType)
{
   if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_ruins_of_ahnqiraj::DoSapwnAndorovIfCan()
{
    // The npc is also a vendor so always spawn after kurinnaxx
    if (m_auiEncounter[TYPE_KURINNAXX] != DONE || m_auiEncounter[TYPE_ANDOROV] == FAIL)
        return;

    if (GetSingleCreatureFromStorage(NPC_GENERAL_ANDOROV))
        return;

    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    for (uint8 i = 0; i < MAX_HELPERS; i++)
    {
        Creature* spawn = pPlayer->SummonCreature(aAndorovSpawnLocs[i].m_uiEntry, aAndorovSpawnLocs[i].m_fX, aAndorovSpawnLocs[i].m_fY, aAndorovSpawnLocs[i].m_fZ, aAndorovSpawnLocs[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (spawn)
        {
            spawn->SetActiveObjectState(true);
            spawn->SetPvP(true);
        }
    }
}

void instance_ruins_of_ahnqiraj::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);

    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2]
               >> m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5]
               >> m_auiEncounter[6];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}
void instance_ruins_of_ahnqiraj::Update(uint32 uiDiff)
{
    if (GetData(TYPE_RAJAXX) == IN_PROGRESS)
    {
        if (m_uiWipeCheckTimer <= uiDiff)
            CheckRajaxxWipe();
        else
            m_uiWipeCheckTimer -= uiDiff;
        if (m_uiArmyDelayTimer)
        {
            if (m_uiArmyDelayTimer <= uiDiff)
                DoSendNextArmyWave();
            else
                m_uiArmyDelayTimer -= uiDiff;
        }
    }
}
void instance_ruins_of_ahnqiraj::DoSortArmyWaves()
{
    std::list<Creature*> lCreatureList;
    // Sort the 7 army waves
    // We need to use gridsearcher for this, because coords search is too complicated here
    for (uint8 i = 0; i < MAX_ARMY_WAVES; ++i)
    {
        // Clear all the army waves
        m_constArmyWavesGuids[i].clear();
        lCreatureList.clear();
        if (Creature* pTemp = GetSingleCreatureFromStorage(aArmySortingParameters[i].m_uiEntry))
        {
            GetCreatureListWithEntryInGrid(lCreatureList, pTemp, NPC_QIRAJI_WARRIOR, aArmySortingParameters[i].m_fSearchDist);
            GetCreatureListWithEntryInGrid(lCreatureList, pTemp, NPC_SWARMGUARD_NEEDLER, aArmySortingParameters[i].m_fSearchDist);
            for (std::list<Creature*>::const_iterator itr = lCreatureList.begin(); itr != lCreatureList.end(); ++itr)
                m_constArmyWavesGuids[i].insert((*itr)->GetObjectGuid());

            m_constArmyWavesGuids[i].insert(pTemp->GetObjectGuid());
        }
        m_sArmyWavesGuids[i] = m_constArmyWavesGuids[i];
    }
    // send the first wave
    m_uiCurrentArmyWave = 0;
    DoSendNextArmyWave();
}

void instance_ruins_of_ahnqiraj::CheckRajaxxWipe()
{
    m_uiWipeCheckTimer = 10000;
    bool stillAlive = false;

    Creature* pAndorov = GetSingleCreatureFromStorage(NPC_GENERAL_ANDOROV);

    if (pAndorov && pAndorov->isAlive())
        stillAlive = true;
    else
    {
        Map::PlayerList const &PlayerList = instance->GetPlayers();
        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
            {
                if (pPlayer && pPlayer->isAlive() && pPlayer->isTargetableForAttack())
                {
                    stillAlive = true;
                    break;
                }
            }
        }
    }

    if (!stillAlive)
    {
        SetData(TYPE_RAJAXX, FAIL);
        ResetRajaxx();
        return;
    }

    for (uint8 i = 0; i < m_uiCurrentArmyWave; ++i)
    {
        for (ObjectGuidSet::const_iterator itr = m_sArmyWavesGuids[i].begin(); itr != m_sArmyWavesGuids[i].end(); ++itr)
        {
            if (Creature* pTemp = instance->GetCreature(*itr))
            {
                if (pAndorov && pAndorov->isAlive() && !pAndorov->isInCombat())
                    pAndorov->AI()->AttackStart(pTemp);
                if (!pTemp->isInCombat())
                    pTemp->SetInCombatWithZone();
            }
        }
    }
}

void instance_ruins_of_ahnqiraj::ResetRajaxx()
{
    if (m_hasResetRajaxx)
        return;

    m_hasResetRajaxx = true;

    std::list<Creature*> lCreatureList;
    for (uint8 i = 0; i < MAX_ARMY_WAVES; ++i)
    {
        for (ObjectGuidSet::const_iterator itr = m_constArmyWavesGuids[i].begin(); itr != m_constArmyWavesGuids[i].end(); ++itr)
        {
            if (Creature *pCreature = instance->GetCreature(*itr))
                if (!pCreature->isAlive())
                {
                    pCreature->RemoveCorpse();
                    pCreature->Respawn();
                }
                else
                    pCreature->Evade();
        }

        m_sArmyWavesGuids[i] = m_constArmyWavesGuids[i];
    }

    for (ObjectGuidSet::const_iterator itr = m_sArmyWavesGuids[6].begin(); itr != m_sArmyWavesGuids[6].end(); ++itr)
    {
        if (Creature* pTemp = instance->GetCreature(*itr))
            pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
    }

    if (Creature* pRajaxx = GetSingleCreatureFromStorage(NPC_RAJAXX))
        pRajaxx->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

    m_uiCurrentArmyWave = 0;
}

void instance_ruins_of_ahnqiraj::DoSendNextArmyWave()
{
    // The next army wave is sent into battle after 2 min or after the previous wave is finished
    if (GetData(TYPE_RAJAXX) != IN_PROGRESS)
        return;

    // The last wave is General Rajaxx itself
    if (m_uiCurrentArmyWave == MAX_ARMY_WAVES)
    {
        if (Creature* pRajaxx = GetSingleCreatureFromStorage(NPC_RAJAXX))
        {
            pRajaxx->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
            DoScriptText(SAY_INTRO, pRajaxx);
            pRajaxx->SetInCombatWithZone();
        }
    }
    else
    {
        // Increase the wave id if some are already dead
        while (m_sArmyWavesGuids[m_uiCurrentArmyWave].empty())
        {
            ++m_uiCurrentArmyWave;
            if (m_uiCurrentArmyWave == MAX_ARMY_WAVES)
                DoSendNextArmyWave();
        }
        if (m_uiCurrentArmyWave >= MAX_ARMY_WAVES)
            return;
        float fX, fY, fZ;

        for (ObjectGuidSet::const_iterator itr = m_sArmyWavesGuids[m_uiCurrentArmyWave].begin(); itr != m_sArmyWavesGuids[m_uiCurrentArmyWave].end(); ++itr)
        {
            if (Creature* pTemp = instance->GetCreature(*itr))
            {
                if (!pTemp->isAlive())
                {
                    m_sArmyWavesGuids[m_uiCurrentArmyWave].erase(itr);
                    itr = m_sArmyWavesGuids[m_uiCurrentArmyWave].begin();
                    continue;
                }
                switch (pTemp->GetEntry())
                {
                    case NPC_COLONEL_ZERRAN:
                    case NPC_MAJOR_PAKKON:
                    case NPC_MAJOR_YEGGETH:
                    case NPC_CAPTAIN_XURREM:
                    case NPC_CAPTAIN_DRENN:
                    case NPC_CAPTAIN_TUUBID:
                    case NPC_CAPTAIN_QEEZ:
                        pTemp->SetActiveObjectState(true);
                    break;
                }
                pTemp->SetWalk(false);
                pTemp->GetRandomPoint(aAndorovMoveLocs[4].m_fX, aAndorovMoveLocs[4].m_fY, aAndorovMoveLocs[4].m_fZ, 10.0f, fX, fY, fZ);
                pTemp->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
            }
        }
        // Yell on each wave (except the first 2)
        if (aArmySortingParameters[m_uiCurrentArmyWave].m_uiYellEntry)
        {
            if (Creature* pRajaxx = GetSingleCreatureFromStorage(NPC_RAJAXX))
                DoScriptText(aArmySortingParameters[m_uiCurrentArmyWave].m_uiYellEntry, pRajaxx);
        }
    }
#if AQ_PATCH < 20
    m_uiArmyDelayTimer = 3 * MINUTE * IN_MILLISECONDS;
#else
    m_uiArmyDelayTimer = 2 * MINUTE * IN_MILLISECONDS;
#endif
    ++m_uiCurrentArmyWave;
    m_uiWipeCheckTimer = 30 * IN_MILLISECONDS;
}

InstanceData* GetInstanceData_instance_ruins_of_ahnqiraj(Map* pMap)
{
    return new instance_ruins_of_ahnqiraj(pMap);
}

void AddSC_instance_ruins_of_ahnqiraj()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_ruins_of_ahnqiraj";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ruins_of_ahnqiraj;
    pNewScript->RegisterSelf();
}
