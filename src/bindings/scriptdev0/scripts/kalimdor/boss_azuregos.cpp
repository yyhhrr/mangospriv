/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Azuregos
SD%Complete: 90
SDComment: spell reflect not effecting dots (Core problem)
SDCategory: Azshara
EndScriptData */

#include "precompiled.h"
#include "../system/ProgressMonitor.h"

enum eAzuregos
{
    SAY_TELEPORT                = -1000100,

    SPELL_ARCANE_VACUUM         = 21147,
    SPELL_CLEAVE                = 19983,    //8255,
    SPELL_FROST_BREATH          = 21099,
    SPELL_CHILL                 = 21098,
    SPELL_MANASTORM             = 21097,
    SPELL_MARK_OF_FROST_DRAKE   = 23184,
    SPELL_MARK_OF_FROST_PLAYER  = 23182,
	SPELL_AURA_OF_FROST			= 23186,
    SPELL_REFLECTION            = 22067,
    SPELL_ENRAGE                = 23537,
	SPELL_TAILSWEEP             = 15847,
};

struct MANGOS_DLL_DECL boss_azuregosAI : public ScriptedAI
{
    boss_azuregosAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    bool m_bEnraged;

    uint32 m_uiCleaveTimer;
    uint32 m_uiFrostBreathTimer;
    uint32 m_uiChillTimer;
    uint32 m_uiManastormTimer;
    uint32 m_uiReflectionTimer;
    uint32 m_uiTeleportTimer;

    void Reset()
    {
        m_bEnraged = false;

        m_uiCleaveTimer			= 7000;
        m_uiFrostBreathTimer	= urand(5000, 8000);
        m_uiChillTimer			= urand(10000, 30000);
        m_uiManastormTimer		= urand(5000, 17000);
        m_uiReflectionTimer		= urand(15000, 30000);
        m_uiTeleportTimer		= 30000;

        m_creature->RemoveAurasDueToSpell(SPELL_MARK_OF_FROST_DRAKE);

        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ARCANE, true);
    }

    void Aggro(Unit* /*pWho*/)
    {
        //DoCastSpellIfCan(m_creature, SPELL_MARK_OF_FROST_DRAKE, CAST_TRIGGERED);
    }
	
    void KilledUnit(Unit* pVictim)
    {
        pVictim->CastSpell(pVictim, SPELL_MARK_OF_FROST_PLAYER, true);
    }

    void JustDied(Unit* killer)
    {
		uint32 m_uiRandomRespawnTime;
		auto rndGlobal = urand(1, 100);

		// Mind. 2 Tage Respawnzeit
		if (rndGlobal <= 30)
		{
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
		}
		// Mind. 3 Tage Respawnzeit
		else if (rndGlobal <= 60 && rndGlobal > 30)
		{
			auto rndDayHours = urand(1, 10);
			if (rndDayHours <= 2)
			{
				m_uiRandomRespawnTime = 259200;				// 3 Tage
			}
			else if (rndDayHours <= 6 && rndDayHours > 2)
			{
				auto rndHours = urand(262800, 302400);		// 3 Tage + 1 bis 12 Stunden
				m_uiRandomRespawnTime = rndHours;
			}
			else
			{
				auto rndHours = urand(306000, 342000);		// 3 Tage + 13 bis 23 Stunden
				m_uiRandomRespawnTime = rndHours;
			}
		}
		// Ansonsten 4 Tage Respawnzeit
		else {
			m_uiRandomRespawnTime = 345600;					// 4 Tage
		}

		m_creature->SetRespawnDelay(m_uiRandomRespawnTime);
		m_creature->SaveToDB();

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::Worldbosses, Bosses::Azuregos);
    }

    void EnterEvadeMode()
    {
		m_creature->SetHealthPercent(95);
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        if (m_creature->isAlive())
            m_creature->GetMotionMaster()->MoveTargetedHome();

        Reset();
    }

	void CheckForMarkedPlayers()
	{
        if (m_creature->isAlive())
        {
			ThreatList const& tList = m_creature->getThreatManager().getThreatList();
			for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
			{
				Player* pPlayer = m_creature->GetMap()->GetPlayer((*itr)->getUnitGuid());
				
                if (pPlayer && !pPlayer->isGameMaster() && pPlayer->isAlive() && pPlayer->HasAura(SPELL_MARK_OF_FROST_PLAYER) && !pPlayer->HasAura(SPELL_AURA_OF_FROST)
					&& pPlayer->IsWithinDist(m_creature, 55.f))
                    pPlayer->CastSpell(pPlayer, SPELL_AURA_OF_FROST, true);
			}
        }
	}

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		CheckForMarkedPlayers();

        // Enrage
        if (!m_bEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            m_bEnraged = true;
        }

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 5000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Frost Breath
        if (m_uiFrostBreathTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_BREATH);
            m_uiFrostBreathTimer = urand(8000, 15000);
        }
        else
            m_uiFrostBreathTimer -= uiDiff;

        // Chill
        if (m_uiChillTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHILL);
            m_uiChillTimer = urand(13000, 25000);
        }
        else
            m_uiChillTimer -= uiDiff;

        // Manastorm
        if (m_uiManastormTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(pTarget, SPELL_MANASTORM);

            m_uiManastormTimer = urand(7500, 12500);
        }
        else
            m_uiManastormTimer -= uiDiff;

        // Reflection
        if (m_uiReflectionTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTION);
            m_uiReflectionTimer = urand(20000, 30000);
        }
        else
            m_uiReflectionTimer -= uiDiff;

        // Teleport
        if (m_uiTeleportTimer < uiDiff)
        {
            DoScriptText(SAY_TELEPORT, m_creature);
			
		    float fX, fY, fZ;
            m_creature->GetPosition(fX, fY, fZ);

            std::vector<ObjectGuid> vGuids;
            m_creature->FillGuidsListFromThreatList(vGuids);

            for (std::vector<ObjectGuid>::const_iterator itr = vGuids.begin(); itr != vGuids.end(); ++itr)
            {
                Unit* pUnit = m_creature->GetMap()->GetUnit(*itr);

				if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                    DoTeleportPlayer(pUnit, fX, fY, fZ+5, pUnit->GetOrientation());
            }

            DoResetThreat();
            m_uiTeleportTimer = 30000;
        }
        else
            m_uiTeleportTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_azuregos(Creature* pCreature)
{
    return new boss_azuregosAI(pCreature);
}

void AddSC_boss_azuregos()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_azuregos";
    pNewscript->GetAI = &GetAI_boss_azuregos;
    pNewscript->RegisterSelf();
}