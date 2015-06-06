//geil

#include "precompiled.h"

int m_addCount;

enum General
{
	MAX_ADD_COUNT = 5,
	MAX_BOSS_COUNT = 1
};

enum Elementar_Trigger
{
	NPC_WASSER_TRIGGER	= 800027,	// Wasser
	NPC_ERDE_TRIGGER	= 800028,	// Erde
	NPC_WIND_TRIGGER	= 800029,	// Wind
	NPC_FEUER_TRIGGER	= 800030	// Feuer
};

enum Elementar_Bosse
{
	NPC_TEMPESTRIA		= 14457,	// Wasser
	NPC_LAVINIUS		= 14464,	// Erde
	NPC_WINDHAESCHER	= 14454,	// Wind
	NPC_GLUTARR			= 14461		// Feuer
};

enum Elementar_Adds
{
	NPC_WAESSRIGER		= 14458,	// Wasser
	NPC_DONNERNDER		= 14462,	// Erde
	NPC_WIRBELNDER		= 14455,	// Wind
	NPC_LODERNER		= 14460		// Feuer
};

enum Elementar_Risse
{
	GO_WASSERRISS_PORTAL	= 179665,	// Wasser
	GO_WASSERRISS			= 181650,	// Wasser
	GO_ERDRISS_PORTAL		= 179664,	// Erde
	GO_ERDRISS				= 123456,	// Erde - ID fehlt irgendwie oder es gab dazu keinen Riss.
	GO_LUFTRISS_PORTAL		= 179667,	// Wind
	GO_LUFTRISS				= 181651,	// Wind
	GO_FEUERRISS_PORTAL		= 179666,	// Feuer
	GO_FEUERISS				= 181652	// Feuer
};

enum SpellsTrigger
{
	SPELL_VISUAL_SPAWN	= 23014
};

struct MANGOS_DLL_DECL trigger_wasserAI : public ScriptedAI
{
	trigger_wasserAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	uint32 m_uiSpawnIntervall;
	uint32 m_uiSpawnAddTimer;
	uint32 m_uiSpawnBossTimer;

	int m_bossCount;
	int m_rissCount;
	bool m_addedRiss;

	void Reset()
	{
		m_uiSpawnAddTimer	= 5000;
		m_uiSpawnBossTimer	= 10000;

		m_addCount	= 0;
		m_bossCount	= 0;
		m_rissCount = 0;

		m_addedRiss = false;
	}

	void JustRespawned()
	{
		if (!m_addedRiss)
		{
			if (m_rissCount < 1)
				m_creature->SummonGameObject(GO_WASSERRISS_PORTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 3.0f, m_creature->GetOrientation(), m_creature->GetRespawnTime());

			for (int i = 0; i < 8; i++)
			{
				int coords = urand(0, 1);

				if (coords > 0)
				{
					int wertX = urand(1, 125);
					int wertY = urand(1, 125);
					int rndX = urand(m_creature->GetPositionX(), m_creature->GetPositionX() - wertX);
					int rndY = urand(m_creature->GetPositionY(), m_creature->GetPositionY() - wertY);
					//m_creature->SummonGameObject(GO_WASSERRISS, rndX, rndY, m_creature->GetPositionZ() + 1.0f, m_creature->GetOrientation(), m_creature->GetRespawnTime());
				}
				else
				{
					int wertX = urand(1, 125);
					int wertY = urand(1, 125);
					int rndX = urand(m_creature->GetPositionX(), m_creature->GetPositionX() + wertX);
					int rndY = urand(m_creature->GetPositionY(), m_creature->GetPositionY() + wertY);
					//m_creature->SummonGameObject(GO_WASSERRISS, rndX, rndY, m_creature->GetPositionZ() + 1.0f, m_creature->GetOrientation(), m_creature->GetRespawnTime());
				}

				m_rissCount++;
			}

			if (m_rissCount == 8)
				m_addedRiss = true;
		}
	}

	void JustDied(Unit* pKiller)
	{
		uint32 m_uiRandomRespawnTime;

		// Mind. 2 Tage Respawnzeit
		auto rndDayHours = urand(1, 10);
		if (rndDayHours <= 2)
		{
			m_uiRandomRespawnTime = 172800;				// 2 Tage
		}
		else if (rndDayHours <= 6 && rndDayHours > 2)
		{
			auto rndHours = urand(176400, 216000);		// 2 Tage + 1 bis 12 Stunden
			m_uiRandomRespawnTime = rndHours;
		}
		else
		{
			auto rndHours = urand(219600, 259200);		// 2 Tage + 13 bis 23 Stunden
			m_uiRandomRespawnTime = rndHours;
		}

		m_creature->SetRespawnDelay(m_uiRandomRespawnTime);
		m_creature->SaveToDB();
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_creature->isAlive() && !m_addedRiss)
		{
			if (m_rissCount < 1)
				m_creature->SummonGameObject(GO_WASSERRISS_PORTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 2.0f, m_creature->GetOrientation(), m_creature->GetRespawnTime());
			
			for (int i = 0; i < 8; i++)
			{
				int coords = urand(0, 1);

				if (coords > 0)
				{
					int wertX = urand(1, 125);
					int wertY = urand(1, 125);
					int rndX = urand(m_creature->GetPositionX(), m_creature->GetPositionX() - wertX);
					int rndY = urand(m_creature->GetPositionY(), m_creature->GetPositionY() - wertY);
					//m_creature->SummonGameObject(GO_WASSERRISS, rndX, rndY, m_creature->GetPositionZ() + 1.0f, m_creature->GetOrientation(), m_creature->GetRespawnTime());
				}
				else
				{
					int wertX = urand(1, 125);
					int wertY = urand(1, 125);
					int rndX = urand(m_creature->GetPositionX(), m_creature->GetPositionX() + wertX);
					int rndY = urand(m_creature->GetPositionY(), m_creature->GetPositionY() + wertY);
					//m_creature->SummonGameObject(GO_WASSERRISS, rndX, rndY, m_creature->GetPositionZ() + 1.0f, m_creature->GetOrientation(), m_creature->GetRespawnTime());
				}

				m_rissCount++;
			}

			if (m_rissCount == 8)
				m_addedRiss = true;
		}

		if (m_addCount < MAX_ADD_COUNT)
		{
			if (m_uiSpawnAddTimer < uiDiff)
			{
				Unit* pAdd = m_creature->SummonCreature(NPC_WAESSRIGER, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 20000);

				m_creature->InterruptNonMeleeSpells(true);

				if (pAdd)
					DoCastSpellIfCan(pAdd, SPELL_VISUAL_SPAWN, 0, m_creature->GetGUID());

				if (pAdd)
					pAdd->GetMotionMaster()->MoveRandomAroundPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 30.0f, 0);

				m_uiSpawnAddTimer = urand(15000, 30000);
				m_addCount++;
			}
			else
				m_uiSpawnAddTimer -= uiDiff;
		}

		if (m_bossCount < MAX_BOSS_COUNT)
		{
			if (m_uiSpawnBossTimer < uiDiff)
			{
				Unit* pBoss = GetClosestCreatureWithEntry(m_creature, NPC_TEMPESTRIA, VISIBLE_RANGE);

				if (pBoss)
				{
					pBoss->SetVisibility(VISIBILITY_ON);
					pBoss->setFaction(14);
					pBoss->GetMotionMaster()->MoveWaypoint();
				}

				m_uiSpawnBossTimer = 0;
				m_bossCount++;
			}
			else
				m_uiSpawnBossTimer -= uiDiff;
		}
	}
};

enum SpellsWasserBoss
{
	SPELL_CONE_OF_COLD	= 22746,
	SPELL_FROST_NOVA	= 14907,
	SPELL_FROSTBOLT		= 23102,
	SPELL_GEYSER		= 10987
};

struct MANGOS_DLL_DECL boss_wasserAI : public ScriptedAI
{
	boss_wasserAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	Unit* pAdd = GetClosestCreatureWithEntry(m_creature, NPC_WAESSRIGER, 15.0f);

	uint32 m_uiConeOfColdTimer;
	uint32 m_uiFrostNovaTimer;
	uint32 m_uiFrostboltTimer;
	uint32 m_uiGeyserTimer;

	uint32 m_uiSendTextTimer = 180000;

	void Reset()
	{
		m_uiConeOfColdTimer	= 5000;
		m_uiFrostNovaTimer	= 10000;
		m_uiFrostboltTimer	= 1000;
		m_uiGeyserTimer		= 15000;

		// Wenn der Trigger nicht am leben ist, ist der Boss und seine kleinen Adds nicht sichtbar. Die Abfrage ist wichtig, da bei einem Wipe immer Reset() aufgerufen wird und die Kreaturen despawnen würden.
		// Der Trigger kann nur sterben, wenn der Boss tot ist.
		if (Unit* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_WASSER_TRIGGER, VISIBLE_RANGE))
		{
			if (!pTrigger->isAlive())
			{
				m_creature->SetVisibility(VISIBILITY_OFF);
				m_creature->setFaction(35);
				m_creature->DeleteThreatList();
				m_creature->CombatStop();

				m_creature->GetMotionMaster()->Clear();
			}
		}

		m_creature->MonsterYell("This frozen land shall suffice for a start. Come to me, mortals - and I shall give you a quick and painful death.", LANG_UNIVERSAL, 0);
	}

	void JustDied(Unit* pKiller)
	{
		if (Unit* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_WASSER_TRIGGER, VISIBLE_RANGE))
			m_creature->DealDamage(pTrigger, 50000, NULL, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

		if (GameObject* pRiss = GetClosestGameObjectWithEntry(m_creature, GO_WASSERRISS_PORTAL, VISIBLE_RANGE))
			pRiss->RemoveFromWorld();
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_uiSendTextTimer < uiDiff)
		{
			m_creature->MonsterYell("If you mortals are all so craven, this will be much easier than I thought!", LANG_UNIVERSAL, 0);
			m_uiSendTextTimer = 360000;
		}
		else
			m_uiSendTextTimer -= uiDiff;

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiFrostboltTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOLT) == CAST_OK)
				m_uiFrostboltTimer = urand(2000, 4000);
		}
		else
			m_uiFrostboltTimer -= uiDiff;

		if (m_uiFrostNovaTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_NOVA) == CAST_OK)
				m_uiFrostNovaTimer = urand(10000, 15000);
		}
		else
			m_uiFrostNovaTimer -= uiDiff;

		if (m_uiConeOfColdTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONE_OF_COLD) == CAST_OK)
				m_uiConeOfColdTimer = urand(15000, 20000);
		}
		else
			m_uiConeOfColdTimer -= uiDiff;

		if (m_uiGeyserTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GEYSER) == CAST_OK)
				m_uiGeyserTimer = urand(20000, 30000);
		}
		else
			m_uiGeyserTimer -= uiDiff;
	}
};

enum SpellsWasserAdds
{
	SPELL_CHILLED		= 20005,
	SPELL_FROST_SHOCK	= 23115
};

struct MANGOS_DLL_DECL npc_wasserAI : public ScriptedAI
{
	npc_wasserAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	uint32 m_uiChilledTimer;
	uint32 m_uiFrostShockTimer;

	void Reset()
	{
		m_uiChilledTimer	= 5000;
		m_uiFrostShockTimer = 3000;

		m_creature->GetMotionMaster()->MoveRandomAroundPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 30.0f, 0);
	}

	void JustDied(Unit* pKiller)
	{
		m_addCount--;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiChilledTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHILLED) == CAST_OK)
				m_uiChilledTimer = urand(10000, 25000);
		}
		else
			m_uiChilledTimer -= uiDiff;

		if (m_uiFrostShockTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_SHOCK) == CAST_OK)
				m_uiFrostShockTimer = urand(8000, 15000);
		}
		else
			m_uiFrostShockTimer -= uiDiff;
	}
};

struct MANGOS_DLL_DECL trigger_erdeAI : public ScriptedAI
{
	trigger_erdeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	void Reset()
	{

	}

	void JustRespawned()
	{
		m_creature->SummonGameObject(GO_ERDRISS_PORTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), m_creature->GetRespawnTime());
	}

	void UpdateAI(const uint32)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
	}
};

struct MANGOS_DLL_DECL trigger_windAI : public ScriptedAI
{
	trigger_windAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	void Reset()
	{

	}

	void JustRespawned()
	{
		m_creature->SummonGameObject(GO_LUFTRISS_PORTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), m_creature->GetRespawnTime());
	}

	void UpdateAI(const uint32)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
	}
};

struct MANGOS_DLL_DECL trigger_feuerAI : public ScriptedAI
{
	trigger_feuerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	void Reset()
	{

	}

	void JustRespawned()
	{
		m_creature->SummonGameObject(GO_FEUERRISS_PORTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), m_creature->GetRespawnTime());
	}

	void UpdateAI(const uint32)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
	}
};


struct MANGOS_DLL_DECL boss_erdeAI : public ScriptedAI
{
	boss_erdeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	void Reset()
	{

	}

	void JustDied(Unit* pKiller)
	{
		Unit* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_ERDE_TRIGGER, VISIBLE_RANGE);
		GameObject* pRiss = GetClosestGameObjectWithEntry(m_creature, GO_ERDRISS_PORTAL, VISIBLE_RANGE);

		if (pTrigger)
			pTrigger->DealDamage(pTrigger, pTrigger->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

		if (pRiss)
			pRiss->RemoveFromWorld();
	}

	void UpdateAI(const uint32)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
	}
};

struct MANGOS_DLL_DECL boss_windAI : public ScriptedAI
{
	boss_windAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	void Reset()
	{

	}

	void JustDied(Unit* pKiller)
	{
		Unit* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_WIND_TRIGGER, VISIBLE_RANGE);
		GameObject* pRiss = GetClosestGameObjectWithEntry(m_creature, GO_LUFTRISS_PORTAL, VISIBLE_RANGE);

		if (pTrigger)
			pTrigger->DealDamage(pTrigger, pTrigger->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

		if (pRiss)
			pRiss->RemoveFromWorld();
	}

	void UpdateAI(const uint32)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
	}
};

struct MANGOS_DLL_DECL boss_feuerAI : public ScriptedAI
{
	boss_feuerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

	void Reset()
	{

	}

	void JustDied(Unit* pKiller)
	{
		Unit* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_FEUER_TRIGGER, VISIBLE_RANGE);
		GameObject* pRiss = GetClosestGameObjectWithEntry(m_creature, GO_FEUERRISS_PORTAL, VISIBLE_RANGE);

		if (pTrigger)
			pTrigger->DealDamage(pTrigger, pTrigger->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

		if (pRiss)
			pRiss->RemoveFromWorld();
	}

	void UpdateAI(const uint32)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
	}
};

CreatureAI* GetAI_trigger_wasser(Creature *_Creature)
{
	return new trigger_wasserAI(_Creature);
}

CreatureAI* GetAI_boss_wasser(Creature *_Creature)
{
	return new boss_wasserAI(_Creature);
}

CreatureAI* GetAI_npc_wasser(Creature *_Creature)
{
	return new npc_wasserAI(_Creature);
}

CreatureAI* GetAI_trigger_erde(Creature *_Creature)
{
	return new trigger_erdeAI(_Creature);
}

CreatureAI* GetAI_boss_erde(Creature *_Creature)
{
	return new boss_erdeAI(_Creature);
}

CreatureAI* GetAI_trigger_wind(Creature *_Creature)
{
	return new trigger_windAI(_Creature);
}

CreatureAI* GetAI_boss_wind(Creature *_Creature)
{
	return new boss_windAI(_Creature);
}

CreatureAI* GetAI_trigger_feuer(Creature *_Creature)
{
	return new trigger_feuerAI(_Creature);
}

CreatureAI* GetAI_boss_feuer(Creature *_Creature)
{
	return new boss_feuerAI(_Creature);
}

void AddSC_elementar_invension()
{
	Script* pNewscript;
	pNewscript = new Script;
	pNewscript->Name = "trigger_wasser_elementar";
	pNewscript->GetAI = &GetAI_trigger_wasser;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "boss_wasser_elementar";
	pNewscript->GetAI = &GetAI_boss_wasser;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "npc_wasser_elementar";
	pNewscript->GetAI = &GetAI_npc_wasser;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "trigger_erde_elementar";
	pNewscript->GetAI = &GetAI_trigger_erde;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "boss_erde_elementar";
	pNewscript->GetAI = &GetAI_boss_erde;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "trigger_wind_elementar";
	pNewscript->GetAI = &GetAI_trigger_wind;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "boss_wind_elementar";
	pNewscript->GetAI = &GetAI_boss_wind;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "trigger_feuer_elementar";
	pNewscript->GetAI = &GetAI_trigger_feuer;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "boss_feuer_elementar";
	pNewscript->GetAI = &GetAI_boss_feuer;
	pNewscript->RegisterSelf();
}