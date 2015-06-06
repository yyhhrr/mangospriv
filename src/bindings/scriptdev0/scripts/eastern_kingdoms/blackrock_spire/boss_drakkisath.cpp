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

/* ScriptData
SDName: Boss_Drakkisath
SD%Complete: 100
SDComment:
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_FIRENOVA			= 23462,
    SPELL_CLEAVE			= 15284,
    SPELL_CONFLIGURATION	= 16805,
    SPELL_THUNDERCLAP		= 15588,
	SPELL_WUT				= 16789,
	SPELL_FLAMMENSTOSS		= 16419,
	SPELL_RUESTUNG			= 6016
};

struct MANGOS_DLL_DECL boss_drakkisathAI : public ScriptedAI
{
    boss_drakkisathAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    Unit* target;

    Creature* CDO;
    Creature* CDT;

    uint64 cdo_GUID;
    uint64 cdt_GUID;

    uint32 m_uiFireNovaTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiConfligurationTimer;
    uint32 m_uiThunderclapTimer;
    uint32 m_uiFlammenstoss_Timer;
	uint32 m_uiRuestung_Durchstechen_Timer;
    uint32 m_uiWut_Timer;

    void Reset()
    {
        m_uiFireNovaTimer				= 15000;
        m_uiCleaveTimer					= 8000;
        m_uiConfligurationTimer			= 20000;
        m_uiThunderclapTimer			= 12000;
		m_uiFlammenstoss_Timer			= 10000;
		m_uiRuestung_Durchstechen_Timer = 7000;
        m_uiWut_Timer					= 35000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Das war ein Fehler von Euch!", LANG_UNIVERSAL, NULL);
		m_creature->SetInCombatWithZone();
		m_creature->CallForHelp(30.0f);
    }

    void JustDied(Unit* Killer)
    {
		m_creature->MonsterYell("Meine Armee.. wird.. Nefarian...",LANG_UNIVERSAL,NULL);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void JustReachedHome()
    {
        if ( CDO->isDead()) {
            CDO->Respawn();
        }
        if ( CDT->isDead()) {
            CDT->Respawn();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        Map* pmap;
        pmap= m_creature->GetMap();


        if (CDO = pmap->GetCreature(cdo_GUID))
        {
            // ok !
        }
        else
        {
            CDO = (m_creature->SummonCreature(10814, 32.0f, -291.0f, 111.0f, 3.21f,TEMPSUMMON_MANUAL_DESPAWN, 0));
            cdo_GUID = CDO->GetGUID();
	    CDO->SetRespawnDelay(30000000);
        }

        if (CDT = pmap->GetCreature(cdt_GUID))
        {
            // ok !
        }
        else
        {
            CDT = (m_creature->SummonCreature(10814, 32.0f, -279.0f, 111.0f, 3.21f, TEMPSUMMON_MANUAL_DESPAWN, 0));
            cdt_GUID = CDT->GetGUID();
	    CDO->SetRespawnDelay(30000000);
        }

		// Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // FireNova
        if (m_uiFireNovaTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FIRENOVA);
            m_uiFireNovaTimer = random(10000, 15000);
        }
        else
            m_uiFireNovaTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = random(6000, 8000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Confliguration
        if (m_uiConfligurationTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONFLIGURATION, 0, m_creature->getVictim()->GetObjectGuid());
            m_uiConfligurationTimer = random(14000, 21000);
        }
        else
            m_uiConfligurationTimer -= uiDiff;

        // Thunderclap
        if (m_uiThunderclapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_THUNDERCLAP);
            m_uiThunderclapTimer= random(12000, 15000);
        }
        else
            m_uiThunderclapTimer -= uiDiff;

		//Wut
        if (m_uiWut_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WUT);
            m_uiWut_Timer = random(30000, 35000);
        } else m_uiWut_Timer -= uiDiff;

		//R¸stung verringern
        if (m_uiRuestung_Durchstechen_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_RUESTUNG);
            m_uiRuestung_Durchstechen_Timer = random(15000, 22000);
        } else m_uiRuestung_Durchstechen_Timer -= uiDiff;

		//Flammenstoﬂ
        if (m_uiFlammenstoss_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_FLAMMENSTOSS);
            m_uiFlammenstoss_Timer = random(8000, 15000);
        } else m_uiFlammenstoss_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_drakkisath(Creature* pCreature)
{
    return new boss_drakkisathAI(pCreature);
}

enum eDrakkisathGuard
{
    SPELL_KNOCKDOWN		= 16790,
    SPELL_MORTAL_STRIKE = 15708,
    SPELL_STRIKE		= 15580
};

struct MANGOS_DLL_DECL mob_chromatic_elite_guardAI : public ScriptedAI
{
    mob_chromatic_elite_guardAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 m_uiKnockdownTimer;
    uint32 m_uiMortalStrikeTimer;
    uint32 m_uiStrikeTimer;

    void Reset()
    {
        m_uiKnockdownTimer		= urand(4000, 8000);
        m_uiMortalStrikeTimer	= urand(5000, 8000);
        m_uiStrikeTimer			= urand(10000, 12000);
    }

    void Aggro(Unit* pVictim)
    {
		m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Knockdown
        if (m_uiKnockdownTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_KNOCKDOWN);
            m_uiKnockdownTimer = urand(8000, 10000);
        }
        else
            m_uiKnockdownTimer -= uiDiff;

        // Mortal Strike
        if (m_uiMortalStrikeTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
            m_uiMortalStrikeTimer = urand(5000, 9000);
        }
        else
            m_uiMortalStrikeTimer -= uiDiff;

        // Strike
        if (m_uiStrikeTimer <= uiDiff)
        {
            if (urand(0, 100) <= 80)
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_STRIKE);
            m_uiStrikeTimer = urand(6000, 9000);
        }
        else
            m_uiStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_chromatic_elite_guard(Creature* pCreature)
{
    return new mob_chromatic_elite_guardAI(pCreature);
}

void AddSC_boss_drakkisath()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_drakkisath";
    newscript->GetAI = &GetAI_boss_drakkisath;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_chromatic_elite_guard";
    newscript->GetAI = &GetAI_mob_chromatic_elite_guard;
    newscript->RegisterSelf();
}
