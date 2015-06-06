/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


//////////////////////////////////////////////////////////////////////////
// ** instance_deadmines.cpp
//  Handles deadmines instance logic.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Header files
#include "precompiled.h"
#include "instance_deadmines.h"


//////////////////////////////////////////////////////////////////////////
// Implementation: class instance_deadmines
instance_deadmines::instance_deadmines(Map* pMap) 
	: ScriptedInstance(pMap)
	, m_uiIronDoorTimer(0)
	, m_uiDoorStep(0)
{
    Initialize();
}

void instance_deadmines::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_deadmines::OnPlayerEnter(Player* pPlayer)
{
    // Respawn the Mysterious chest if one of the players who enter the instance has the quest in his log
    if (pPlayer->GetQuestStatus(QUEST_FORTUNE_AWAITS) == QUEST_STATUS_COMPLETE &&
            !pPlayer->GetQuestRewardStatus(QUEST_FORTUNE_AWAITS))
        DoRespawnGameObject(GO_MYSTERIOUS_CHEST, HOUR);
}

void instance_deadmines::OnCreatureCreate(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_MR_SMITE)
        m_mNpcEntryGuidStore[NPC_MR_SMITE] = pCreature->GetObjectGuid();
}

void instance_deadmines::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_FACTORY_DOOR:
            if (m_auiEncounter[TYPE_RHAHKZOR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);

            break;
        case GO_MAST_ROOM_DOOR:
            if (m_auiEncounter[TYPE_SNEED] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);

            break;
        case GO_FOUNDRY_DOOR:
            if (m_auiEncounter[TYPE_GILNID] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);

            break;
        case GO_IRON_CLAD_DOOR:
            if (m_auiEncounter[TYPE_IRON_CLAD_DOOR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

            break;
        case GO_DEFIAS_CANNON:
        case GO_SMITE_CHEST:
        case GO_MYSTERIOUS_CHEST:
            break;

        default:
            return;
    }

    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_deadmines::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_RHAHKZOR:
			{
				// Save encounter data
				SetData(TYPE_RHAHKZOR, DONE); 

				// Temporary					  x				y			 z			 o
				const float coordinates[] = { -93.671036f, -398.859711f, 58.290090f, 2.650465f,
											  -95.011383f, -401.365784f, 57.638744f, 2.650465f};

				const float targetCoordinates[] = { -185.951569f, -421.325958f, 54.393425f, 4.266438f,
													-182.692825f, -422.883881f, 54.415958f, 4.266438f };
				
				// TODO: Enable respawn of patrol after death

				// Spawn patrol
				Creature *newCreature = pCreature->SummonCreature(NPC_DEFIAS_OVERSEER, coordinates[0], coordinates[1], coordinates[2], coordinates[3], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[0], targetCoordinates[1], targetCoordinates[2]);
				
				newCreature = pCreature->SummonCreature(NPC_DEFIAS_EVOKER, coordinates[4], coordinates[5], coordinates[6], coordinates[7], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[4], targetCoordinates[5], targetCoordinates[6]);
			}
			
			break;

        case NPC_SNEED:
			{
				SetData(TYPE_SNEED, DONE);  

				// Temporary					  x				y			 z			 o
				const float coordinates[] = {	-208.388f, -504.123f, 51.6638f, 2.76514f,		// 
												-213.047f, -504.664f, 50.9039f, 2.62847f };	// 

				const float targetCoordinates[] = {	-288.394f, -524.449f, 49.6165f,
													-292.883f, -524.449f, 49.6165f	};
				int targetIndex = 0;

				// Spawn patrol
				Creature *newCreature = pCreature->SummonCreature(NPC_DEFIAS_OVERSEER, coordinates[0], coordinates[1], coordinates[2], coordinates[3], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[0], targetCoordinates[1], targetCoordinates[2]);

				// Left
				newCreature = pCreature->SummonCreature(NPC_DEFIAS_EVOKER, coordinates[4], coordinates[5], coordinates[6], coordinates[7], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[3], targetCoordinates[4], targetCoordinates[5]);
			} 
			break;

        case NPC_GILNID:
			{
				SetData(TYPE_GILNID, DONE);

				// Temporary					  x				y			 z			 o
				const float coordinates[] = { -235.304489f, -578.590576f, 51.227814f, 6.282550f,		// Middle
											  -236.711929f, -576.317749f, 51.221772f, 6.282550f,		// Left
											  -236.713767f, -580.797729f, 51.221859f, 6.282550f };		// Right
		
				const float targetCoordinates[] = {	// Middle
													-178.145187f, -576.702637f, 19.309032f,

													// Left
													-175.473480f, -574.118225f, 19.311218f,
																						  
													// Right
													-182.127060f, -574.390662f, 19.309032f};
				int targetIndex = 0;

				// Spawn patrol
				// Middle
				Creature *newCreature = pCreature->SummonCreature(NPC_DEFIAS_WIZARD, coordinates[0], coordinates[1], coordinates[2], coordinates[3], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[0], targetCoordinates[1], targetCoordinates[2]);

				// Left
				newCreature = pCreature->SummonCreature(NPC_DEFIAS_TASKMASTER, coordinates[4], coordinates[5], coordinates[6], coordinates[7], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[3], targetCoordinates[4], targetCoordinates[5]);

				// Right
				newCreature = pCreature->SummonCreature(NPC_DEFIAS_TASKMASTER, coordinates[8], coordinates[9], coordinates[10], coordinates[11], TEMPSUMMON_DEAD_DESPAWN, 0);
				newCreature->SetWalk(true);	// This creature will just walk slowly
				newCreature->GetMotionMaster()->Clear();
				newCreature->GetMotionMaster()->MovePoint(1, targetCoordinates[6], targetCoordinates[7], targetCoordinates[8]);
			}
			break;
    }
}

void instance_deadmines::OnCreatureEnterCombat(Creature* pCreature)
{
	// One of the blackguards in front of Mr. Smite
	if (pCreature->GetEntry() == NPC_DEFIAS_BLACKGUARD &&
		pCreature->getVictim())
	{
		if (Creature* pMrSmite = GetSingleCreatureFromStorage(NPC_MR_SMITE))
		{
			if (!pMrSmite->isDead())
			{
				// This will always pull Mr. Smite
				pMrSmite->Attack(pCreature->getVictim(), true);
				pMrSmite->SetWalk(false);
				pMrSmite->GetMotionMaster()->MoveChase(pCreature->getVictim());
			}
		}
	}
}

void instance_deadmines::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_RHAHKZOR:
        {
            if (uiData == DONE)
                DoUseDoorOrButton(GO_FACTORY_DOOR);

            m_auiEncounter[uiType] = uiData;
            break;
        }
        case TYPE_SNEED:
        {
            if (uiData == DONE)
                DoUseDoorOrButton(GO_MAST_ROOM_DOOR);

            m_auiEncounter[uiType] = uiData;
            break;
        }
        case TYPE_GILNID:
        {
            if (uiData == DONE)
                DoUseDoorOrButton(GO_FOUNDRY_DOOR);

            m_auiEncounter[uiType] = uiData;
            break;
        }
        case TYPE_IRON_CLAD_DOOR:
        {
            // delayed door animation to sync with Defias Cannon animation
            if (uiData == DONE)
                m_uiIronDoorTimer = 500;

            m_auiEncounter[uiType] = uiData;
            break;
        }
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
		for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
		{
			saveStream << m_auiEncounter[i];
			if (i < MAX_ENCOUNTER - 1)
			{
				saveStream << " ";
			}

		}

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_deadmines::GetData(uint32 uiType) const
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_deadmines::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
	for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
	{
		loadStream >> m_auiEncounter[i];

		if (m_auiEncounter[i] == IN_PROGRESS)
			m_auiEncounter[i] = NOT_STARTED;
	}

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_deadmines::Update(uint32 uiDiff)
{
    if (m_uiIronDoorTimer)
    {
        if (m_uiIronDoorTimer <= uiDiff)
        {
            switch (m_uiDoorStep)
            {
                case 0:
                    DoUseDoorOrButton(GO_IRON_CLAD_DOOR, 0, true);

					// You there! Check out that noise!
                    if (Creature* pMrSmite = GetSingleCreatureFromStorage(NPC_MR_SMITE))
                        DoScriptText(INST_SAY_ALARM1, pMrSmite);

                    if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_IRON_CLAD_DOOR))
                    {
                        // should be static spawns, fetch the closest ones at the pier
                        if (Creature* pi1 = GetClosestCreatureWithEntry(pDoor, NPC_PIRATE, 40.0f))
                        {
                            pi1->SetWalk(false);
                            pi1->GetMotionMaster()->MovePoint(0, pDoor->GetPositionX(), pDoor->GetPositionY(), pDoor->GetPositionZ());
                        }

                        if (Creature* pi2 = GetClosestCreatureWithEntry(pDoor, NPC_SQUALLSHAPER, 40.0f))
                        {
                            pi2->SetWalk(false);
                            pi2->GetMotionMaster()->MovePoint(0, pDoor->GetPositionX(), pDoor->GetPositionY(), pDoor->GetPositionZ());
                        }
                    }

                    ++m_uiDoorStep;
                    m_uiIronDoorTimer = 15000;
                    break;

                case 1:
					// We're under attack! Avast, ye swabs! Repell the invaders!
                    if (Creature* pMrSmite = GetSingleCreatureFromStorage(NPC_MR_SMITE))
                        DoScriptText(INST_SAY_ALARM2, pMrSmite);

                    m_uiDoorStep = 0;
                    m_uiIronDoorTimer = 0;
                    break;
            }
        }
        else
            m_uiIronDoorTimer -= uiDiff;
    }
}


//////////////////////////////////////////////////////////////////////////
// Global functions: Script registration
InstanceData* GetInstanceData_instance_deadmines(Map* pMap)
{
    return new instance_deadmines(pMap);
}

void AddSC_instance_deadmines()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_deadmines";
    pNewScript->GetInstanceData = &GetInstanceData_instance_deadmines;
    pNewScript->RegisterSelf();
}

//////////////////////////////////////////////////////////////////////////
