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

#ifndef DEF_RUINS_OF_AHNQIRAJ_H
#define DEF_RUINS_OF_AHNQIRAJ_H
//  0 - 1.9
// 10 - 1.10
// 15 - 1.10.1
// 20 - 1.11
// 30 - 1.12
#define AQ_PATCH 0

enum
{
    MAX_ENCOUNTER               = 7,
    MAX_HELPERS                 = 5,

    TYPE_KURINNAXX              = 0,
    TYPE_RAJAXX                 = 1,
    TYPE_MOAM                   = 2,
    TYPE_BURU                   = 3,
    TYPE_AYAMISS                = 4,
    TYPE_OSSIRIAN               = 5,
    TYPE_ANDOROV                = 6,

    MAX_ARMY_WAVES              = 7,
};

enum Creatures
{
    NPC_KURINNAXX                 = 15348,
    NPC_RAJAXX                    = 15341,
    NPC_GENERAL_ANDOROV           = 15471,
    NPC_COLONEL_ZERRAN            = 15385,
    NPC_MAJOR_PAKKON              = 15388,
    NPC_MAJOR_YEGGETH             = 15386,
    NPC_CAPTAIN_XURREM            = 15390,
    NPC_CAPTAIN_DRENN             = 15389,
    NPC_CAPTAIN_TUUBID            = 15392,
    NPC_CAPTAIN_QEEZ              = 15391,
    NPC_KALDOREI_ELITE            = 15473,
    NPC_MOAM                      = 15340,
    NPC_BURU                      = 15370,
    NPC_AYAMISS                   = 15369,
    NPC_OSSIRIAN                  = 15339,
    
    // Trash and Adds
    NPC_SWARMGUARD_NEEDLER        = 15344,
    NPC_QIRAJI_WARRIOR            = 15387,
    NPC_MANA_FIEND                = 15527,
    NPC_HIVEZARA_WASP             = 15325,
    NPC_HIVEZARA_SWARMER          = 15546,
    NPC_HIVEZARA_LARVA            = 15555,
    NPC_ANUB_WARRIOR              = 15537,
    NPC_ANUB_SWARM                = 15538,
    NPC_AHNQIRAJ_TRIGGER          = 15426,
    NPC_BURU_EGG                  = 15514,
    NPC_BURU_EGG_TRIGGER          = 15964,
    NPC_HATCHLING                 = 15521,
    NPC_HIVEZARA_HATCHLING        = 15521,
    NPC_OSSIRIAN_CRYSTAL_TRIGGER  = 15590,
    NPC_TORNADO                   = 19922,
};
// Rajaxx yells
enum RajaxxYells
{
    SAY_WAVE3                     = -1509005,
    SAY_WAVE4                     = -1509006,
    SAY_WAVE5                     = -1509007,
    SAY_WAVE6                     = -1509008,
    SAY_WAVE7                     = -1509009,
    SAY_INTRO                     = -1509010,
    SAY_DEAGGRO                   = -1509015, // on Rajaxx evade
    SAY_COMPLETE_QUEST            = -1509017, // Yell when realm complete quest 8743 for world event
};

enum GameObjects
{
    GO_SAND_TRAP                  = 180647,
    GO_OSSIRIAN_CRYSTAL           = 180619,
};

enum Misc
{
    AREAID_WATCHERS_TERRACE       = 3453,
    FACTION_CENARION_CIRCLE       = 609,
    YELL_OSSIRIAN_BREACHED        = -1509022,
};

struct SpawnLocation
{
    uint32 m_uiEntry;
    float m_fX, m_fY, m_fZ, m_fO;
};

struct Loc
{
	float x, y, z, o;
	uint32 id;
};

static  const SpawnLocation aAndorovSpawnLocs[MAX_HELPERS] =
{
    {NPC_GENERAL_ANDOROV, -8660.4f,  1510.29f, 32.449f,  2.2184f},
    {NPC_KALDOREI_ELITE,  -8655.84f, 1509.78f, 32.462f,  2.33341f},
    {NPC_KALDOREI_ELITE,  -8657.39f, 1506.28f, 32.418f,  2.33346f},
    {NPC_KALDOREI_ELITE,  -8660.96f, 1504.9f,  32.1567f, 2.33306f},
    {NPC_KALDOREI_ELITE,  -8664.45f, 1506.44f, 32.0944f, 2.33302f}
};


// Movement locations for Andorov
static const SpawnLocation aAndorovMoveLocs[] =
{
    { 0, -8701.51f, 1561.80f, 32.092f },
    { 0, -8718.66f, 1577.69f, 21.612f },
    { 0, -8876.97f, 1651.96f, 21.57f, 5.52f },
    { 0, -8882.15f, 1602.77f, 21.386f },
    { 0, -8940.45f, 1550.69f, 21.616f },
};

struct SortingParameters
{
    uint32 m_uiEntry;
    int32 m_uiYellEntry;
    float m_fSearchDist;
};

static const SortingParameters aArmySortingParameters[MAX_ARMY_WAVES] =
{
    { NPC_CAPTAIN_QEEZ, 0, 16.0f },
    { NPC_CAPTAIN_TUUBID, 0, 15.0f },
    { NPC_CAPTAIN_DRENN, SAY_WAVE3, 19.0f },
    { NPC_CAPTAIN_XURREM, SAY_WAVE4, 16.0f },
    { NPC_MAJOR_YEGGETH, SAY_WAVE5, 14.0f },
    { NPC_MAJOR_PAKKON, SAY_WAVE6, 10.0f },
    { NPC_COLONEL_ZERRAN, SAY_WAVE7, 7.0f },
};

class MANGOS_DLL_DECL instance_ruins_of_ahnqiraj : public ScriptedInstance
{
    public:
        instance_ruins_of_ahnqiraj(Map* pMap);
        ~instance_ruins_of_ahnqiraj() {}

        void Initialize();

        bool IsEncounterInProgress() const;              // not active in AQ20

        void OnCreatureRespawn(Creature * pcreature);
        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);
        void OnPlayerEnter(Player* pPlayer);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureEvade(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void GetKaldoreiGuidList(GuidList& lList) { lList = m_lKaldoreiGuidList; }

        void Update(uint32 uiDiff) override;

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        void DoSapwnAndorovIfCan();
        void DoSortArmyWaves();
        void DoSendNextArmyWave();
        void CheckRajaxxWipe();
        void ResetRajaxx();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        bool m_hasResetRajaxx;
        std::string m_strInstData;

        GuidList m_lKaldoreiGuidList;
        ObjectGuidSet m_sArmyWavesGuids[MAX_ARMY_WAVES];
        ObjectGuidSet m_constArmyWavesGuids[MAX_ARMY_WAVES];

        uint32 m_uiWipeCheckTimer;
        uint32 m_uiArmyDelayTimer;
        uint8 m_uiCurrentArmyWave;
};
#endif
