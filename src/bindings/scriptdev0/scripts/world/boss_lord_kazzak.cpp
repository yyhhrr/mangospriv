/* Special Thanks the Mangos-Zero, and Scriptdev2, and ScriptDev-Zero team!
 * Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Lord_Kazzak
SD%Complete: 70
SDComment: 
SDCategory: Dark-Portal
EndScriptData */

#include "precompiled.h"
#include "../system/ProgressMonitor.h"

#define SAY_INTRO                       -1000147
#define SAY_AGGRO1                      -1000148
#define SAY_AGGRO2                      -1000149
#define SAY_SURPREME1                   -1000150
#define SAY_SURPREME2                   -1000151
#define SAY_KILL1                       -1000152
#define SAY_KILL2                       -1000153
#define SAY_KILL3                       -1000154
#define SAY_DEATH                       -1000155
#define EMOTE_FRENZY                    -1000002
#define SAY_RAND1                       -1000157
#define SAY_RAND2                       -1000158

// New IDs according to wowd.org
#define SPELL_SHADOWVOLLEY              21341
#define SPELL_CLEAVE                    20691 //8255
#define SPELL_THUNDERCLAP               26554 //23931
#define SPELL_VOIDBOLT                  21066// 29317  //or 26006, standard 22709 is too weak
#define SPELL_MARKOFKAZZAK              21056

// What are these for??
#define SPELL_ENRAGE                    21340 //28747 
#define SPELL_CAPTURESOUL               21054
#define SPELL_TWISTEDREFLECTION         21063
#define SPELL_EXPLODE                   20476

/* Replaced
#define SPELL_SHADOWVOLLEY              21341
#define SPELL_CLEAVE                    8255
#define SPELL_THUNDERCLAP               26554
#define SPELL_VOIDBOLT                  22709
#define SPELL_MARKOFKAZZAK              21056
*/

#define MAX_MARK_TARGETS                5
/* 
Melee
Shadowbolt Volley
Cleave
Thunderclap
Twisted Reflection
Mark of Kazzak
Void Bolt
Capture Soul
*/
struct MANGOS_DLL_DECL boss_lordkazzakAI : public ScriptedAI
{
    boss_lordkazzakAI(Creature *c) : ScriptedAI(c)
	{
		Reset();
	}

    uint32 ShadowVolley_Timer;
    uint32 Cleave_Timer;
    uint32 ThunderClap_Timer;
    uint32 VoidBolt_Timer;
    uint32 MarkOfKazzak_Timer;
    uint32 Enrage_Timer;
    uint32 Twisted_Reflection_Timer;
    bool enrage;

    void Reset()
    {
        ShadowVolley_Timer			= urand(5000, 8000);
        Cleave_Timer				= 5000;
        ThunderClap_Timer			= urand(14000, 17000);
        VoidBolt_Timer				= 15000;
        MarkOfKazzak_Timer			= 18000;
        Enrage_Timer				= 180000;
        Twisted_Reflection_Timer	= 15000;  
        enrage						= false;
    }

    Player* DoSelectRandomNonMeleePlayer()
    {
        if (!m_creature->CanHaveThreatList())
            return 0;

        std::vector<ObjectGuid> vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        std::vector<Player*> vManaPlayers;

        if (!vGuids.empty())
        {
            for (std::vector<ObjectGuid>::const_iterator itr = vGuids.begin(); itr != vGuids.end(); ++itr)
            {
                if (Unit* pTarget = m_creature->GetMap()->GetUnit(*itr))
                {
                    // We need only a player
                    if (!pTarget->IsCharmerOrOwnerPlayerOrPlayerItself())
                        continue;

                    if(pTarget->getPowerType() == POWER_MANA)
                        // a player with mana power
                        vManaPlayers.push_back((Player*)pTarget);
                }
            }
        }
        
        if (!vManaPlayers.empty())
        {
            std::vector<Player*>::iterator i = vManaPlayers.begin();
            advance(i, (rand() % vManaPlayers.size()));
            return (*i);
        }
        
        return 0;
    }

    void JustRespawned()
    {
        DoScriptText(SAY_INTRO, m_creature);
    }

    void Aggro(Unit *who)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature);
				break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature);
				break;
        }
    }

    void KilledUnit(Unit* victim)
    {
        if (victim->GetTypeId() != TYPEID_PLAYER)
            return;

        m_creature->CastSpell(m_creature, SPELL_CAPTURESOUL, true);

        switch(urand(0,2))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
            case 2: DoScriptText(SAY_KILL3, m_creature); break;
        }
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

		// 12 Stunden = 43200 Sekunden.

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
			Track(static_cast<Player*>(killer), Raids::Worldbosses, Bosses::LordKazzak);
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

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		//ShadowVolley_Timer
		if (ShadowVolley_Timer < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWVOLLEY) == CAST_OK)

			if (enrage)
				ShadowVolley_Timer = 1000;
			else
				ShadowVolley_Timer = urand(6000, 10000);
		}
		else
			ShadowVolley_Timer -= diff;

		//Cleave_Timer
		if (Cleave_Timer < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
				Cleave_Timer = urand(6000, 8000);
		}
		else
			Cleave_Timer -= diff;

		//ThunderClap_Timer
		if (ThunderClap_Timer < diff)
		{
			if (DoCastSpellIfCan(m_creature, SPELL_THUNDERCLAP) == CAST_OK)
				ThunderClap_Timer = urand(11000, 14000);
		}
		else
			ThunderClap_Timer -= diff;

		//VoidBolt_Timer
		if (VoidBolt_Timer < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_VOIDBOLT) == CAST_OK)
				VoidBolt_Timer = urand(16000, 20000);
		}
		else
			VoidBolt_Timer -= diff;

		//MarkOfKazzak_Timer
		if (MarkOfKazzak_Timer < diff)
		{
			if (Player* pManaPlayer = DoSelectRandomNonMeleePlayer())
			{
				m_creature->CastSpell(pManaPlayer, SPELL_MARKOFKAZZAK, false);
				MarkOfKazzak_Timer = 18000;
			}
		}
		else
			MarkOfKazzak_Timer -= diff;

		//Twisted_Reflection_Timer
		if (Twisted_Reflection_Timer < diff)
		{
			if (Unit* u = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
			{
				if (DoCastSpellIfCan(u, SPELL_TWISTEDREFLECTION) == CAST_OK)
					Twisted_Reflection_Timer = 18000;
			}
		}
		else
			Twisted_Reflection_Timer -= diff;

		if (!enrage)
		{
			//Enrage_Timer
			if (Enrage_Timer < diff)
			{
				DoScriptText(EMOTE_FRENZY, m_creature);
				if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
					enrage = true;
			}
			else
				Enrage_Timer -= diff;
		}

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lordkazzak(Creature *_Creature)
{
    return new boss_lordkazzakAI (_Creature);
}

void AddSC_boss_lord_kazzak()
{
    Script* pNewscript;
    pNewscript = new Script;
    pNewscript->Name = "boss_lord_kazzak";
    pNewscript->GetAI = &GetAI_boss_lordkazzak;
    pNewscript->RegisterSelf();
}