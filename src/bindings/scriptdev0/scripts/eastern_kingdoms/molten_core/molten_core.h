/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_MOLTEN_CORE_H
#define DEF_MOLTEN_CORE_H

enum
{
    MAX_ENCOUNTER           = 13,

    NPC_LUCIFRON            = 12118,
    NPC_MAGMADAR            = 11982,
    NPC_GEHENNAS            = 12259,
    NPC_GARR                = 12057,
    NPC_GEDDON              = 12056,
    NPC_SHAZZRAH            = 12264,
    NPC_GOLEMAGG            = 11988,
    NPC_SULFURON            = 12098,
    NPC_DOMO                = 12018,
    NPC_RAGNAROS            = 11502,
    NPC_FLAMEWAKERPRIEST    = 11662,
	NPC_KERNHUND			= 11671,
	NPC_FEUER_WICHTEL		= 11669,
	NPC_MAJORDOMO			= 80002,

	GO_LAVA_STEAM           = 178107,
	GO_LAVA_SPLASH          = 178108,

    TYPE_SULFURON           = 1,
    TYPE_GEDDON             = 2,
    TYPE_SHAZZRAH           = 3,
    TYPE_GOLEMAGG           = 4,
    TYPE_GARR               = 5,
    TYPE_MAGMADAR           = 6,
    TYPE_GEHENNAS           = 7,
    TYPE_LUCIFRON           = 8,
    TYPE_MAJORDOMO          = 9,
    TYPE_RAGNAROS           = 10,
    TYPE_CHEST				= 17,
    
    TYPE_TRASH_MAG = 18,
    TYPE_TRASH_GAR = 19,

    DATA_SULFURON           = 11,
    DATA_GOLEMAGG           = 12,
    DATA_GARR               = 13,
    DATA_MAJORDOMO          = 14,
    DATA_MAGMADAR			= 15,

    DATA_BOSSES_DEAD_CHECK  = 16
};

static const float RANGE_CALL_FOR_HELP = 15.0f;

struct sSpawnLocation
{
    uint32 m_uiEntry;
    float m_fX, m_fY, m_fZ, m_fO;
};

static sSpawnLocation m_aMajordomoLocations[2] =
{
    {NPC_MAJORDOMO, 757.455750f, -1183.250000f, -118.639053f, 2.880723f},  // Summon fight position
    {NPC_MAJORDOMO, 847.103f, -816.153f, -229.775f, 4.344f}     // Summon and teleport location (near Ragnaros)
};

struct sRuneEncounters
{
    uint32 m_uiRuneEntry, m_uiType;
};

typedef GUIDList GUIDList;


struct MANGOS_DLL_DECL instance_molten_core : public ScriptedInstance
{
    instance_molten_core(Map* pMap) : ScriptedInstance(pMap) {
        Initialize();
    };

	void DoHandleAdds(GUIDList &m_luiAddsGUIDs, bool bRespawn = true);

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

	uint64 m_uiLucifronGUID, m_uiMagmadarGUID, m_uiGehennasGUID, m_uiGarrGUID, m_uiGeddonGUID, m_uiShazzrahGUID, m_uiSulfuronGUID, m_uiGolemaggGUID, m_uiMajorDomoGUID, m_uiRagnarosGUID, m_uiFlamewakerPriestGUID, m_uiFeuerWichtelGUID, m_uiKernHundGUID;

    uint64 m_uiFirelordCacheGUID;

    uint64 magmadar_runeGUID, magmadar_rune_feuerGUID;
    uint64 gehennas_runeGUID, gehennas_rune_feuerGUID;
    uint64 garr_runeGUID, garr_rune_feuerGUID;
    uint64 shazzrah_runeGUID, shazzrah_rune_feuerGUID;
    uint64 golemagg_runeGUID, golemagg_rune_feuerGUID;
    uint64 sulfuron_runeGUID, sulfuron_rune_feuerGUID;
    uint64 geddon_runeGUID, geddon_rune_feuerGUID;
	uint64 domo2_guid;

    GameObject* pGoRune;
    GameObject* pGoRune_Fire;
	Creature*	pWichtel;
	Creature*	pRagi;

	Creature* m_Feuerwichtel;
	Creature* m_Kernhund;

    bool majo_spawned;

	bool b_RuneSulfuronClear;
    bool b_RuneGeddonClear;
	bool b_RuneShazzrahClear;
	bool b_RuneGolemaggClear;
	bool b_RuneGarrClear;
	bool b_RuneMagmadarClear;
	bool b_RuneGehennasClear;

    void Update(uint32 diff)
    {
        if (Creature *domo2 = instance->GetCreature(domo2_guid))
            if (!m_uiRagnarosGUID && GetData(TYPE_RAGNAROS) != DONE && !domo2->isAlive())
                domo2->Respawn();
    }
    
	void MoltenCoreRuneClear(ChatHandler& reader)
	{
		if (b_RuneSulfuronClear)
			reader.PSendSysMessage("Die Rune von Sulfuron wurde gel\303\266scht.");

		if (b_RuneGeddonClear)
			reader.PSendSysMessage("Die Rune von Baron Geddon wurde gel\303\266scht.");

		if (b_RuneShazzrahClear)
			reader.PSendSysMessage("Die Rune von Shazzrah wurde gel\303\266scht.");

		if (b_RuneGolemaggClear)
			reader.PSendSysMessage("Die Rune von Golemagg wurde gel\303\266scht.");

		if (b_RuneGarrClear)
			reader.PSendSysMessage("Die Rune von Garr wurde gel\303\266scht.");

		if (b_RuneMagmadarClear)
			reader.PSendSysMessage("Die Rune von Magmadar wurde gel\303\266scht.");

		if (b_RuneGehennasClear)
			reader.PSendSysMessage("Die Rune von Gehennas wurde gel\303\266scht.");
	}

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiLucifronGUID			= 0;
        m_uiMagmadarGUID			= 0;
        m_uiGehennasGUID			= 0;
        m_uiGarrGUID				= 0;
        m_uiGeddonGUID				= 0;
        m_uiShazzrahGUID			= 0;
        m_uiSulfuronGUID			= 0;
        m_uiGolemaggGUID			= 0;
        m_uiMajorDomoGUID			= 0;
        m_uiRagnarosGUID			= 0;
        m_uiFlamewakerPriestGUID	= 0;
		m_uiFeuerWichtelGUID		= 0;
		m_uiKernHundGUID			= 0;
		domo2_guid = 0;

        magmadar_runeGUID			= 0;
        magmadar_rune_feuerGUID		= 0;
        gehennas_runeGUID			= 0;
        gehennas_rune_feuerGUID		= 0;
        garr_runeGUID				= 0;
        garr_rune_feuerGUID			= 0;
        shazzrah_runeGUID			= 0;
        shazzrah_rune_feuerGUID		= 0;
        golemagg_runeGUID			= 0;
        golemagg_rune_feuerGUID		= 0;
        sulfuron_runeGUID			= 0;
        sulfuron_rune_feuerGUID		= 0;
        geddon_runeGUID				= 0;
        geddon_rune_feuerGUID		= 0;

        m_uiFirelordCacheGUID		= 0;

        majo_spawned			= false;

		b_RuneSulfuronClear		= false;
		b_RuneGeddonClear		= false;
		b_RuneShazzrahClear		= false;
		b_RuneGolemaggClear		= false;
		b_RuneGarrClear			= false;
		b_RuneMagmadarClear		= false;
		b_RuneGehennasClear		= false;
    }

    bool IsEncounterInProgress() const
    {
        return false;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch (pGo->GetEntry())
        {

        case 176954:                                    // Golemagg
            golemagg_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178190:
            golemagg_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 176951:                                    // Sulfuron
            sulfuron_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178187:
            sulfuron_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 176952:                                    // Geddon
            geddon_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178188:
            geddon_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 176956:                                    // Magmadar
            magmadar_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178192:
            magmadar_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 176957:                                    // Gehennas
            gehennas_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178193:
            gehennas_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 176955:                                    // Garr
            garr_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178191:
            garr_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 176953:                                    // Shazzrah
            shazzrah_runeGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 178189:
            shazzrah_rune_feuerGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case 179703:
            m_uiFirelordCacheGUID = pGo->GetGUID();     //majordomo event chest
            break;
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
        case NPC_LUCIFRON:
            m_uiLucifronGUID = pCreature->GetGUID();
            break;
        case NPC_MAGMADAR:
            m_uiMagmadarGUID = pCreature->GetGUID();
            break;
        case NPC_GEHENNAS:
            m_uiGehennasGUID = pCreature->GetGUID();
            break;
        case NPC_GARR:
            m_uiGarrGUID = pCreature->GetGUID();
			m_mNpcEntryGuidStore[NPC_GARR] = m_uiGarrGUID;
            break;
        case NPC_GEDDON:
            m_uiGeddonGUID = pCreature->GetGUID();
            break;
        case NPC_SHAZZRAH:
            m_uiShazzrahGUID = pCreature->GetGUID();
            break;
        case NPC_SULFURON:
            m_uiSulfuronGUID = pCreature->GetGUID();
            break;
        case NPC_GOLEMAGG:
            m_uiGolemaggGUID = pCreature->GetGUID();
            break;
        case NPC_DOMO:
            m_uiMajorDomoGUID = pCreature->GetGUID();
			pCreature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            //pCreature->SetVisibility(VISIBILITY_OFF);
            pCreature->setFaction(734);
            break;
		case 80002: //Domo 2
            domo2_guid = pCreature->GetGUID();
			break;
        case NPC_RAGNAROS:
            m_uiRagnarosGUID = pCreature->GetGUID();
            break;
        case NPC_FLAMEWAKERPRIEST:
            m_uiFlamewakerPriestGUID = pCreature->GetGUID();
            break;
        case NPC_FEUER_WICHTEL:
            m_uiFeuerWichtelGUID = pCreature->GetGUID();
            break;
        case NPC_KERNHUND:
            m_uiKernHundGUID = pCreature->GetGUID();
            break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch (uiType)
        {
        case TYPE_SULFURON:
            pGoRune=instance->GetGameObject(sulfuron_runeGUID);
            pGoRune_Fire=instance->GetGameObject(sulfuron_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
                pGoRune->SetGoState(GO_STATE_READY);
				b_RuneSulfuronClear = true;
            }
            m_auiEncounter[0] = uiData;
            break;

        case TYPE_GEDDON:
            pGoRune=instance->GetGameObject(geddon_runeGUID);
            pGoRune_Fire=instance->GetGameObject(geddon_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
                pGoRune->SetGoState(GO_STATE_READY);
				b_RuneGeddonClear = true;
            }
            m_auiEncounter[1] = uiData;
            break;

        case TYPE_SHAZZRAH:
            pGoRune=instance->GetGameObject(shazzrah_runeGUID);
            pGoRune_Fire=instance->GetGameObject(shazzrah_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
				pGoRune->SetGoState(GO_STATE_READY);
				b_RuneShazzrahClear = true;
            }
            m_auiEncounter[2] = uiData;
            break;

        case TYPE_GOLEMAGG:
            pGoRune=instance->GetGameObject(golemagg_runeGUID);
            pGoRune_Fire=instance->GetGameObject(golemagg_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
                pGoRune->SetGoState(GO_STATE_READY);
				b_RuneGolemaggClear = true;
            }
            m_auiEncounter[3] = uiData;
            break;

        case TYPE_GARR:
            pGoRune=instance->GetGameObject(garr_runeGUID);
            pGoRune_Fire=instance->GetGameObject(garr_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
                pGoRune->SetGoState(GO_STATE_READY);
				b_RuneGarrClear = true;
            }
            m_auiEncounter[4] = uiData;
            break;

        case TYPE_MAGMADAR:
            pGoRune=instance->GetGameObject(magmadar_runeGUID);
            pGoRune_Fire=instance->GetGameObject(magmadar_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
                pGoRune->SetGoState(GO_STATE_READY);
				b_RuneMagmadarClear = true;
            }
            m_auiEncounter[5] = uiData;
            break;

        case TYPE_GEHENNAS:
            pGoRune=instance->GetGameObject(gehennas_runeGUID);
            pGoRune_Fire=instance->GetGameObject(gehennas_rune_feuerGUID);
            if (uiData == DONE)
            {
                pGoRune_Fire->SetGoState(GO_STATE_READY);
            }
            if (uiData == SPECIAL)
            {
                pGoRune->SetGoState(GO_STATE_READY);
				b_RuneGehennasClear = true;
            }
            m_auiEncounter[6] = uiData;
            break;

        case TYPE_LUCIFRON:
            m_auiEncounter[7] = uiData;
            break;

        case TYPE_MAJORDOMO:
            if (uiData == DONE)
            {
                SetData(TYPE_CHEST,DONE);
            }
            m_auiEncounter[8] = uiData;
            break;

        case TYPE_RAGNAROS:
			pRagi=instance->GetCreature(m_uiRagnarosGUID);
			if (uiData == FAIL)
            {
                //pRagi->ForcedDespawn();
				//pRagi->RemoveCorpse();
            }
            m_auiEncounter[9] = uiData;
            break;

        case TYPE_CHEST:
            DoRespawnGameObject(m_uiFirelordCacheGUID, 604800);
            m_auiEncounter[10] = uiData;
            break;

	case TYPE_TRASH_GAR:
	  m_auiEncounter[11] = uiData;
	  break;

	case TYPE_TRASH_MAG:
	  m_auiEncounter[12] = uiData;
	  break;
        }

        if (uiData == DONE || uiData == SPECIAL)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11]  << " " << m_auiEncounter[12] ;

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }


	void RespawnDomo()
	{
		auto domo = instance->GetCreature(domo2_guid);
		if(domo == nullptr)
			return;

		domo->Respawn();
		domo->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	}

    const char* Save()
    {
        return strInstData.c_str();
    }

    bool reload()
    {
        SetData(TYPE_SULFURON, m_auiEncounter[0]);
        SetData(TYPE_GEDDON, m_auiEncounter[1]);
        SetData(TYPE_SHAZZRAH, m_auiEncounter[2]);
        SetData(TYPE_GOLEMAGG, m_auiEncounter[3]);
        SetData(TYPE_GARR, m_auiEncounter[4]);
        SetData(TYPE_MAGMADAR, m_auiEncounter[5]);
        SetData(TYPE_GEHENNAS, m_auiEncounter[6]);
    }

    uint32 GetData(uint32 uiType)
    {
        switch (uiType)
        {
        case TYPE_SULFURON:
            return m_auiEncounter[0];
        case TYPE_GEDDON:
            return m_auiEncounter[1];
        case TYPE_SHAZZRAH:
            return m_auiEncounter[2];
        case TYPE_GOLEMAGG:
            return m_auiEncounter[3];
        case TYPE_GARR:
            return m_auiEncounter[4];
        case TYPE_MAGMADAR:
            return m_auiEncounter[5];
        case TYPE_GEHENNAS:
            return m_auiEncounter[6];
        case TYPE_LUCIFRON:
            return m_auiEncounter[7];
        case TYPE_MAJORDOMO:
            return m_auiEncounter[8];
        case TYPE_RAGNAROS:
            return m_auiEncounter[9];
		case TYPE_TRASH_GAR:
			return m_auiEncounter[11];
		case TYPE_TRASH_MAG:
			return m_auiEncounter[12];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch (uiData)
        {
        case DATA_SULFURON:
            return m_uiSulfuronGUID;

        case DATA_GOLEMAGG:
            return m_uiGolemaggGUID;

        case DATA_GARR:
            return m_uiGarrGUID;

        case DATA_MAJORDOMO:
            return m_uiMajorDomoGUID;

        case DATA_MAGMADAR:
            return m_uiMagmadarGUID;
        }

        return 0;
    }

    void Load(const char* chrIn)
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
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11] >> m_auiEncounter[12];

        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)           // Do not load an encounter as "In Progress" - reset it instead.
                m_auiEncounter[i] = NOT_STARTED;

        //reload();
        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

#endif