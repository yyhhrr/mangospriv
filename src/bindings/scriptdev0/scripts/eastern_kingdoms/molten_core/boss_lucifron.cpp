/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Lucifron
SD%Complete: 100
SDComment:
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "Pat.h"
#include "../system/ProgressMonitor.h"

#define SPELL_IMPENDINGDOOM 19702
#define SPELL_LUCIFRONCURSE 19703
#define SPELL_SHADOWSHOCK   19460

//Adds
#define SPELL_SPALTEN		20605
#define SPELL_GEDANKEN		20604
#define SPELL_ENRAGE		27680

struct MANGOS_DLL_DECL boss_lucifronAI : public ScriptedAI
{
    boss_lucifronAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 ImpendingDoom_Timer;
    uint32 LucifronCurse_Timer;
    uint32 ShadowShock_Timer;

    void Reset()
    {
        ImpendingDoom_Timer = 10000;    //Initial cast after 10 seconds so the debuffs alternate
        LucifronCurse_Timer = 20000;	//Initial cast after 20 seconds
        ShadowShock_Timer	= 6000;		//6 seconds
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit* pWho)
    {
		if (pInstance)
			pInstance->SetData(TYPE_LUCIFRON, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_LUCIFRON, FAIL);
	}

    void JustDied(Unit *killer)
    {
		if (pInstance)
			pInstance->SetData(TYPE_LUCIFRON, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::MoltenCore, Bosses::Lucifron);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		//Impending doom timer
        if (ImpendingDoom_Timer < diff)
        {
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_IMPENDINGDOOM, CAST_TRIGGERED);
            ImpendingDoom_Timer = 15000;
        }
		else
			ImpendingDoom_Timer -= diff;
		
		//Lucifron's curse timer
        if (LucifronCurse_Timer < diff)
        {
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_LUCIFRONCURSE, CAST_TRIGGERED);
            LucifronCurse_Timer = 20000;
        }
		else
			LucifronCurse_Timer -= diff;

        //Shadowshock
        if (ShadowShock_Timer < diff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_SHADOWSHOCK, CAST_TRIGGERED);
			
			ShadowShock_Timer = 6000;
		}
		else
			ShadowShock_Timer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lucifron(Creature* pCreature)
{
    return new boss_lucifronAI(pCreature);
}

struct MANGOS_DLL_DECL npc_beschuetzer_der_flammenAI : public ScriptedAI
{
    npc_beschuetzer_der_flammenAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

	uint32 m_uiAddSpaltenTimer;
    uint32 m_uiAddMindControlTimer;

    void Reset()
    {
        m_uiAddSpaltenTimer		= urand(4000, 7000);
        m_uiAddMindControlTimer = urand(6000, 10000);

		m_creature->addUnitState(UNIT_STAT_IGNORE_PATHFINDING);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiAddSpaltenTimer < diff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SPALTEN) == CAST_OK)
                m_uiAddSpaltenTimer = urand(5000, 8000);
        }
		else
			m_uiAddSpaltenTimer -= diff;

        if (m_uiAddMindControlTimer <= diff)
        {
			Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (DoCastSpellIfCan(pTarget, SPELL_GEDANKEN) == CAST_OK)
                m_uiAddMindControlTimer = urand(8000, 15000);
        }
        else
            m_uiAddMindControlTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_beschuetzer_der_flammen(Creature *_Creature)
{
    return new npc_beschuetzer_der_flammenAI (_Creature);
}

void AddSC_boss_lucifron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lucifron";
    newscript->GetAI = &GetAI_boss_lucifron;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="npc_lucifron_add";
    newscript->GetAI = &GetAI_npc_beschuetzer_der_flammen;
    newscript->RegisterSelf();
}
