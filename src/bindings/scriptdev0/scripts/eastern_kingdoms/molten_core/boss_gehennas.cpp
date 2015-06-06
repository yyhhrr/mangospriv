
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
SDName: Boss_Gehennas
SD%Complete: 90
SDComment: Adds MC NYI
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "../system/ProgressMonitor.h"

#define SPELL_SHADOWBOLT            19729
#define SPELL_RAINOFFIRE            19717
#define SPELL_GEHENNASCURSE         19716

struct MANGOS_DLL_DECL boss_gehennasAI : public ScriptedAI
{
    boss_gehennasAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 ShadowBolt_Timer;
    uint32 RainOfFire_Timer;
    uint32 GehennasCurse_Timer;

    void Reset()
    {
        ShadowBolt_Timer	= 8000;
        RainOfFire_Timer	= 4000;
        GehennasCurse_Timer = 20000;
    }

    void Aggro(Unit* pWho)
    {
		if (pInstance)
			pInstance->SetData(TYPE_GEHENNAS, IN_PROGRESS);

		m_creature->CallForHelp(RANGE_CALL_FOR_HELP);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_GEHENNAS, NOT_STARTED);
	}

    void JustDied(Unit *killer)
    {
		if (pInstance)
			pInstance->SetData(TYPE_GEHENNAS, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::MoltenCore, Bosses::Gehennas);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //ShadowBolt_Timer
        if (ShadowBolt_Timer < diff)
        {
            if (Unit* bTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(bTarget, SPELL_SHADOWBOLT);
            ShadowBolt_Timer = random(7000, 10000);
        } else ShadowBolt_Timer -= diff;

        //RainOfFire_Timer
        if (RainOfFire_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_RAINOFFIRE);

            RainOfFire_Timer = random(5000, 7000);
        } else RainOfFire_Timer -= diff;

        //GehennasCurse_Timer
        if (GehennasCurse_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_GEHENNASCURSE);
            GehennasCurse_Timer = 30000;
        } else GehennasCurse_Timer -= diff;
		
		DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_gehennas(Creature* pCreature)
{
    return new boss_gehennasAI(pCreature);
}

#define SPELL_ADD_STOSS				19730
#define SPELL_ADD_FAUST_RAGI		20277
#define SPELL_ADD_RUSH				24317

struct MANGOS_DLL_DECL npc_gehennas_addAI : public ScriptedAI
{
    npc_gehennas_addAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

	uint32 m_uiAddStossTimer;
    uint32 m_uiAddFaustRagiTimer;
	uint32 m_uiAddRushTimer;

    void Reset()
    {
        m_uiAddStossTimer		= urand(5000,8000);
		m_uiAddFaustRagiTimer	= urand(5000,7000);
        m_uiAddRushTimer		= urand(2000,5000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
    
	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiAddStossTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ADD_STOSS);
            m_uiAddStossTimer = random(6000, 10000);
        } else m_uiAddStossTimer -= diff;

        if (m_uiAddFaustRagiTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ADD_FAUST_RAGI);
            m_uiAddFaustRagiTimer = random(8000, 14000);
        } else m_uiAddFaustRagiTimer -= diff;

		if (m_uiAddRushTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ADD_RUSH);
            m_uiAddRushTimer = random(8000, 12000);
        } else m_uiAddRushTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_gehennas_add(Creature *_Creature)
{
    return new npc_gehennas_addAI (_Creature);
}

void AddSC_boss_gehennas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gehennas";
    newscript->GetAI = &GetAI_boss_gehennas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_gehennas_add";
    newscript->GetAI = &GetAI_npc_gehennas_add;
    newscript->RegisterSelf();
}
