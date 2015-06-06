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

ScriptName: Zul'Gurub Opening Event
*/

#include "precompiled.h"

enum
{
	//Hexendoktor
	SPELL_SHADOWVOLLEY	= 20741,

	//Trolle
	SPELL_SUMMONEFFECT	= 19484,

	NPC_HEXENDOKTOR		= 800052,
	NPC_PORTAL			= 800053,
	NPC_TROLLE			= 800054,

	NPC_PORTAL_DUMMY1	= 800055,
	NPC_PORTAL_DUMMY2	= 800056
};

static const float TrollWaypoints[1][3] =
{
	{ 1360.06f, -4373.35f, 26.1009f }
};

struct MANGOS_DLL_DECL mob_zg_opening_hexendoktorAI : public ScriptedAI
{
	mob_zg_opening_hexendoktorAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_creature->addUnitState(UNIT_STAT_ROOT);
		Reset();
	}

	int pPortalSpawns;

	bool PortalSpawned;

	uint32 m_uiSpawnPortalTimer;
	uint32 m_uiShadowVolleyTimer;

	void Reset()
	{
		m_uiSpawnPortalTimer = 5000;
		m_uiShadowVolleyTimer = 10000;
		pPortalSpawns = 0;

		PortalSpawned = false;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (!PortalSpawned && m_uiSpawnPortalTimer <= uiDiff)
		{
			if (Creature* pDummy1 = GetClosestCreatureWithEntry(m_creature, NPC_PORTAL_DUMMY1, 200.0f))
			{
				float pDummyX1 = pDummy1->GetPositionX();
				float pDummyY1 = pDummy1->GetPositionY();
				float pDummyZ1 = pDummy1->GetPositionZ();

				m_creature->SummonCreature(NPC_PORTAL, pDummyX1, pDummyY1, pDummyZ1, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
			}

			if (Creature* pDummy2 = GetClosestCreatureWithEntry(m_creature, NPC_PORTAL_DUMMY2, 200.0f))
			{
				float pDummyX2 = pDummy2->GetPositionX();
				float pDummyY2 = pDummy2->GetPositionY();
				float pDummyZ2 = pDummy2->GetPositionZ();

				m_creature->SummonCreature(NPC_PORTAL, pDummyX2, pDummyY2, pDummyZ2, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
			}

			PortalSpawned = true;
			m_uiSpawnPortalTimer = 0;
		}
		else
			m_uiSpawnPortalTimer -= uiDiff;

		if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
		{
			if (m_uiShadowVolleyTimer < uiDiff)
			{
				if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWVOLLEY) == CAST_OK)
					m_uiShadowVolleyTimer = urand(4000, 8000);
			}
			else
				m_uiShadowVolleyTimer -= uiDiff;
		}
		else
		{
			if (m_uiShadowVolleyTimer < uiDiff)
			{
				if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWVOLLEY) == CAST_OK)
					m_uiShadowVolleyTimer = 1000;
			}
			else
				m_uiShadowVolleyTimer -= uiDiff;
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_zg_opening_hexendoktor(Creature* pCreature)
{
	return new mob_zg_opening_hexendoktorAI(pCreature);
}

struct MANGOS_DLL_DECL mob_zg_opening_portalAI : public ScriptedAI
{
	mob_zg_opening_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_creature->addUnitState(UNIT_STAT_ROOT);
		m_creature->addUnitState(UNIT_STAT_CAN_NOT_MOVE);
		Reset();
	}

	uint32 m_uiSpawnTrollTimer;
	uint32 m_uiDespawnSelfTimer;

	int pTrollSpawns = 0;
	int pMap = m_creature->GetMapId();

	void Reset()
	{
		m_uiSpawnTrollTimer = 5000;
		pTrollSpawns = 0;
	}

	void JustSummoned(Creature* pSummoned)
	{
		pSummoned->CastSpell(pSummoned, SPELL_SUMMONEFFECT, false);

		if (Unit* pTarget = pSummoned->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			pSummoned->AI()->AttackStart(pTarget);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (Creature* pHexer = GetClosestCreatureWithEntry(m_creature, NPC_HEXENDOKTOR, 200.0f))
		{
			if (!pHexer->isAlive())
				pHexer->ForcedDespawn();
		}

		if (m_uiSpawnTrollTimer <= uiDiff)
		{
			if (pTrollSpawns <= 15)
			{
				m_creature->SummonCreature(NPC_TROLLE, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
				m_uiSpawnTrollTimer = 8000;
				pTrollSpawns++;
			}
		}
		else
			m_uiSpawnTrollTimer -= uiDiff;
	}
};

CreatureAI* GetAI_mob_zg_opening_portal(Creature* pCreature)
{
	return new mob_zg_opening_portalAI(pCreature);
}

struct MANGOS_DLL_DECL mob_zg_opening_trollAI : public ScriptedAI
{
	mob_zg_opening_trollAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiRainOfFireTimer;

	void Reset()
	{
		m_uiRainOfFireTimer = urand(5000, 8000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_zg_opening_troll(Creature* pCreature)
{
	return new mob_zg_opening_trollAI(pCreature);
}

enum ZulFarrak
{
	QUEST_DIE_GLEICHE_SUCHE_NEUE_SPUREN_HORDE	= 20011,
	QUEST_DIE_GLEICHE_SUCHE_NEUE_SPUREN_ALLIANZ = 20012,
	GO_TROLLCAGE								= 800001,
};

bool GossipHello_mob_zg_opening_zulfarrak_troll(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetQuestStatus(QUEST_DIE_GLEICHE_SUCHE_NEUE_SPUREN_HORDE) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(QUEST_DIE_GLEICHE_SUCHE_NEUE_SPUREN_ALLIANZ) == QUEST_STATUS_INCOMPLETE)
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, "Wisst ihr etwas \303\274ber einen Blutgott namens Hakkar?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	else
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_mob_zg_opening_zulfarrak_troll(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
	GameObject* pCage;
	pCage = GetClosestGameObjectWithEntry(pCreature, GO_TROLLCAGE, 3.0f);

	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, "Dann werden wir Euch auch nicht befreien.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		pPlayer->SEND_GOSSIP_MENU(90034, pCreature->GetObjectGuid());
		break;
	case GOSSIP_ACTION_INFO_DEF + 2:
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, "Versprochen", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		pPlayer->SEND_GOSSIP_MENU(90035, pCreature->GetObjectGuid());
		break;
	case GOSSIP_ACTION_INFO_DEF + 3:
		pPlayer->SEND_GOSSIP_MENU(90036, pCreature->GetObjectGuid());
		pPlayer->AreaExploredOrEventHappens(QUEST_DIE_GLEICHE_SUCHE_NEUE_SPUREN_HORDE);
		pPlayer->AreaExploredOrEventHappens(QUEST_DIE_GLEICHE_SUCHE_NEUE_SPUREN_ALLIANZ);
		pCage->SetGoState(GO_STATE_ACTIVE);
		break;
	}
	return true;
}

enum Tabetha
{
	QUEST_HAKKAR_DER_SEELENSCHINDER_HORDE	= 20013,
	QUEST_HAKKAR_DER_SEELENSCHINDER_ALLIANZ	= 20014,
};

bool GossipHello_mob_zg_opening_tabetha(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetQuestStatus(QUEST_HAKKAR_DER_SEELENSCHINDER_HORDE) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(QUEST_HAKKAR_DER_SEELENSCHINDER_ALLIANZ) == QUEST_STATUS_INCOMPLETE)
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, "Ich h\303\266re euch zu.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	else
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_mob_zg_opening_tabetha(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, "Weiter", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		pPlayer->SEND_GOSSIP_MENU(90037, pCreature->GetObjectGuid());
		break;
	case GOSSIP_ACTION_INFO_DEF + 2:
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, "Verstehe", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		pPlayer->SEND_GOSSIP_MENU(90038, pCreature->GetObjectGuid());
		break;
	case GOSSIP_ACTION_INFO_DEF + 3:
		pPlayer->SEND_GOSSIP_MENU(90039, pCreature->GetObjectGuid());
		pPlayer->AreaExploredOrEventHappens(QUEST_HAKKAR_DER_SEELENSCHINDER_HORDE);
		pPlayer->AreaExploredOrEventHappens(QUEST_HAKKAR_DER_SEELENSCHINDER_ALLIANZ);
		break;
	}
	return true;
}

enum Beschwoerer
{
	SPELL_SHADOW_VOLLEY = 21341,
	SPELL_SLEEP			= 24664,
	SPELL_RAIN_OF_FIRE	= 19717,
	SPELL_DARK_AURA		= 21157
};

struct MANGOS_DLL_DECL mob_zg_opening_beschwoererAI : public ScriptedAI
{
	mob_zg_opening_beschwoererAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiShadowVolleyTimer;
	uint32 m_uiSleepTimer;
	uint32 m_uiRainOfFireTimer;

	void Reset()
	{
		m_uiShadowVolleyTimer = urand(4000, 6000);
		m_uiSleepTimer = 8000;
		m_uiRainOfFireTimer = urand(8000, 12000);

		DoCastSpellIfCan(m_creature, SPELL_DARK_AURA);
	}

	void Aggro(Unit* pWho)
	{
		m_creature->MonsterYell("Ihr werdet uns niemals aufhalten!", LANG_UNIVERSAL, 0);
	}

	void JustDied(Unit* pKiller)
	{
		m_creature->MonsterYell("Ihr kommt zu sp\303\244t, Sterbliche... Hakkar kann nicht mehr aufgehalten... werden... Unsere Truppen... sind... bereits... auf dem Weg...", LANG_UNIVERSAL, 0);
	}

	void JustReachedHome()
	{
		DoCastSpellIfCan(m_creature, SPELL_DARK_AURA);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShadowVolleyTimer <= uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_VOLLEY);
			m_uiShadowVolleyTimer = 8000;
		}
		else
			m_uiShadowVolleyTimer -= uiDiff;

		if (m_uiSleepTimer <= uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				DoCastSpellIfCan(pTarget, SPELL_SLEEP);
				m_uiSleepTimer = 12000;
			}
		}
		else
			m_uiSleepTimer -= uiDiff;

		if (m_uiRainOfFireTimer <= uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				DoCastSpellIfCan(pTarget, SPELL_RAIN_OF_FIRE);
				m_uiRainOfFireTimer = 10000;
			}
		}
		else
			m_uiRainOfFireTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_zg_opening_beschwoerer(Creature* pCreature)
{
	return new mob_zg_opening_beschwoererAI(pCreature);
}

void AddSC_zulgurub_opening()
{
	Script* pNewScript;
	pNewScript = new Script;
	pNewScript->Name = "mob_zg_opening_hexendoktor";
	pNewScript->GetAI = &GetAI_mob_zg_opening_hexendoktor;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_zg_opening_portal";
	pNewScript->GetAI = &GetAI_mob_zg_opening_portal;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_zg_opening_troll";
	pNewScript->GetAI = &GetAI_mob_zg_opening_troll;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_zg_opening_zulfarrak_troll";
	pNewScript->pGossipHello = &GossipHello_mob_zg_opening_zulfarrak_troll;
	pNewScript->pGossipSelect = &GossipSelect_mob_zg_opening_zulfarrak_troll;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_zg_opening_tabetha";
	pNewScript->pGossipHello = &GossipHello_mob_zg_opening_tabetha;
	pNewScript->pGossipSelect = &GossipSelect_mob_zg_opening_tabetha;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_zg_opening_beschwoerer";
	pNewScript->GetAI = &GetAI_mob_zg_opening_beschwoerer;
	pNewScript->RegisterSelf();
}
