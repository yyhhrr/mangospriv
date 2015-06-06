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

#ifndef DEF_ZULFARRAK_H
#define DEF_ZULFARRAK_H

enum
{
    MAX_ENCOUNTER                   = 9,

    TYPE_VELRATHA                   = 0,
    TYPE_GAHZRILLA                  = 1,
    TYPE_ANTUSUL                    = 2,
    TYPE_THEKA                      = 3,
    TYPE_ZUMRAH                     = 4,
    TYPE_NEKRUM                     = 5,
    TYPE_SEZZZIZ                    = 6,
    TYPE_CHIEF_SANDSCALP            = 7,
	TYPE_TREPPE_EVENT				= 8,

    NPC_VELRATHA                    = 7795,
    NPC_GAHZRILLA                   = 7273,
    NPC_ANTUSUL                     = 8127,
    NPC_THEKA                       = 7272,
    NPC_ZUMRAH                      = 7271,
    NPC_NEKRUM                      = 7796,
    NPC_SEZZZIZ                     = 7275,
    NPC_CHIEF_SANDSCALP             = 7267,

    ENTRY_ZUMRAH					= 7271,
    ENTRY_BLY						= 7604,
    ENTRY_RAVEN						= 7605,
    ENTRY_ORO						= 7606,
    ENTRY_WEEGLI					= 7607,
    ENTRY_MURTA						= 7608,

    GO_END_DOOR						= 146084,
    GO_SHALLOW_GRAVE				= 128403,

    EVENT_PYRAMID					= 9,

    AREATRIGGER_ANTUSUL             = 1447,
};

enum zfPyramidPhases
{
    PYRAMID_NOT_STARTED, //default
    PYRAMID_CAGES_OPEN, //happens in GO hello for cages
    PYRAMID_ARRIVED_AT_STAIR , //happens in Weegli's movementinform
    PYRAMID_WAVE_1,
    PYRAMID_PRE_WAVE_2,
    PYRAMID_WAVE_2,
    PYRAMID_PRE_WAVE_3,
    PYRAMID_WAVE_3,
    PYRAMID_KILLED_ALL_TROLLS,
};

class MANGOS_DLL_DECL instance_zulfarrak : public ScriptedInstance
{
    public:
        instance_zulfarrak(Map* pMap);
        ~instance_zulfarrak() {}

        void Initialize();

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void OnCreatureCreate(Creature* pCreature);
		void OnGameObjectCreate(GameObject* pGO);
		void OnObjectCreate(GameObject* pGo);

		void Update(uint32 diff);

		void MoveNPCIfAlive(ScriptedInstance* pInstance, uint32 entry,float x,float y,float z,float o);
		void SpawnPyramidWave(uint32 wave);
		void SendAddsUpStairs(uint32 count);

		void GetShallowGravesGuidList(GuidList& lList) { lList = m_lShallowGravesGuidList; }

		bool IsWaveAllDead();

        void SetData(uint32 uiType, uint32 uiData);
		uint64 GetData64 (uint32 data);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

		std::list<uint64> addsAtBase,movedadds;

        uint64 ZumrahGUID;
        uint64 BlyGUID;
        uint64 WeegliGUID;
        uint64 OroGUID;
        uint64 RavenGUID;
        uint64 MurtaGUID;
        uint64 EndDoorGUID;

        uint32 PyramidPhase;
        uint32 major_wave_Timer;
        uint32 minor_wave_Timer;
        uint32 addGroupSize;
        uint32 waypoint;

		GuidList m_lShallowGravesGuidList;
};

#endif
