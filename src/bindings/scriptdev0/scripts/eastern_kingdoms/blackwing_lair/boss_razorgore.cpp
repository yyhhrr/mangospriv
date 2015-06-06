/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Razorgore
SD%Complete: 95
SDComment: Timers may be improved.
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"
#include "../system/system.h"
#include "../system/ProgressMonitor.h"

enum
{
	ENTRY_RAZORGORE				= 12434,

    SAY_EGGS_BROKEN_1           = -1469022,
    SAY_EGGS_BROKEN_2           = -1469023,
    SAY_EGGS_BROKEN_3           = -1469024,
    SAY_DEATH                   = -1469025,

    SPELL_POSSESS               = 23014,                    // visual effect and increase the damage taken
    SPELL_DESTROY_EGG           = 19873,
    SPELL_EXPLODE_ORB           = 20037,                    // used if attacked without destroying the eggs - triggers 20038

    SPELL_CLEAVE                = 19632,
    SPELL_WARSTOMP              = 24375,
    SPELL_FIREBALL_VOLLEY       = 22425,
    SPELL_CONFLAGRATION         = 23023,
    SPELL_WARMING_FLAMES		= 23040,
    // Grethok
    SPELL_ARCANE_MISSILES		= 22273,
    SPELL_DOMINATE_MIND			= 14515,
    SPELL_GREATER_POLYMORPH		= 22274,
    SPELL_SLOW					= 13747,
};

struct MANGOS_DLL_DECL boss_razorgoreAI : public ScriptedAI
{
    boss_razorgoreAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_blackwing_lair*)pCreature->GetInstanceData();
        Reset();
    }

    instance_blackwing_lair* m_pInstance;

	uint32 m_uiCheckControllerAggroTimer;
    uint32 m_uiIntroVisualTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiWarStompTimer;
    uint32 m_uiFireballVolleyTimer;
    uint32 m_uiConflagrationTimer;
	uint32 m_fTargetThreat;
	uint32 m_uiAggroTimer;

	Unit* m_MostHated;

    bool m_bEggsExploded;

	ObjectGuid m_uiControllerGUID;

    void Reset() override
    {
		m_MostHated = nullptr;
		m_fTargetThreat					= 0;
		m_uiAggroTimer					= 0;
        m_uiIntroVisualTimer			= 5000;
        m_bEggsExploded					= false;
		
        m_uiCleaveTimer					= urand(8000, 10000);
        m_uiWarStompTimer				= 30000;
        m_uiConflagrationTimer			= urand(20000, 25000);
        m_uiFireballVolleyTimer			= 8000;
		m_uiCheckControllerAggroTimer	= 3000;

		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.20f);

		m_uiControllerGUID.Clear();
	}

    void JustDied(Unit* killer) override
    {
        if (m_pInstance)
        {
            // Don't set instance data unless all eggs are destroyed
            if (m_pInstance->GetData(TYPE_RAZORGORE) != SPECIAL)
                m_pInstance->SetData(TYPE_RAZORGORE, FAIL);
			else
                m_pInstance->SetData(TYPE_RAZORGORE, DONE);
        }

		DoScriptText(SAY_DEATH, m_creature);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::BlackWingLair, Bosses::Razorgore);
    }

    void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage) override
    {
        if (uiDamage < m_creature->GetHealth())
            return;

        if (!m_pInstance)
            return;

        // Don't allow any accident
        if (m_bEggsExploded)
        {
            uiDamage = 0;
            return;
        }

        // Boss explodes everything and resets - this happens if not all eggs are destroyed
        if (m_pInstance->GetData(TYPE_RAZORGORE) == IN_PROGRESS)
        {
            uiDamage = 0;
            m_bEggsExploded = true;
			DoCastSpellIfCan(m_creature, SPELL_EXPLODE_ORB, CAST_TRIGGERED);
			m_pInstance->KillAllPlayersInRazorgoreRoom();

			m_creature->ForcedDespawn();
            m_pInstance->SetData(TYPE_RAZORGORE, FAIL);
        }
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORGORE, FAIL);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        // Defenders should attack the players and the boss
        pSummoned->SetInCombatWithZone();
        pSummoned->AI()->AttackStart(m_creature);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id != 19832 || pCaster->GetTypeId() != TYPEID_PLAYER)
            return;

        // Drop threat for previous controller
        if (Player* pPrevController = m_creature->GetMap()->GetPlayer(m_uiControllerGUID))
		{
            if (m_creature->getThreatManager().getThreat(pPrevController))
                m_creature->getThreatManager().modifyThreatPercent(pPrevController,-100);

			pPrevController->CastStop();
			pPrevController->InterruptNonMeleeSpells(true);
		}

        m_creature->AddThreat(pCaster, 10000.0f);
        m_uiControllerGUID = ((Player*)pCaster)->GetObjectGuid();
	}

    void CheckControllerAggro(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        Player* pPlayer = (Player*)pVictim;

        if (pPlayer->GetObjectGuid() == m_uiControllerGUID)
            return;

        if (Player* pController = m_creature->GetMap()->GetPlayer(m_uiControllerGUID))
        {
            float m_fThreat = m_creature->getThreatManager().getThreat(pPlayer);
            m_creature->getThreatManager().addThreat(pController, m_fThreat*10);
            m_creature->TauntApply(pController);
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
        // If last controller is alive he should always have aggro until he dies
        if (m_uiControllerGUID)
        {
            if (m_uiCheckControllerAggroTimer <= uiDiff)
            {
                CheckControllerAggro(m_creature->getVictim());
                m_uiCheckControllerAggroTimer = 3000;
            }
            else
                m_uiCheckControllerAggroTimer -= uiDiff;
        }

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(5000, 7000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // War Stomp
        if (m_uiWarStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WARSTOMP) == CAST_OK)
                m_uiWarStompTimer = 30000;
        }
        else
            m_uiWarStompTimer -= uiDiff;

        // Fireball Volley
        if (m_uiFireballVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FIREBALL_VOLLEY) == CAST_OK)
                m_uiFireballVolleyTimer = urand(10000, 12000);
        }
        else
            m_uiFireballVolleyTimer -= uiDiff;
		
        // Conflagration
        if (m_uiConflagrationTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CONFLAGRATION) == CAST_OK)
                m_uiConflagrationTimer = 13000;
        }
        else
            m_uiConflagrationTimer -= uiDiff;
		
         if (!m_MostHated && m_creature->getVictim()->HasAura(SPELL_CONFLAGRATION, EFFECT_INDEX_0))
         {
			 m_MostHated = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0);

			 m_fTargetThreat = m_creature->getThreatManager().getThreat(m_MostHated);
			 m_creature->getThreatManager().modifyThreatPercent(m_MostHated, -100);

             if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1))
                 m_creature->TauntApply(pTarget);
         }

		 if (m_MostHated && !m_MostHated->HasAura(SPELL_CONFLAGRATION, EFFECT_INDEX_0))
         {
			m_creature->getThreatManager().addThreatDirectly(m_MostHated, m_fTargetThreat);
			m_MostHated = nullptr;
		 }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_razorgore(Creature* pCreature)
{
    return new boss_razorgoreAI(pCreature);
}

bool EffectDummyGameObj_go_black_dragon_egg(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, GameObject* pGOTarget)
{
    if (uiSpellId == SPELL_DESTROY_EGG && uiEffIndex == EFFECT_INDEX_1)
    {
        if (!pGOTarget->isSpawned())
            return true;

        if (ScriptedInstance* pInstance = (ScriptedInstance*)pGOTarget->GetInstanceData())
        {
            if (urand(0, 1))
            {
                switch (urand(0, 2))
                {
                    case 0: DoScriptText(SAY_EGGS_BROKEN_1, pCaster); break;
                    case 1: DoScriptText(SAY_EGGS_BROKEN_2, pCaster); break;
                    case 2: DoScriptText(SAY_EGGS_BROKEN_3, pCaster); break;
                }
            }

			if(Creature* razorgore = pInstance->GetSingleCreatureFromStorage(ENTRY_RAZORGORE))
			{
				auto* victim = razorgore->getVictim();
				if(victim)
				{
					auto threatMgr = razorgore->getThreatManager();
					float newThread = threatMgr.getThreat(victim) + 1000;
					razorgore->getThreatManager().addThreat(pCaster, newThread);
				}
				
			}

			pInstance->SetData64(DATA_DRAGON_EGG, pGOTarget->GetObjectGuid());
        }

        return true;
    }

    return false;
}

struct MANGOS_DLL_DECL mob_grethok_the_controllerAI : public ScriptedAI
{
    mob_grethok_the_controllerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_blackwing_lair*)pCreature->GetInstanceData();
        Reset();
    }

    instance_blackwing_lair* m_pInstance;

    uint32 m_uiArcaneMisslesTimer;
    uint32 m_uiDominateMindTimer;
    uint32 m_uiGreaterPolymorphTimer;
    uint32 m_uiSlowTimer;
	uint32 m_uiIntroVisualTimer;

    void Reset()
    {
        m_uiArcaneMisslesTimer		= 2000;
        m_uiDominateMindTimer		= 8000;
        m_uiGreaterPolymorphTimer	= 10000;
        m_uiSlowTimer				= 15000;
		m_uiIntroVisualTimer		= 1000;

		DoCastSpellIfCan(m_creature, SPELL_POSSESS, 0, m_creature->GetGUID());
    }

    void Aggro(Unit* pWho)
    {
		m_creature->MonsterYell("Eindringlinge versuchen die Brut zu verletzen! Schl\303\244gt Alarm! Sch\303\274tzt die Eier um jeden Preis!", LANG_UNIVERSAL, 0);
		m_creature->CastStop();
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

        // Arcane Missiles
        if (m_uiArcaneMisslesTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_MISSILES) == CAST_OK)
                m_uiArcaneMisslesTimer = urand(7000, 10000);
        }
        else
            m_uiArcaneMisslesTimer -= uiDiff;

        // Dominate Mind
        if (m_uiDominateMindTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
			if (DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_DOMINATE_MIND) == CAST_OK)
			{
				m_creature->MonsterYell("Sterbliche Schwachk\303\266pfe! Nun dient Ihr mir!", LANG_UNIVERSAL, 0);
				m_uiDominateMindTimer = urand(15000, 20000);
			}  
        }
        else
            m_uiDominateMindTimer -= uiDiff;

        // Greater Polymorph
        if (m_uiGreaterPolymorphTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_GREATER_POLYMORPH) == CAST_OK)
                m_uiGreaterPolymorphTimer = urand(10000, 15000);
        }
        else
            m_uiGreaterPolymorphTimer -= uiDiff;

        // Slow
        if (m_uiSlowTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SLOW) == CAST_OK)
                m_uiSlowTimer = urand(8000, 15000);
        }
        else
            m_uiSlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_grethok_the_controller(Creature* pCreature)
{
    return new mob_grethok_the_controllerAI(pCreature);
}

/*####
## go_orb_of_domination
####*/

bool GOUse_go_orb_of_domination(Player* pPlayer, GameObject* pGo)
{
    instance_blackwing_lair* m_pInstance = (instance_blackwing_lair*)pGo->GetInstanceData();

    if (m_pInstance)
    {
        if (m_pInstance->GetData(TYPE_RAZORGORE) != IN_PROGRESS)
            return true;

		if (pPlayer->HasAura(23958, EFFECT_INDEX_0))
			return true;

		if (!pPlayer->HasAura(23958, EFFECT_INDEX_0))
		{
			if (Creature* pRazorgore = m_pInstance->GetSingleCreatureFromStorage(NPC_RAZORGORE))
				pRazorgore->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.30f);

			pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
			pPlayer->CastSpell(pPlayer, 23958, true);
			//pPlayer->CastSpell(pPlayer, 23018, true); //23958
			pPlayer->CastSpell(pPlayer, 19832, true);
			pPlayer->CastSpell(pPlayer, 23014, true);
		}
    }

    return true;
}

struct MANGOS_DLL_DECL mob_trigger_razorgoreAI : public ScriptedAI
{
	mob_trigger_razorgoreAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (instance_blackwing_lair*)pCreature->GetInstanceData();
		Reset();
	}

	instance_blackwing_lair* m_pInstance;

	void Reset()
	{
		m_creature->setFaction(35);
		m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		DoMeleeAttackIfReady();
	}
};

bool GossipHello_mob_trigger_razorgore(Player* pPlayer, Creature* pCreature)
{
	ScriptedInstance* pInstance;
	pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

	if (pPlayer->GetSession()->GetSecurity() > SEC_GAMEMASTER)
	{
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<Dev> Razorgore Phase 2", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Razorgore Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Vealastrasz Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);


			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Lashlayer Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);


			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Firemaw Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Ebonroc Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);


			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Flamegor Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Chromaggus Done", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
	}

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
	return true;
}

bool GossipSelect_mob_trigger_razorgore(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	instance_blackwing_lair* m_pInstance;
	m_pInstance = (instance_blackwing_lair*)pCreature->GetInstanceData();
	std::string playername = pPlayer->GetName();

	if (uiAction == GOSSIP_ACTION_INFO_DEF)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_RAZORGORE, SPECIAL);

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'Razorgore Phase 2')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}
	
	if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
		{
			m_pInstance->SetData(TYPE_RAZORGORE, DONE);
			m_pInstance->DespawnEggs();
		}

		if (Creature* pRazorgore = m_pInstance->GetSingleCreatureFromStorage(NPC_RAZORGORE))
			pRazorgore->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'Razorgore Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 2)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_VAELASTRASZ, DONE);

		if (Creature* pVealastrasz = m_pInstance->GetSingleCreatureFromStorage(NPC_VAELASTRASZ))
			pVealastrasz->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'VAELASTRASZ Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 3)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_LASHLAYER, DONE);

		if (Creature* pLashlayer = m_pInstance->GetSingleCreatureFromStorage(NPC_LASHLAYER))
			pLashlayer->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'LASHLAYER Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 4)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_FIREMAW, DONE);

		if (Creature* pFiremaw = m_pInstance->GetSingleCreatureFromStorage(NPC_FIREMAW))
			pFiremaw->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'FIREMAW Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 5)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_EBONROC, DONE);

		if (Creature* Ebonroc = m_pInstance->GetSingleCreatureFromStorage(NPC_EBONROC))
			Ebonroc->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'EBONROC Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 6)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_FLAMEGOR, DONE);

		if (Creature* pFlamegor = m_pInstance->GetSingleCreatureFromStorage(NPC_FLAMEGOR))
			pFlamegor->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'FLAMEGOR Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 7)
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_CHROMAGGUS, DONE);

		if (Creature* pChromaggus = m_pInstance->GetSingleCreatureFromStorage(NPC_CHROMAGGUS))
			pChromaggus->ForcedDespawn();

		SD0Database.PExecute("INSERT INTO gamemaster_menu (id, charid, accountid, date, menu) VALUES ('0', '%u', '%u', CURRENT_TIMESTAMP, 'CHROMAGGUS Done')", pPlayer->GetGUID(), pPlayer->GetAccountId());
	}

	return true;
}

CreatureAI* GetAI_mob_trigger_razorgore(Creature* pCreature)
{
	return new mob_trigger_razorgoreAI(pCreature);
}

void AddSC_boss_razorgore()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_razorgore";
    pNewScript->GetAI = &GetAI_boss_razorgore;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_grethok_the_controller";
    pNewScript->GetAI = &GetAI_mob_grethok_the_controller;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_black_dragon_egg";
    pNewScript->pEffectDummyGO = &EffectDummyGameObj_go_black_dragon_egg;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_orb_of_domination";
    pNewScript->pGOUse = &GOUse_go_orb_of_domination;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_trigger_razorgore";
	pNewScript->GetAI = &GetAI_mob_trigger_razorgore;
	pNewScript->pGossipHello = &GossipHello_mob_trigger_razorgore;
	pNewScript->pGossipSelect = &GossipSelect_mob_trigger_razorgore;
	pNewScript->RegisterSelf();
}
