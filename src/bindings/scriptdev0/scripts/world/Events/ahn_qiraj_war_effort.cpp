/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################

ScriptName: Ahn'Qiraj War Effort Event
*/

#include "precompiled.h"
#include "../system/system.h"

const int DB_UPDATE_INTERVAL = 10000;

enum collector_npcs
{
    NPC_H_PURPLE_LOTUS = 15512,
    NPC_H_FIREBLOOM = 15508,
    NPC_H_PEACEBLOOM = 15477,
    NPC_H_COPPER_BAR = 15459,
    NPC_H_TIN_BAR = 15460,
    NPC_H_MITHRIL_BAR = 15469,
    NPC_H_LEAN_WOLF_STEAK = 15533,
    NPC_H_BAKED_SALMON = 15535,
    NPC_H_RUNECLOTH_BANDAGE = 15532,
    NPC_H_MAGEWEAVE_BANDAGE = 15529,
    NPC_H_WOOL_BANDAGE = 15528,
    NPC_H_SPOTTED_YELLOWTAIL = 15534,
    NPC_H_RUGGED_LEATHER = 15525,
    NPC_H_HEAVY_LEATHER = 15515,
    NPC_H_THICK_LEATHER = 15522,
    NPC_A_RAINBOW_FIN_ALBACORE = 15455,
    NPC_A_COPPER_BAR = 15383,
    NPC_A_ARTHAS_TEAR = 15445,
    NPC_A_LINEN_BANDAGE = 15451,
    NPC_A_SILK_BANDAGE = 15452,
    NPC_A_RUNECLOTH_BANDAGE = 15453,
    NPC_A_ROAST_RAPTOR = 15456,
    NPC_A_MEDIUM_LEATHER = 15448,
    NPC_A_STRANGLEKELP = 15434,
    NPC_A_PURPLE_LOTUS = 15437,
    NPC_A_SPOTTED_YELLOWTAIL = 15457,
    NPC_A_THORIUM_BAR = 15432,
    NPC_A_IRON_BAR = 15431,
    NPC_A_LIGHT_LEATHER = 15446,
    NPC_A_THICK_LEATHER = 15450,
};

enum collect_quests
{
    QUEST_H_PURPLE_LOTUS = 8582,
    QUEST_H_PURPLE_LOTUS_2 = 8583,
    QUEST_H_FIREBLOOM = 8580,
    QUEST_H_FIREBLOOM_2 = 8581,
    QUEST_H_LEAN_WOLF_STEAK = 8611,
    QUEST_H_LEAN_WOLF_STEAK_2 = 8612,
    QUEST_A_LIGHT_LEATHER = 8511,
    QUEST_A_LIGHT_LEATHER_2 = 8512,
    QUEST_H_BAKED_SALMON = 8615,
    QUEST_H_BAKED_SALMON_2 = 8616,
    QUEST_A_IRON_BAR = 8494,
    QUEST_A_IRON_BAR_2 = 8495,
    QUEST_A_THORUM_BAR = 8499,
    QUEST_A_THORUM_BAR_2 = 8500,
    QUEST_H_RUGGED_LEATHER = 8600,
    QUEST_H_RUGGED_LEATHER_2 = 8601,
    QUEST_H_SPOTTED_YELLOWTAIL = 8613,
    QUEST_H_SPOTTED_YELLOWTAIL_2 = 8614,
    QUEST_H_TIN_BAR = 8542,
    QUEST_H_TIN_BAR_2 = 8543,
    QUEST_H_WOOL_BANDAGE = 8604,
    QUEST_H_WOOL_BANDAGE_2 = 8605,
    QUEST_H_PEACEBLOOM = 8549,
    QUEST_H_PEACEBLOOM_2 = 8550,
    QUEST_A_SPOTTED_YELLOWTAIL = 8528,
    QUEST_A_SPOTTED_YELLOWTAIL_2 = 8529,
    QUEST_A_RUNECLOTH_BANDAGE = 8522,
    QUEST_A_RUNECLOTH_BANDAGE_2 = 8523,
    QUEST_H_MAGEWEAVE_BANDAGE = 8607,
    QUEST_H_MAGEWEAVE_BANDAGE_2 = 8608,
    QUEST_A_THICK_LEATHER = 8515,
    QUEST_A_THICK_LEATHER_2 = 8516,
    QUEST_A_PURPLE_LOTUS = 8505,
    QUEST_A_PURPLE_LOTUS_2 = 8506,
    QUEST_A_STRANGLEKELP = 8503,
    QUEST_A_STRANGLEKELP_2 = 8504,
    QUEST_H_COPPER_BAR = 8532,
    QUEST_H_COPPER_BAR_2 = 8533,
    QUEST_A_SILK_BANDAGE = 8520,
    QUEST_A_SILK_BANDAGE_2 = 8521,
    QUEST_A_MEDIUM_LEATHER = 8513,
    QUEST_A_MEDIUM_LEATHER_2 = 8514,
    QUEST_A_ROAST_RAPTOR = 8526,
    QUEST_A_ROAST_RAPTOR_2 = 8527,
    QUEST_H_MITHRIL_BAR = 8545,
    QUEST_H_MITHRIL_BAR_2 = 8546,
    QUEST_A_LINEN_BANDAGE = 8517,
    QUEST_A_LINEN_BANDAGE_2 = 8518,
    QUEST_A_ARTHAS_TEAR = 8509,
    QUEST_A_ARTHAS_TEAR_2 = 8510,
    QUEST_A_COPPER_BAR = 8492,
    QUEST_A_COPPER_BAR_2 = 8493,
    QUEST_H_THICK_LEATHER = 8590,
    QUEST_H_THICK_LEATHER_2 = 8591,
    QUEST_H_HEAVY_LEATHER = 8588,
    QUEST_H_HEAVY_LEATHER_2 = 8589,
    QUEST_A_RAINBOW_FIN_ALBACORE = 8524,
    QUEST_A_RAINBOW_FIN_ALBACORE_2 = 8525,
    QUEST_H_RUNECLOTH_BANDAGE = 8609,
    QUEST_H_RUNECLOTH_BANDAGE_2 = 8610,
};

static const double aSpawnPositions[10][4] =
{
    { 1637.11f, -4147.21f, 36.0414, -2.54818f },                // Horde Herb
    { 1683.11f, -4134.35f, 39.5419f, -2.56563f },               // Horde 
    { 1619.83f, -4092.43f, 34.5107f, -2.58309f },               // Horde 
    { 1579.35f, -4109.25f, 34.5417f, -2.53073f },               // Horde 
    { 1590.82f, -4155.33f, 36.2926f, -2.58309f },               // Horde Cooking
    { -4971.55f, -1148.57f, 501.648f, 2.28638f },               // Alliance Bandages
    { -4937.29f, -1282.74f, 501.672f, 2.26893f },               // Alliance Cooking
    { -4937.29f, -1282.74f, 501.672f, 2.26893f },               // Alliance Herbs
    { -4913.85f, -1226.00f, 501.651f, 2.25148f },               // Alliance Bars
    { -4958.51f, -1179.32f, 501.659f, 2.26893f },               // Alliance Skins
};

static uint8 gobjectState[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

struct MANGOS_DLL_DECL npc_aq_war_effort_collector : public ScriptedAI
{
    npc_aq_war_effort_collector(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	uint32 m_progess;
	uint32 m_updateTimer;

	void Reset()
	{
        m_progess = 0;
        m_updateTimer = DB_UPDATE_INTERVAL;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        m_updateTimer += uiDiff;
        if (m_updateTimer >= DB_UPDATE_INTERVAL)
        {
            m_updateTimer %= DB_UPDATE_INTERVAL;

            uint32 quest = GetQuestId(m_creature->GetEntry());

            if (!quest)
                return;
			
            QueryResult* result = SD0Database.PQuery("SELECT progress FROM world_events WHERE id=%u", GetWorldEventId(quest));

            if (!result)
                return;

            Field* fields = result->Fetch();

            if (fields == nullptr)
                return;

            uint32 progress = (uint32)fields[0].GetDouble();

            if (progress >= GetGlobalMaximum(quest))
                m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

            uint8 groupIndex;
            uint32 gobaseid; 
            float x, y, z, o;
            
            GetGameObjectInfo(gobaseid, groupIndex, x, y, z, o);

            if (!gobaseid)
                return;

            uint32 resourcesPerTier = GetGlobalMaximum(quest) / 5;

            while (progress >= (gobjectState[groupIndex] + 1) * resourcesPerTier && gobjectState[groupIndex] < 5)
            {
                int goentry = gobaseid + ++gobjectState[groupIndex];
                if (goentry == 180805)
                    goentry = 180800;
                GameObject* go = m_creature->SummonObject(m_creature->GetMap(), goentry, x, y, z, o);
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void GetGameObjectInfo(uint32 &gobaseid, uint8 &groupIndex, float &x, float &y, float &z, float &o)
    {
        switch (m_creature->GetEntry())
        {
        case NPC_H_PURPLE_LOTUS:
        case NPC_H_FIREBLOOM:
        case NPC_H_PEACEBLOOM:
            gobaseid = 180818;
            groupIndex = 0;
            break;
        case NPC_H_COPPER_BAR:
        case NPC_H_TIN_BAR:
        case NPC_H_MITHRIL_BAR:
            gobaseid = 180838;
            groupIndex = 1;
            break;
        case NPC_H_BAKED_SALMON:
        case NPC_H_LEAN_WOLF_STEAK:
        case NPC_H_SPOTTED_YELLOWTAIL:
            gobaseid = 180832;
            groupIndex = 2;
            break;
        case NPC_H_RUNECLOTH_BANDAGE:
        case NPC_H_MAGEWEAVE_BANDAGE:
        case NPC_H_WOOL_BANDAGE:
            gobaseid = 180826;
            groupIndex = 3;
            break;
        case NPC_H_RUGGED_LEATHER:
        case NPC_H_HEAVY_LEATHER:
        case NPC_H_THICK_LEATHER:
            gobaseid = 180812;
            groupIndex = 4;
            break;
        case NPC_A_LINEN_BANDAGE:
        case NPC_A_SILK_BANDAGE:
        case NPC_A_RUNECLOTH_BANDAGE:
            gobaseid = 180673;
            groupIndex = 5;
            break;
        case NPC_A_ROAST_RAPTOR:
        case NPC_A_SPOTTED_YELLOWTAIL:
        case NPC_A_RAINBOW_FIN_ALBACORE:
            gobaseid = 180804;
            groupIndex = 6;
            break;
        case NPC_A_ARTHAS_TEAR:
        case NPC_A_PURPLE_LOTUS:
        case NPC_A_STRANGLEKELP:
            gobaseid = 180800;
            groupIndex = 7;
            break;
        case NPC_A_COPPER_BAR:
        case NPC_A_IRON_BAR:
        case NPC_A_THORIUM_BAR:
            gobaseid = 180779;
            groupIndex = 8;
            break;
        case NPC_A_LIGHT_LEATHER:
        case NPC_A_MEDIUM_LEATHER:
        case NPC_A_THICK_LEATHER:
            gobaseid = 180691;
            groupIndex = 9;
            break;
        default:
            return;
        }

        x = aSpawnPositions[groupIndex][0];
        y = aSpawnPositions[groupIndex][1];
        z = aSpawnPositions[groupIndex][2];
        o = aSpawnPositions[groupIndex][3];

        return;
    }

    static uint32 GetQuestId(uint32 creatureEntity)
    {
        switch (creatureEntity)
        {
        case NPC_H_PURPLE_LOTUS:
            return QUEST_H_PURPLE_LOTUS;
        case NPC_H_FIREBLOOM:
            return QUEST_H_FIREBLOOM;
        case NPC_H_PEACEBLOOM:
            return QUEST_H_PEACEBLOOM;
        case NPC_H_COPPER_BAR:
            return QUEST_H_COPPER_BAR;
        case NPC_H_TIN_BAR:
            return QUEST_H_TIN_BAR;
        case NPC_H_MITHRIL_BAR:
            return QUEST_H_MITHRIL_BAR;
        case NPC_H_BAKED_SALMON:
            return QUEST_H_BAKED_SALMON;
        case NPC_H_LEAN_WOLF_STEAK:
            return QUEST_H_LEAN_WOLF_STEAK;
        case NPC_H_RUNECLOTH_BANDAGE:
            return QUEST_H_RUNECLOTH_BANDAGE;
        case NPC_H_MAGEWEAVE_BANDAGE:
            return QUEST_H_MAGEWEAVE_BANDAGE;
        case NPC_H_WOOL_BANDAGE:
            return QUEST_H_WOOL_BANDAGE;
        case NPC_H_SPOTTED_YELLOWTAIL:
            return QUEST_H_SPOTTED_YELLOWTAIL;
        case NPC_H_THICK_LEATHER:
            return QUEST_H_THICK_LEATHER;
        case NPC_H_HEAVY_LEATHER:
            return QUEST_H_HEAVY_LEATHER;
        case NPC_H_RUGGED_LEATHER:
            return QUEST_H_RUGGED_LEATHER;
        case NPC_A_RUNECLOTH_BANDAGE:
            return QUEST_A_RUNECLOTH_BANDAGE;
        case NPC_A_SILK_BANDAGE:
            return QUEST_A_SILK_BANDAGE;
        case NPC_A_LINEN_BANDAGE:
            return QUEST_A_LINEN_BANDAGE;
        case NPC_A_RAINBOW_FIN_ALBACORE:
            return QUEST_A_RAINBOW_FIN_ALBACORE;
        case NPC_A_ROAST_RAPTOR:
            return QUEST_A_ROAST_RAPTOR;
        case NPC_A_SPOTTED_YELLOWTAIL:
            return QUEST_A_SPOTTED_YELLOWTAIL;
        case NPC_A_ARTHAS_TEAR:
            return QUEST_A_ARTHAS_TEAR;
        case NPC_A_PURPLE_LOTUS:
            return QUEST_A_PURPLE_LOTUS;
        case NPC_A_STRANGLEKELP:
            return QUEST_A_STRANGLEKELP;
        case NPC_A_COPPER_BAR:
            return QUEST_A_COPPER_BAR;
        case NPC_A_IRON_BAR:
            return QUEST_A_IRON_BAR;
        case NPC_A_THORIUM_BAR:
            return QUEST_A_THORUM_BAR;
        case NPC_A_LIGHT_LEATHER:
            return QUEST_A_LIGHT_LEATHER;
        case NPC_A_MEDIUM_LEATHER:
            return QUEST_A_MEDIUM_LEATHER;
        case NPC_A_THICK_LEATHER:
            return QUEST_A_THICK_LEATHER;
        }

        return 0;
    }

    static uint32 GetWorldEventId(uint32 questId)
    {
        switch (questId)
        {
        case QUEST_H_COPPER_BAR:
        case QUEST_H_COPPER_BAR_2:
            return 3;
        case QUEST_A_COPPER_BAR:
        case QUEST_A_COPPER_BAR_2:
            return 4;
        case QUEST_A_PURPLE_LOTUS:
        case QUEST_A_PURPLE_LOTUS_2:
            return 5;
        case QUEST_H_PURPLE_LOTUS:
        case QUEST_H_PURPLE_LOTUS_2:
            return 6;
        case QUEST_A_THICK_LEATHER:
        case QUEST_A_THICK_LEATHER_2:
            return 7;
        case QUEST_H_THICK_LEATHER:
        case QUEST_H_THICK_LEATHER_2:
            return 8;
        case QUEST_A_SPOTTED_YELLOWTAIL:
        case QUEST_A_SPOTTED_YELLOWTAIL_2:
            return 9;
        case QUEST_H_SPOTTED_YELLOWTAIL:
        case QUEST_H_SPOTTED_YELLOWTAIL_2:
            return 10;
        case QUEST_A_RUNECLOTH_BANDAGE:
        case QUEST_A_RUNECLOTH_BANDAGE_2:
            return 11;
        case QUEST_H_RUNECLOTH_BANDAGE:
        case QUEST_H_RUNECLOTH_BANDAGE_2:
            return 12;
        case QUEST_A_IRON_BAR:
        case QUEST_A_IRON_BAR_2:
            return 13;
        case QUEST_A_THORUM_BAR:
        case QUEST_A_THORUM_BAR_2:
            return 14;
        case QUEST_A_ARTHAS_TEAR:
        case QUEST_A_ARTHAS_TEAR_2:
            return 15;
        case QUEST_A_STRANGLEKELP:
        case QUEST_A_STRANGLEKELP_2:
            return 16;
        case QUEST_A_LIGHT_LEATHER:
        case QUEST_A_LIGHT_LEATHER_2:
            return 17;
        case QUEST_A_MEDIUM_LEATHER:
        case QUEST_A_MEDIUM_LEATHER_2:
            return 18;
        case QUEST_A_ROAST_RAPTOR:
        case QUEST_A_ROAST_RAPTOR_2:
            return 19;
        case QUEST_A_RAINBOW_FIN_ALBACORE:
        case QUEST_A_RAINBOW_FIN_ALBACORE_2:
            return 20;
        case QUEST_A_LINEN_BANDAGE:
        case QUEST_A_LINEN_BANDAGE_2:
            return 21;
        case QUEST_A_SILK_BANDAGE:
        case QUEST_A_SILK_BANDAGE_2:
            return 22;
        case QUEST_H_TIN_BAR:
        case QUEST_H_TIN_BAR_2:
            return 23;
        case QUEST_H_MITHRIL_BAR:
        case QUEST_H_MITHRIL_BAR_2:
            return 24;
        case QUEST_H_PEACEBLOOM:
        case QUEST_H_PEACEBLOOM_2:
            return 25;
        case QUEST_H_FIREBLOOM:
        case QUEST_H_FIREBLOOM_2:
            return 26;
        case QUEST_H_HEAVY_LEATHER:
        case QUEST_H_HEAVY_LEATHER_2:
            return 27;
        case QUEST_H_RUGGED_LEATHER:
        case QUEST_H_RUGGED_LEATHER_2:
            return 28;
        case QUEST_H_LEAN_WOLF_STEAK:
        case QUEST_H_LEAN_WOLF_STEAK_2:
            return 29;
        case QUEST_H_BAKED_SALMON:
        case QUEST_H_BAKED_SALMON_2:
            return 30;
        case QUEST_H_WOOL_BANDAGE:
        case QUEST_H_WOOL_BANDAGE_2:
            return 31;
        case QUEST_H_MAGEWEAVE_BANDAGE:
        case QUEST_H_MAGEWEAVE_BANDAGE_2:
            return 32;
        }
        return 0;
    }

    static uint32 GetGlobalMaximum(uint32 questId)
    {
        switch (questId)
        {
        case QUEST_H_COPPER_BAR:
        case QUEST_H_COPPER_BAR_2:
        case QUEST_A_COPPER_BAR:
        case QUEST_A_COPPER_BAR_2:
            return 90000 / 20;
        case QUEST_A_PURPLE_LOTUS:
        case QUEST_A_PURPLE_LOTUS_2:
        case QUEST_H_PURPLE_LOTUS:
        case QUEST_H_PURPLE_LOTUS_2:
            return 26000 / 20;
        case QUEST_A_THICK_LEATHER:
        case QUEST_A_THICK_LEATHER_2:
        case QUEST_H_THICK_LEATHER:
        case QUEST_H_THICK_LEATHER_2:
            return 80000 / 10;
        case QUEST_A_SPOTTED_YELLOWTAIL:
        case QUEST_A_SPOTTED_YELLOWTAIL_2:
        case QUEST_H_SPOTTED_YELLOWTAIL:
        case QUEST_H_SPOTTED_YELLOWTAIL_2:
            return 17000 / 20;
        case QUEST_A_RUNECLOTH_BANDAGE:
        case QUEST_A_RUNECLOTH_BANDAGE_2:
        case QUEST_H_RUNECLOTH_BANDAGE:
        case QUEST_H_RUNECLOTH_BANDAGE_2:
            return 400000 / 20;
        case QUEST_A_IRON_BAR:
        case QUEST_A_IRON_BAR_2:
            return 28000 / 20;
        case QUEST_A_THORUM_BAR:
        case QUEST_A_THORUM_BAR_2:
            return 24000 / 20;
        case QUEST_A_ARTHAS_TEAR:
        case QUEST_A_ARTHAS_TEAR_2:
            return 20000 / 20;
        case QUEST_A_STRANGLEKELP:
        case QUEST_A_STRANGLEKELP_2:
            return 33000 / 20;
        case QUEST_A_LIGHT_LEATHER:
        case QUEST_A_LIGHT_LEATHER_2:
            return 180000 / 10;
        case QUEST_A_MEDIUM_LEATHER:
        case QUEST_A_MEDIUM_LEATHER_2:
            return 110000 / 10;
        case QUEST_A_ROAST_RAPTOR:
        case QUEST_A_ROAST_RAPTOR_2:
            return 20000 / 20;
        case QUEST_A_RAINBOW_FIN_ALBACORE:
        case QUEST_A_RAINBOW_FIN_ALBACORE_2:
            return 14000 / 20;
        case QUEST_A_LINEN_BANDAGE:
        case QUEST_A_LINEN_BANDAGE_2:
            return 800000 / 20;
        case QUEST_A_SILK_BANDAGE:
        case QUEST_A_SILK_BANDAGE_2:
            return 600000 / 20;
        case QUEST_H_TIN_BAR:
        case QUEST_H_TIN_BAR_2:
            return 22000 / 20;
        case QUEST_H_MITHRIL_BAR:
        case QUEST_H_MITHRIL_BAR_2:
            return 18000 / 20;
        case QUEST_H_PEACEBLOOM:
        case QUEST_H_PEACEBLOOM_2:
            return 96000 / 20;
        case QUEST_H_FIREBLOOM:
        case QUEST_H_FIREBLOOM_2:
            return 19000 / 20;
        case QUEST_H_HEAVY_LEATHER:
        case QUEST_H_HEAVY_LEATHER_2:
            return 60000 / 10;
        case QUEST_H_RUGGED_LEATHER:
        case QUEST_H_RUGGED_LEATHER_2:
            return 60000 / 10;
        case QUEST_H_LEAN_WOLF_STEAK:
        case QUEST_H_LEAN_WOLF_STEAK_2:
            return 10000 / 20;
        case QUEST_H_BAKED_SALMON:
        case QUEST_H_BAKED_SALMON_2:
            return 10000 / 20;
        case QUEST_H_WOOL_BANDAGE:
        case QUEST_H_WOOL_BANDAGE_2:
            return 250000 / 20;
        case QUEST_H_MAGEWEAVE_BANDAGE:
        case QUEST_H_MAGEWEAVE_BANDAGE_2:
            return 250000 / 20;
        }
        return 0;
    }

};

CreatureAI* GetAI_npc_aq_war_effort_collector(Creature* pCreature)
{
    return new npc_aq_war_effort_collector(pCreature);
}

bool QuestRewarded_npc_aq_war_effort_collector(Player*, Creature* pCreature, const Quest* pQuest)
{
    uint32 dbID = npc_aq_war_effort_collector::GetWorldEventId(pQuest->GetQuestId());
    
    if (!dbID)
        return true;

    SD0Database.PExecute("UPDATE world_events SET progress = (progress + 1) WHERE id=%u", dbID);

    return true;
}

enum
{
    NPC_GORCHUK         = 15700,
    NPC_SNOWFALL        = 15701,
    NPC_STRONGHAMMER    = 15458,
    NPC_ZOG             = 15539,

    GOSSIP_MENU_TRANSIT = 404,
};

static const uint32 aGossipData[4][2] =
{
    { NPC_GORCHUK, 10267 },
    { NPC_SNOWFALL, 6772 },
    { NPC_STRONGHAMMER, 10274 },
    { NPC_ZOG, 6665 },
};

bool GossipHello_npc_war_effort_commander(Player* pPlayer, Creature* pCreature)
{
    uint32 status = 0;//pCreature->GetWorldState(31);

    switch (status)
    {
        // collecting ressources
        case 0:
            if (uint32 menu = pCreature->GetCreatureInfo()->GossipMenuId)
                pPlayer->SEND_GOSSIP_MENU(menu, pCreature->GetObjectGuid());
            break;
        // 5 days transit
        case 1:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_TRANSIT, pCreature->GetObjectGuid());
            break;
    }

    return true;
}

struct npc_aq_war_effort_commander : public ScriptedAI
{
    npc_aq_war_effort_commander(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() { }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void ReceiveEmote(Player* pPlayer, uint32 uiEmote) override
    {
        if (uiEmote == 59 && m_creature->GetEntry() == NPC_ZOG)
        {
            WorldPacket data(SMSG_MESSAGECHAT, 200);
            WorldObject::BuildMonsterChat(&data, m_creature->GetObjectGuid(), CHAT_MSG_MONSTER_EMOTE, "General Zog acknowledges your obedience.", LANG_UNIVERSAL,
                m_creature->GetName(), pPlayer->GetObjectGuid(), pPlayer->GetName());
            pPlayer->SendDirectMessage(&data);
            m_creature->HandleEmote(EMOTE_ONESHOT_SALUTE);
        }
    }
};

CreatureAI* GetAI_npc_aq_war_effort_commander(Creature* pCreature)
{
    return new npc_aq_war_effort_commander(pCreature);
}

void AddSC_aq_war_effort()
{
	Script* pNewScript;
	pNewScript = new Script;
	pNewScript->Name = "npc_aq_war_effort_collector";
    pNewScript->GetAI = &GetAI_npc_aq_war_effort_collector;
    pNewScript->pQuestRewardedNPC = &QuestRewarded_npc_aq_war_effort_collector;
	pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_aq_war_effort_commander";
    pNewScript->GetAI = &GetAI_npc_aq_war_effort_commander;
    pNewScript->pGossipHello = &GossipHello_npc_war_effort_commander;
    pNewScript->RegisterSelf();
}
