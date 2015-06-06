
#include "precompiled.h"
#include "molten_core.h"
#include "TemporarySummon.h"

#include "boss_ragnaros.h"

#define GOSSIP_ITEM_RAG "Zeig dich, RAGNAROS!"

enum
{
    SAY_AGGRO               = -1409003,
    SAY_SLAY                = -1409005,
    SAY_SPECIAL             = -1409006,                     // Use unknown
    SAY_LAST_ADD            = -1409019,                     // When only one add remaining
    SAY_DEFEAT_1            = -1409007,
    SAY_DEFEAT_2            = -1409020,
    SAY_DEFEAT_3            = -1409021,

    SAY_SUMMON_0            = -1409023,
    SAY_SUMMON_1            = -1409024,
    SAY_SUMMON_MAJ          = -1409008,
    SAY_ARRIVAL1_RAG        = -1409009,
    SAY_ARRIVAL2_MAJ        = -1409010,
    SAY_ARRIVAL3_RAG        = -1409011,
    SAY_ARRIVAL4_MAJ        = -1409022,
	SAY_ARRIVAL5_RAG        = -1409012,

    GOSSIP_ITEM_SUMMON_1    = -3409000,
    GOSSIP_ITEM_SUMMON_2    = -3409001,
    GOSSIP_ITEM_SUMMON_3    = -3409002,

    TEXT_ID_SUMMON_1        = 4995,
    TEXT_ID_SUMMON_2        = 5011,
    TEXT_ID_SUMMON_3        = 5012,

    SPELL_TELEPORT_SELF     = 19484,
    SPELL_SUMMON_RAGNAROS   = 19774,
    SPELL_ELEMENTAL_FIRE    = 19773,
    SPELL_RAGEMERGE		    = 20568,
	SPELL_RAGNA_SUBMERGE    = 21107,	// Apply: mod_stealth
	SPELL_ABTAUCHEN			= 20567,	// Dummy effect
	SPELL_SUBMERGE_EFFECT	= 21859,
};

struct MANGOS_DLL_DECL boss_major_ragAI : public ScriptedAI
{
    boss_major_ragAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		m_uiRagnarosSummonPlayerGUID = ObjectGuid();
        Reset();
    }

    ScriptedInstance* pInstance;

	uint32 m_uiSpeechTimer;
	uint8 m_uiSpeech;

	ObjectGuid m_ragnarosGuid;
    GUIDList m_RagnarosList;

	ObjectGuid m_uiRagnarosSummonPlayerGUID;

	bool b_RagnarosEventStart;

    void Reset()
    {
       m_uiSpeechTimer		= 2000;
	   m_uiSpeech			= 0;

	   b_RagnarosEventStart = false;

	   m_uiRagnarosSummonPlayerGUID.Clear();

	   m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    Creature* GetRagnaros()
    {
        if (!pInstance)
            return NULL;

		Creature* pRagnaros = pInstance->GetSingleCreatureFromStorage(NPC_RAGNAROS);

        return pRagnaros ? pRagnaros : NULL;
    }

    void StartSummonEvent(Player* pPlayer)
    {
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        // Prevent possible exploits with double summoning
        if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(m_ragnarosGuid))
            return;

		if (Creature* pRagnaros = GetRagnaros())
			pRagnaros->ForcedDespawn();

        DoScriptText(SAY_SUMMON_0, m_creature, pPlayer);

        m_uiSpeechTimer = 3000;
        m_uiSpeech = 1;
    }

	void StartSummonEventInstant(Player* pPlayer)
	{
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        // Prevent possible exploits with double summoning
        if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(m_ragnarosGuid))
            return;

		if (Creature* pRagnaros = GetRagnaros())
			pRagnaros->ForcedDespawn();

        DoScriptText(SAY_SUMMON_0, m_creature, pPlayer);

        m_uiSpeechTimer = 0;
        m_uiSpeech = 15;

		Creature* ragnaros = m_creature->SummonCreature(NPC_RAGNAROS, 839.66f, -831.60f, -231.91f, 3, TEMPSUMMON_DEAD_DESPAWN, 864000);
		if(ragnaros == nullptr)
			return;

		m_RagnarosList.push_back(ragnaros->GetGUID());
		static_cast<boss_ragnarosAI*>(ragnaros->AI())->StartEvent(true);
		m_creature->SetRespawnDelay(std::numeric_limits<uint32>::max());
		m_creature->CastSpell(m_creature, 5, true);
	}

	void RagnarosEncounterStart(ChatHandler& reader)
	{
		if (b_RagnarosEventStart)
			reader.PSendSysMessage("Der Kampf mit Ragnaros beginnt in 10 Sekunden!");
	}

    void JustSummoned(Creature* pSummoned)
    {
		if(m_uiSpeech >= 15)
			return;
        // Also remove npc flags where needed
        switch(pSummoned->GetEntry())
        {
            case NPC_RAGNAROS:
				pSummoned->SetVisibility(VISIBILITY_OFF);
                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				//pSummoned->HandleEmote(EMOTE_ONESHOT_SUBMERGE);
				m_RagnarosList.push_back(pSummoned->GetObjectGuid());
                break;
        }

        pSummoned->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Handling of his combat-end speech and Ragnaros summoning
        if (m_uiSpeech)
        {
            if (m_uiSpeechTimer < uiDiff)
            {
                switch (m_uiSpeech)
                {
                    // Ragnaros Summon Event
                    case 1:
                        DoScriptText(SAY_SUMMON_1, m_creature);
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 1000;
                        break;
                    case 2:
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_RAGNAROS);
                        m_creature->GetMotionMaster()->MovePoint(1, 831.079590f, -816.023193f, -229.023270f);
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 9500;
                        break;
                    case 3:
                        // Reset orientation
                        if (GameObject* pLavaSteam = pInstance->GetSingleGameObjectFromStorage(GO_LAVA_STEAM))
                            m_creature->SetFacingToObject(pLavaSteam);
                        m_uiSpeechTimer = 1000;
                        ++m_uiSpeech;
                        break;
                    case 4:
                        DoScriptText(SAY_SUMMON_MAJ, m_creature);
						m_creature->SummonCreature(NPC_RAGNAROS, 839.66f, -831.60f, -231.91f, 3, TEMPSUMMON_MANUAL_DESPAWN, 0);

						detail_log("Ragnaros ist gespawnt.");
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 1000;
                        break;
                    case 5:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
							{
								m_creature->SetFacingTo(5.25f);
								//pRagnaros->CastSpell(pRagnaros, SPELL_SUBMERGE_EFFECT,  true);
								pRagnaros->CastSpell(pRagnaros, SPELL_RAGNA_SUBMERGE, false);
								pRagnaros->SetVisibility(VISIBILITY_ON);

								detail_log("Ragnaros taucht ab, damit der Effekt funktioniert.");							
							}
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 6000;
                        break;
                    case 6:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
							{
								pRagnaros->RemoveAurasDueToSpell(SPELL_RAGNA_SUBMERGE);
								pRagnaros->CastSpell(m_creature, SPELL_RAGEMERGE, false);
							}
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 4000;
                    case 7:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
							{
								//m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_EFFECT);

								detail_log("Ragnaros sollte nun aus dem Boden kommen. Mit Effekt.");
							}
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 8:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
								pRagnaros->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 9:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
								DoScriptText(SAY_ARRIVAL1_RAG, pRagnaros);
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 13000;
                        break;
                    case 10:
                        DoScriptText(SAY_ARRIVAL2_MAJ, m_creature);
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 10000;
                        break;
                    case 11:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
								DoScriptText(SAY_ARRIVAL3_RAG, pRagnaros);
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 10000;
                        break;
                    case 12:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
								pRagnaros->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
						}
                        ++m_uiSpeech;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 13:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
							{
								pRagnaros->CastSpell(m_creature, SPELL_ELEMENTAL_FIRE, true);
								//pRagnaros->CastSpell(m_creature, 5, true);
								static_cast<boss_ragnarosAI*>(pRagnaros->AI())->StartEvent(false);
							}
						}
						m_creature->SetRespawnDelay(std::numeric_limits<uint32>::max());
						++m_uiSpeech;
                        m_uiSpeechTimer = 500;
                        break;
					case 14:
						{
							m_creature->CastSpell(m_creature, 5, true);
							++m_uiSpeech;
						} break;
						/*
                    case 14:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
							{
								DoScriptText(SAY_ARRIVAL5_RAG, pRagnaros);

								if (m_uiRagnarosSummonPlayerGUID)
								{
									if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiRagnarosSummonPlayerGUID))
										pRagnaros->SetFacingToObject(pPlayer);
								}

								b_RagnarosEventStart = true;
							}
                        }
						++m_uiSpeech;
                        m_uiSpeechTimer = 12000;
                        break;
                    case 15:
						for (GUIDList::const_iterator itr = m_RagnarosList.begin(); itr != m_RagnarosList.end(); ++itr)
						{
							if (Creature* pRagnaros = m_creature->GetMap()->GetCreature(*itr))
							{
								pRagnaros->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
								
								if (Player* pPlayer = pInstance->GetPlayerInMap(true, false))
								{
									pRagnaros->AI()->AttackStart(pPlayer);
									m_creature->SetRespawnDelay(std::numeric_limits<uint32>::max());
									m_creature->ForcedDespawn();
									return;
								}
							}
                        }
                        break;
						*/
				}
            }
            else
                m_uiSpeechTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_major_rag(Creature* pCreature)
{
    return new boss_major_ragAI(pCreature);
}

bool GossipHello_boss_major_rag(Player* pPlayer, Creature* pCreature)
{
	ScriptedInstance* pInstance;
	pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
	uint32 uidata=pInstance->GetData(TYPE_MAJORDOMO);
   
	std::stringstream ss;
	ss << "MAJORDOMO DEBUG: ";
	ss << pPlayer->GetInstanceId() << " " << pPlayer->GetName() << " " << uidata;

	if (uidata == DONE || pPlayer->isGameMaster())
	{
		ss << " in if";
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RAG, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
		if(pPlayer->isGameMaster())
		{
			ss << " is GM";
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<Gamemaster> Spawn Ragnaros Instant", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
		}
	}

	ss << " after if";
	debug_log(ss.str().c_str());

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

	return true;
}

bool GossipSelect_boss_major_rag(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

		if (boss_major_ragAI* pMajoAI = static_cast<boss_major_ragAI*>(pCreature->AI()))
                pMajoAI->StartSummonEvent(pPlayer);
    }
	else if(uiAction == GOSSIP_ACTION_INFO_DEF + 1)
	{
        pPlayer->CLOSE_GOSSIP_MENU();

		if (boss_major_ragAI* pMajoAI = static_cast<boss_major_ragAI*>(pCreature->AI()))
                pMajoAI->StartSummonEventInstant(pPlayer);
	}

    return true;
}

void AddSC_boss_major_rag()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_major_rag";
    newscript->GetAI = &GetAI_boss_major_rag;
    newscript->pGossipHello = &GossipHello_boss_major_rag;
    newscript->pGossipSelect = &GossipSelect_boss_major_rag;
    newscript->RegisterSelf();
}