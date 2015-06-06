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
SDName: instance_zulfarrak
SD%Complete: 50%
SDComment:
SDCategory: Zul'Farrak
EndScriptData */

#include "precompiled.h"
#include "ScriptMgr.h"
#include "sc_instance.h"
#include "zulfarrak.h"
#include "Player.h"
#include "TemporarySummon.h"

#define PATH_ADDS 81553

int const pyramidSpawnTotal = 54;
/* list of wave spawns: 0 = wave ID, 1 = creature id, 2 = x, 3 = y
no z coordinat b/c they're all the same */
float pyramidSpawns [pyramidSpawnTotal][4] =
{
    {1,7789.0f,1894.64f,1206.29f},
    {1,7787.0f,1890.08f,1218.68f},
    {1,8876.0f,1883.76f,1222.3f},
    {1,7789.0f,1874.18f,1221.24f},
    {1,7787.0f,1892.28f,1225.49f},
    {1,7788.0f,1889.94f,1212.21f},
    {1,7787.0f,1879.02f,1223.06f},
    {1,7789.0f,1874.45f,1204.44f},
    {1,8876.0f,1898.23f,1217.97f},
    {1,7787.0f,1882.07f,1225.7f},
    {1,8877.0f,1896.46f,1205.62f},
    {1,7787.0f,1886.97f,1225.86f},
    {1,7787.0f,1894.72f,1221.91f},
    {1,7787.0f,1883.5f,1218.25f},
    {1,7787.0f,1886.93f,1221.4f},
    {1,8876.0f,1889.82f,1222.51f},
    {1,7788.0f,1893.07f,1215.26f},
    {1,7788.0f,1878.57f,1214.16f},
    {1,7788.0f,1883.74f,1212.35f},
    {1,8877.0f,1877,1207.27f},
    {1,8877.0f,1873.63f,1204.65f},
    {1,8876.0f,1877.4f,1216.41f},
    {1,8877.0f,1899.63f,1202.52f},
    {2,7789.0f,1902.83f,1223.41f},
    {2,8876.0f,1889.82f,1222.51f},
    {2,7787.0f,1883.5f,1218.25f},
    {2,7788.0f,1883.74f,1212.35f},
    {2,8877.0f,1877,1207.27f},
    {2,7787.0f,1890.08f,1218.68f},
    {2,7789.0f,1894.64f,1206.29f},
    {2,8876.0f,1877.4f,1216.41f},
    {2,7787.0f,1892.28f,1225.49f},
    {2,7788.0f,1893.07f,1215.26f},
    {2,8877.0f,1896.46f,1205.62f},
    {2,7789.0f,1874.45f,1204.44f},
    {2,7789.0f,1874.18f,1221.24f},
    {2,7787.0f,1879.02f,1223.06f},
    {2,8876.0f,1898.23f,1217.97f},
    {2,7787.0f,1882.07f,1225.7f},
    {2,8877.0f,1873.63f,1204.65f},
    {2,7787.0f,1886.97f,1225.86f},
    {2,7788.0f,1878.57f,1214.16f},
    {2,7787.0f,1894.72f,1221.91f},
    {2,7787.0f,1886.93f,1221.4f},
    {2,8876.0f,1883.76f,1222.3f},
    {2,7788.0f,1889.94f,1212.21f},
    {2,8877.0f,1899.63f,1202.52f},
    {3,7788.0f,1878.57f,1214.16f},
    {3,7787.0f,1894.72f,1221.91f},
    {3,7787.0f,1886.93f,1221.4f},
    {3,8876.0f,1883.76f,1222.3f},
    {3,7788.0f,1889.94f,1212.21f},
    {3,7275.0f,1889.23f,1207.72f},
    {3,7796.0f,1879.77f,1207.96f}
};

float Spawnsway[2][3] =
{
    {1884.86f,1228.62f,9.0f},
    {1887.53f,1263.0f,41.0f}
};

instance_zulfarrak::instance_zulfarrak(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_zulfarrak::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    PyramidPhase = 0;
}

void instance_zulfarrak::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ANTUSUL:
            m_mNpcEntryGuidStore[NPC_ANTUSUL] = pCreature->GetObjectGuid();
            break;
                case ENTRY_ZUMRAH:
                    ZumrahGUID = pCreature->GetGUID();
                    break;
                case ENTRY_BLY:
                    BlyGUID = pCreature->GetGUID();
                    pCreature->SetReactState(REACT_PASSIVE); // starts out passive (in a cage)
                    break;
                case ENTRY_RAVEN:
                    RavenGUID = pCreature->GetGUID();
                    pCreature->SetReactState(REACT_PASSIVE);// starts out passive (in a cage)
                    break;
                case ENTRY_ORO:
                    OroGUID = pCreature->GetGUID();
                    pCreature->SetReactState(REACT_PASSIVE);// starts out passive (in a cage)
                    break;
                case ENTRY_WEEGLI:
                    WeegliGUID = pCreature->GetGUID();
                    pCreature->SetReactState(REACT_PASSIVE);// starts out passive (in a cage)
                    break;
                case ENTRY_MURTA:
                    MurtaGUID = pCreature->GetGUID();
                    pCreature->SetReactState(REACT_PASSIVE);// starts out passive (in a cage)
                    break;
    }
}

uint32 instance_zulfarrak::GetData(uint32 type)
{
    switch(type)
    {
        case EVENT_PYRAMID:
            return PyramidPhase;
    }
    return 0;
}

uint64 instance_zulfarrak::GetData64(uint32 data)
{
    switch(data)
    {
        case ENTRY_ZUMRAH:
            return ZumrahGUID;
        case ENTRY_BLY:
            return BlyGUID;
        case ENTRY_RAVEN:
            return RavenGUID;
        case ENTRY_ORO:
            return OroGUID;
        case ENTRY_WEEGLI:
            return WeegliGUID;
        case ENTRY_MURTA:
            return MurtaGUID;
        case GO_END_DOOR:
            return EndDoorGUID;
    }
    return 0;
}

void instance_zulfarrak::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_SHALLOW_GRAVE:
            m_lShallowGravesGuidList.push_back(pGo->GetObjectGuid());
            break;
        case GO_END_DOOR:
            EndDoorGUID = pGo->GetGUID();
            if (GetData(TYPE_SEZZZIZ) == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
    }
}

void instance_zulfarrak::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_VELRATHA:
        case TYPE_GAHZRILLA:
        case TYPE_ANTUSUL:
        case TYPE_THEKA:
        case TYPE_ZUMRAH:
        case TYPE_NEKRUM:
        case TYPE_SEZZZIZ:
        case TYPE_CHIEF_SANDSCALP:
		case TYPE_TREPPE_EVENT:
            m_auiEncounter[uiType] = uiData;
            break;
        case EVENT_PYRAMID:
            PyramidPhase = uiData;
            return;
        default:
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3]
                   << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " " << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_zulfarrak::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
               >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}
/*
uint32 instance_zulfarrak::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}*/

void instance_zulfarrak::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_VELRATHA: SetData(TYPE_VELRATHA, IN_PROGRESS); break;
        case NPC_GAHZRILLA: SetData(TYPE_GAHZRILLA, IN_PROGRESS); break;
        case NPC_ANTUSUL: SetData(TYPE_ANTUSUL, IN_PROGRESS); break;
        case NPC_THEKA: SetData(TYPE_THEKA, IN_PROGRESS); break;
        case NPC_ZUMRAH: SetData(TYPE_ZUMRAH, IN_PROGRESS); break;
        case NPC_NEKRUM: SetData(TYPE_NEKRUM, IN_PROGRESS); break;
        case NPC_SEZZZIZ: SetData(TYPE_SEZZZIZ, IN_PROGRESS); break;
        case NPC_CHIEF_SANDSCALP: SetData(TYPE_CHIEF_SANDSCALP, IN_PROGRESS); break;
    }
}

void instance_zulfarrak::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_VELRATHA: SetData(TYPE_VELRATHA, FAIL); break;
        case NPC_GAHZRILLA: SetData(TYPE_GAHZRILLA, FAIL); break;
        case NPC_ANTUSUL: SetData(TYPE_ANTUSUL, FAIL); break;
        case NPC_THEKA: SetData(TYPE_THEKA, FAIL); break;
        case NPC_ZUMRAH: SetData(TYPE_ZUMRAH, FAIL); break;
        case NPC_NEKRUM: SetData(TYPE_NEKRUM, FAIL); break;
        case NPC_SEZZZIZ: SetData(TYPE_SEZZZIZ, FAIL); break;
        case NPC_CHIEF_SANDSCALP: SetData(TYPE_CHIEF_SANDSCALP, FAIL); break;
    }
}

void instance_zulfarrak::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_VELRATHA: SetData(TYPE_VELRATHA, DONE); break;
        case NPC_GAHZRILLA: SetData(TYPE_GAHZRILLA, DONE); break;
        case NPC_ANTUSUL: SetData(TYPE_ANTUSUL, DONE); break;
        case NPC_THEKA: SetData(TYPE_THEKA, DONE); break;
        case NPC_ZUMRAH: SetData(TYPE_ZUMRAH, DONE); break;
        case NPC_NEKRUM: SetData(TYPE_NEKRUM, DONE); break;
        case NPC_SEZZZIZ: SetData(TYPE_SEZZZIZ, DONE); break;
        case NPC_CHIEF_SANDSCALP: SetData(TYPE_CHIEF_SANDSCALP, DONE); break;
    }
}

void instance_zulfarrak::Update(uint32 diff)
{
    switch (PyramidPhase)
    {
        case PYRAMID_NOT_STARTED:
        case PYRAMID_KILLED_ALL_TROLLS:
            break;
        case PYRAMID_ARRIVED_AT_STAIR:
            SpawnPyramidWave(1);
            SetData(EVENT_PYRAMID,PYRAMID_WAVE_1);
            major_wave_Timer=120000;
            minor_wave_Timer=0;
            addGroupSize=2;
            break;
        case PYRAMID_WAVE_1:
            if (IsWaveAllDead())
            {
                SetData(EVENT_PYRAMID,PYRAMID_PRE_WAVE_2);
                major_wave_Timer = 10000; //give players a few seconds before wave 2 starts to rebuff
            }
            else
                if (minor_wave_Timer<diff)
                {
                    SendAddsUpStairs(addGroupSize++);
                    minor_wave_Timer=10000;
                }
                else
                    minor_wave_Timer -= diff;
            break;
        case PYRAMID_PRE_WAVE_2:
            if (major_wave_Timer<diff)
            {
                // beginning 2nd wave!
                SpawnPyramidWave(2);
                SetData(EVENT_PYRAMID,PYRAMID_WAVE_2);
                minor_wave_Timer = 0;
                addGroupSize=2;
            }
            else
                major_wave_Timer -= diff;
            break;
        case PYRAMID_WAVE_2:
            if (IsWaveAllDead())
            {
                SpawnPyramidWave(3);
                SetData(EVENT_PYRAMID,PYRAMID_PRE_WAVE_3);
                major_wave_Timer = 5000; //give NPCs time to return to their home spots
            }
            else
                if (minor_wave_Timer<diff)
                {
                    SendAddsUpStairs(addGroupSize++);
                    minor_wave_Timer=10000;
                }
                else
                    minor_wave_Timer -= diff;
            break;
        case PYRAMID_PRE_WAVE_3:
            if (major_wave_Timer<diff)
            {
                // move NPCs to bottom of stair
                MoveNPCIfAlive(this, ENTRY_BLY,1887.92f,1228.179f,9.98f,4.78f);
                MoveNPCIfAlive(this, ENTRY_MURTA,1891.57f,1228.68f,9.69f,4.78f);
                MoveNPCIfAlive(this, ENTRY_ORO,1897.23f,1228.34f,9.43f,4.78f);
                MoveNPCIfAlive(this, ENTRY_RAVEN,1883.68f,1227.95f,9.543f,4.78f);
                MoveNPCIfAlive(this, ENTRY_WEEGLI,1878.02f,1227.65f,9.485f,4.78f);

                SetData(EVENT_PYRAMID,PYRAMID_WAVE_3);
            }
            else
                major_wave_Timer -= diff;
            break;
        case PYRAMID_WAVE_3:
            if (IsWaveAllDead())
            {
                SetData(EVENT_PYRAMID, PYRAMID_KILLED_ALL_TROLLS);
                MoveNPCIfAlive(this, ENTRY_BLY, 1883.82f, 1200.83f, 8.87f, 1.32f);
                MoveNPCIfAlive(this, ENTRY_MURTA, 1891.83f, 1201.45f, 8.87f, 1.32f);
                MoveNPCIfAlive(this, ENTRY_ORO, 1894.50f, 1204.40f, 8.87f, 1.32f);
                MoveNPCIfAlive(this, ENTRY_RAVEN, 1874.11f, 1206.17f, 8.87f, 1.32f);
                MoveNPCIfAlive(this, ENTRY_WEEGLI, 1877.52f, 1199.63f, 8.87f, 1.32f);
            }
            break;
    };
}

void instance_zulfarrak::MoveNPCIfAlive(ScriptedInstance* pInstance, uint32 entry,float x,float y,float z,float)
{
	if (Creature* pNPC = pInstance->instance->GetCreature(pInstance->GetData64(entry)))
    {
        if (pNPC->isAlive())
        {
			pNPC->SetWalk(true, true);
            pNPC->GetMotionMaster()->MovePoint(1, x, y, z);
            CreatureCreatePos pos(pNPC->GetMap(), x, y, z, 0.0f);
            pNPC->SetRespawnCoord(pos);
			pNPC->SetCombatStartPosition(x, y, z);
        }
    }
}

void instance_zulfarrak::SpawnPyramidWave(uint32 wave)
{
    if (Creature* pNPC = instance->GetCreature(GetData64(ENTRY_BLY)))
    {
        for (int i = 0; i < pyramidSpawnTotal; i++)
        {
            if (pyramidSpawns[i][0] == (float)wave)
            {
                Creature* ts = pNPC->SummonCreature(uint32(pyramidSpawns[i][1]), pyramidSpawns[i][2], pyramidSpawns[i][3], 8.87f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000);
                ts->GetMotionMaster()->MoveRandomAroundPoint(pyramidSpawns[i][2], pyramidSpawns[i][3], 8.87f, 4.0f);
                addsAtBase.push_back(ts->GetGUID());
            }
        }
    }
}

bool instance_zulfarrak::IsWaveAllDead()
{
    for(std::list<uint64>::iterator itr = addsAtBase.begin(); itr != addsAtBase.end(); ++itr)
    {
        if (Creature* add = instance->GetCreature((*itr)))
        {
            if (add->isAlive())
                return false;
        }
    }
    for(std::list<uint64>::iterator itr = movedadds.begin(); itr != movedadds.end(); ++itr)
    {
        if (Creature* add = instance->GetCreature(((*itr))))
        {
            if (add->isAlive())
                return false;
        }
    }
    return true;
}

void instance_zulfarrak::SendAddsUpStairs(uint32 count)
{
    //pop a add from list, send him up the stairs...
    for (uint32 addCount = 0; addCount<count && !addsAtBase.empty(); addCount++)
    {
        if (Creature* add = instance->GetCreature(*addsAtBase.begin()))
        {
            if (add->isAlive())
                add->GetMotionMaster()->MovePoint(PATH_ADDS, 1884.99f, 1263.0f, 41.52f);
            movedadds.push_back(add->GetGUID());
        }
        addsAtBase.erase(addsAtBase.begin());
    }
}

InstanceData* GetInstanceData_instance_zulfarrak(Map* pMap)
{
    return new instance_zulfarrak(pMap);
}

void AddSC_instance_zulfarrak()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_zulfarrak";
    pNewScript->GetInstanceData = &GetInstanceData_instance_zulfarrak;
    pNewScript->RegisterSelf();
}
