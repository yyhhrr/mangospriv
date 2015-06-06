/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLACKWING_LAIR
#define DEF_BLACKWING_LAIR

enum
{
    MAX_ENCOUNTER               = 12,

    TYPE_RAZORGORE              = 0,
    TYPE_VAELASTRASZ            = 1,
    TYPE_LASHLAYER              = 2,
    TYPE_FIREMAW                = 3,
    TYPE_EBONROC                = 4,
    TYPE_FLAMEGOR               = 5,
    TYPE_CHROMAGGUS             = 6,
    TYPE_NEFARIAN               = 7,
    VALUE_BREATH1				= 8,
    VALUE_BREATH2				= 9,
	VALUE_DRAGON1				= 10,
	VALUE_DRAGON2				= 11,

    DATA_DRAGON_EGG             = 1,                        // track the used eggs

    NPC_RAZORGORE               = 12435,
    NPC_VAELASTRASZ             = 13020,
    NPC_LASHLAYER               = 12017,
    NPC_FIREMAW                 = 11983,
    NPC_EBONROC                 = 14601,
    NPC_FLAMEGOR                = 11981,
    NPC_CHROMAGGUS              = 14020,
    NPC_NEFARIAN                = 11583,
    NPC_LORD_VICTOR_NEFARIUS    = 10162,
    NPC_BLACKWING_TECHNICIAN    = 13996,                    // Flees at Vael intro event
	NPC_LORD_VICTOR_DUMMY		= 93123,

    // Razorgore event related
    NPC_GRETHOK_THE_CONTROLLER	= 12557,
	NPC_BLACKWING_GUARDSMAN		= 14456,
    NPC_BLACKWING_ORB_TRIGGER   = 14449,
    NPC_NEFARIANS_TROOPS        = 14459,
    NPC_MONSTER_GENERATOR       = 12434,
    NPC_BLACKWING_LEGIONNAIRE   = 12416,                    // one spawn per turn
    NPC_BLACKWING_MAGE          = 12420,                    // one spawn per turn
    NPC_DEATH_TALON_DRAGONSPAWN = 12422,                    // two spawns per turn

	//LASHLAYER DRAGONS
	NPC_BLUE_DRAGON				= 14024,
	NPC_BRONZE_DRAGON			= 14025,
	NPC_RED_DRAGON				= 14022,
	NPC_GREEN_DRAGON			= 14023,
	NPC_DRAGONKIN_LASH			= 12468,
	NPC_PRIEST_LASH				= 12458,
	NPC_SUPPRESSION_TRIGGER		= 800007,

	NPC_BLACKWING_AUFSEHER		= 12461,
	NPC_FLAMMENSCHUPPE			= 12463,
	NPC_SCHNAUBER				= 12464,
	NPC_WYRMKIN					= 12465,
	NPC_HAUPTMANN				= 12467,

	//Nefarian Spawns
    NPC_BRONZE_DRAKANOID        = 14263,
    NPC_BLUE_DRAKANOID          = 14261,
    NPC_RED_DRAKANOID           = 14264,
    NPC_GREEN_DRAKANOID         = 14262,
    NPC_BLACK_DRAKANOID         = 14265,
    NPC_CHROMATIC_DRAKANOID     = 14302,

    GO_PORTCULLIS_ENTRANCE		= 176964,
    GO_PORTCULLIS_RAZORGORE     = 176965,
    GO_PORTCULLIS_CHROMAGGUS    = 179116,
    GO_PORTCULLIS_NEFARIAN		= 179117,
    GO_PORTCULLIS_VAELASTRASZ	= 179364,
    GO_PORTCULLIS_BROODLORD		= 179365,

    GO_ORB_OF_DOMINATION        = 177808,                   // trigger 19832 on Razorgore
    GO_BLACK_DRAGON_EGG         = 177807,
    GO_DRAKONID_BONES           = 179804,
    GO_CHROMAGGUS_LEVER         = 179148,
    GO_DOOR_NEFARIAN            = 176966,

    GO_RAZERGORE_DOOR_LEVER		= 900003,
    GO_VAELASTRASZ_DOOR_LEVER	= 900004,
    GO_BROODLORD_DOOR_LEVER		= 900005,
    GO_CHROMAGGUS_DOOR_LEVER	= 900006,

    GO_SUPPRESSION_DEVICE       = 179784,

    EMOTE_ORB_SHUT_OFF          = -1469035,
    EMOTE_TROOPS_FLEE           = -1469033,                 // emote by Nefarian's Troops npc

	SPELL_ROOTED				= 31366,

    MAX_EGGS_DEFENDERS          = 4,
};

enum Misc
{
    FACTION_FRIENDLY            = 35,
    FACTION_BLACKWING           = 103,
	SPELL_DRAGON_ORB			= 19869,
    SPELL_USE_DRAGON_ORB        = 23018,
	SPELL_MIND_EXHAUSTION		= 23958,
    MAX_BLACKWING_ORC           = 40,
    MAX_BLACKWING_DRAGONKIN     = 12
};

enum Timer
{
    TIMER_START_SPAWMING_ADDS   = 35000,
    TIMER_NEXT_ADD_SPAWN        = 15000
};

struct Location
{
    float x, y, z, o;
};

static Location Corner[]=                       // Spawn positions of Razorgore encounter
{
    {-7659.32f, -1042.81f, 407.21f, 6.05f},
    {-7643.92f, -1064.61f, 407.21f, 0.50f},
    {-7623.02f, -1094.21f, 407.21f, 0.67f},
    {-7607.71f, -1116.54f, 407.21f, 1.37f},
    {-7584.31f, -990.123f, 407.21f, 4.50f},
    {-7568.75f, -1012.85f, 407.21f, 3.71f},
    {-7548.45f, -1041.96f, 407.21f, 3.79f},
    {-7532.35f, -1064.68f, 407.21f, 2.94f},
};

// Coords used to spawn Nefarius at the throne
static const float aNefariusSpawnLoc[4] = { -7466.16f, -1040.80f, 412.053f, 2.14675f};

class MANGOS_DLL_DECL instance_blackwing_lair : public ScriptedInstance
{
    public:
        instance_blackwing_lair(Map* pMap);
        ~instance_blackwing_lair() {}

        void Initialize();

		bool m_uiGrethokManager;
		bool m_uiGuardsmanManager;
		bool m_uiRazorgorManager;
		bool m_uiVealManager;
		bool m_uiLashLayerManager;
		bool m_uiChromaggusManager;

		//void OnCreatureRespawn(Creature* pCreature);
        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);
		void OnObjectUse(GameObject* pGo);

        void OnCreatureEnterCombat(Creature* pCreature);
        void OnCreatureDeath(Creature* pCreature);
		void OnCreatureEvade(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

		void SetData64(uint32 uiData, uint64 uiGuid);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);

		void DespawnTechnicians();
		void KillAllPlayersInRazorgoreRoom();
		void DespawnEggs();
		void DespawnLashlayerRoom();

		void SetPlayerController(ObjectGuid guid);
    protected:
        std::string strInstData;
        uint32 m_auiEncounter[MAX_ENCOUNTER];

		std::vector<uint64> mGuardsmans;
        std::list<Creature*> razorgoreAdds;

        uint32 m_uiResetTimer;
        uint32 m_uiDefenseTimer;
        uint32 m_uiRazorgoreSummonTimer;
        uint32 m_uiDragonkinSummoned;
        uint32 m_uiOrcSummoned;
		uint32 m_uiChromaggusPullTimer;
		uint32 m_uiDoorCheckTimer;

		uint32 m_uiGuardsmanManagerTimer;
		uint32 m_uiGrethokManagerTimer;
		uint32 m_uiRazorgorManagerTimer;
		uint32 m_uiVealManagerTimer;
		uint32 m_uiLashLayerManagerTimer;
		uint32 m_uiChromaggusManagerTimer;

        GuidList m_lTechnicianGuids;

        GUIDList m_lBlackwingGuardsman;
        GUIDList m_lDragonTrio;
        GuidList m_lDragonEggsGuids;
        GuidList m_lUsedEggsGuids;

        GuidList m_lDrakonidBonesGuids;
        GuidList m_lDefendersGuids;
		GuidList m_lSuppressionDeviceGuids;
		GuidList m_lLashDragons;

		GuidList m_lTechnicianAndOther;

        GuidList m_lNefDragonsP1Guids;

		ObjectGuid m_playerController;
};

#endif
