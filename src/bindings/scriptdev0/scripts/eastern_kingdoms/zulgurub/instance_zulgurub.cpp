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
SDName: Instance_ZulGurub
SD%Complete: 80
SDComment: Missing reset function after killing a boss for Ohgan, Thekal.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

instance_zulgurub::instance_zulgurub(Map* pMap) : ScriptedInstance(pMap),
    m_bHasIntroYelled(false),
    m_bHasAltarYelled(false)
{
    Initialize();
}

void instance_zulgurub::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_zulgurub::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
		case NPC_VILEBRANCH_SPEAKER: //respawn speaker after server restart (crash) if mandokir hasn't been defeated
			{
				auto mandokirState = GetData(TYPE_MANDOKIR);
				if(mandokirState == NOT_STARTED || mandokirState == FAIL)
				{
					pCreature->SetDeathState(DeathState::DEAD); //hackfix - implement something like ForcedRespawn() instead
					pCreature->Respawn();
				}
			} break;
        case NPC_LORKHAN:
        case NPC_ZATH:
			m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
			break;
        case NPC_THEKAL:
		{
			if (pCreature->isDead())
				pCreature->GetInstanceData()->SetData(TYPE_THEKAL, DONE);

			if (GetData(TYPE_THEKAL) == DONE)
				DoLowerHakkarHitPoints();

			m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
			break;
		}
        case NPC_JINDO:
        case NPC_HAKKAR:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
		case NPC_JEKLIK_FIRE_DUMMY:
			m_lJeklikFireMobGuids.push_back(pCreature->GetObjectGuid());
			return;
		case 14517:
		{
			if (pCreature->isDead())
				pCreature->GetInstanceData()->SetData(TYPE_JEKLIK, DONE);

			if (GetData(TYPE_JEKLIK) == DONE)
				DoLowerHakkarHitPoints();
				break;
		}
		case 14507:
		{
			if (pCreature->isDead())
				pCreature->GetInstanceData()->SetData(TYPE_VENOXIS, DONE);

			if (GetData(TYPE_VENOXIS) == DONE)
				DoLowerHakkarHitPoints();
			break;
		}
		case 14510:
		{
			if (pCreature->isDead())
				pCreature->GetInstanceData()->SetData(TYPE_MARLI, DONE);

			if (GetData(TYPE_MARLI) == DONE)
				DoLowerHakkarHitPoints();
				break;
		}
		case 14515:
		{
			if (pCreature->isDead())
				pCreature->GetInstanceData()->SetData(TYPE_ARLOKK, DONE);

			if (GetData(TYPE_ARLOKK) == DONE)
				DoLowerHakkarHitPoints();
				break;
		}
    }
}

void instance_zulgurub::OnObjectCreate(GameObject* pGo)
{
	switch (pGo->GetEntry())
	{
        case GO_FORCEFIELD:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            return;
		case GO_JEKLIK_FIRE:
			m_lJeklikFireGuids.push_back(pGo->GetObjectGuid());
			return;

	default:
		return;
	}
}

void instance_zulgurub::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_JEKLIK:
			m_auiEncounter[uiType] = uiData;
			if (uiData == DONE || uiData == FAIL)
				DespawnJeklikFire();
			if (uiData == DONE)
				DoLowerHakkarHitPoints();
            break;
        case TYPE_ARLOKK:
            DoUseDoorOrButton(GO_FORCEFIELD);
        case TYPE_VENOXIS:
        case TYPE_MARLI:
        case TYPE_THEKAL:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoLowerHakkarHitPoints();
            break;
		case TYPE_MANDOKIR:
        case TYPE_OHGAN:
        case TYPE_LORKHAN:
        case TYPE_ZATH:
		case TYPE_GAHZRANKA:
        case TYPE_HAKKAR:
		case TYPE_RND_BOSS:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8]  << " "
			<< m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

// Each time High Priest dies lower Hakkar's HP
void instance_zulgurub::DoLowerHakkarHitPoints()
{
    if (Creature* pHakkar = GetSingleCreatureFromStorage(NPC_HAKKAR))
    {
        if (pHakkar->isAlive() && pHakkar->GetMaxHealth() > HP_LOSS_PER_PRIEST)
        {
            pHakkar->SetMaxHealth(pHakkar->GetMaxHealth() - HP_LOSS_PER_PRIEST);
            pHakkar->SetHealth(pHakkar->GetHealth() - HP_LOSS_PER_PRIEST);
         }
     }
}

void instance_zulgurub::HakkarYell(uint32 atEntry)
{
    switch(atEntry)
    {
        case AT_ZULGURUB_ENTRANCE_1:
        case AT_ZULGURUB_ENTRANCE_2:
        {
            if (m_bEnterZulGurubYelled)
                return;

			if (Creature* pHakkar = GetSingleCreatureFromStorage(NPC_HAKKAR))
				DoScriptText(SAY_HAKKAR_MINION_DESTROY, pHakkar);

            m_bEnterZulGurubYelled = true;
            return;
        }    
        case AT_ALTAR_OF_THE_BLOOD_GOD_DOWN:
        case AT_ALTAR_OF_THE_BLOOD_GOD_UP:
        {
            if (m_bProtectAltarYelled)
                return;

			if (Creature* pHakkar = GetSingleCreatureFromStorage(NPC_HAKKAR))
				DoScriptText(SAY_HAKKAR_PROTECT_ALTAR, pHakkar);

            m_bProtectAltarYelled = true;
            return;
        }
        default:
            return;
    }
}

void instance_zulgurub::DespawnJeklikFire()
{
	for (GuidList::const_iterator itr = m_lJeklikFireGuids.begin(); itr != m_lJeklikFireGuids.end(); ++itr)
	{
		if (GameObject* pGoFire= instance->GetGameObject(*itr))
		{
			if (pGoFire->isSpawned())
				pGoFire->RemoveFromWorld();
		}
	}

	for (GuidList::const_iterator itr = m_lJeklikFireMobGuids.begin(); itr != m_lJeklikFireMobGuids.end(); ++itr)
	{
		if (Creature* pJelikFireMob = instance->GetCreature(*itr))
		{
			if (pJelikFireMob)
				pJelikFireMob->ForcedDespawn();
		}
	}
}

void instance_zulgurub::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
		>> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
		>> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_zulgurub::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

InstanceData* GetInstanceData_instance_zulgurub(Map* pMap)
{
    return new instance_zulgurub(pMap);
}

void AddSC_instance_zulgurub()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_zulgurub";
    pNewScript->GetInstanceData = &GetInstanceData_instance_zulgurub;
    pNewScript->RegisterSelf();
}
