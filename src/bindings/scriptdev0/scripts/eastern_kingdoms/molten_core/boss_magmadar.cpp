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
SDName: Boss_Magmadar
SD%Complete: 75
SDComment: Conflag on ground nyi, fear causes issues without VMAPs
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "../system/ProgressMonitor.h"

enum
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,

    SPELL_FRENZY                = 19451,
    SPELL_MAGMASPIT             = 19449,                    //This is actually a buff he gives himself
    SPELL_PANIC                 = 19408,
    SPELL_LAVABOMB 				= 20474,
	SPELL_LAVA_ATEM				= 19272	
};

struct MANGOS_DLL_DECL boss_magmadarAI : public ScriptedAI
{
    boss_magmadarAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

	bool TankFeared;
	Unit* target;
	ObjectGuid Tank;

    uint32 Frenzy_Timer;
    uint32 Panic_Timer;
    uint32 Lavabomb_Timer;
	uint32 LavaAtem_Timer;
    float tankthreat;
    uint32 hittimer;

    void Reset()
    {
        Frenzy_Timer	= 25000;
        Panic_Timer		= 7000;
        Lavabomb_Timer	= 5000;
		LavaAtem_Timer	= 7000;

        hittimer		= 1300;

        tankthreat = 0.0f;

		Tank = ObjectGuid();
		TankFeared = false;

		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
        m_creature->CastSpell(m_creature, SPELL_MAGMASPIT, true);
    }

    void JustDied(Unit *killer)
    {
		if (pInstance)
		{
			pInstance->SetData(TYPE_MAGMADAR,DONE);
			pInstance->SetData(TYPE_TRASH_MAG,DONE);
		}

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::MoltenCore, Bosses::Magmadar);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Frenzy_Timer
        if (Frenzy_Timer < diff)
        {
            DoScriptText(EMOTE_GENERIC_FRENZY_KILL, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_FRENZY);
            Frenzy_Timer = 15000;
        } else Frenzy_Timer -= diff;

        // Panic
        if (Panic_Timer <= diff)
        {
			if (Unit* pTank = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
				Tank = pTank->GetObjectGuid();

            DoCastSpellIfCan(m_creature->getVictim(), SPELL_PANIC);
            Panic_Timer = 30000;
        }
        else
			Panic_Timer -= diff;

		// Tank aggro loss at fear
		if (!TankFeared && Tank)
		{
			Unit* pTank = m_creature->GetMap()->GetUnit(Tank);
			if (pTank && pTank->HasAura(SPELL_PANIC))
			{
				TankFeared = true;
				// m_creature->getThreatManager().modifyThreatPercent(pTank, -99);
                tankthreat = m_creature->getThreatManager().getCurrentVictim()->getThreat() -1.0f;
                m_creature->getThreatManager().addThreatDirectly(pTank, -( m_creature->getThreatManager().getCurrentVictim()->getThreat() -1.0f));
			}
		}

		// Tank aggro regain after fear
		if (TankFeared && Tank)
		{
			Unit* pTank = m_creature->GetMap()->GetUnit(Tank);
			if (pTank && !pTank->HasAura(SPELL_PANIC))
			{
				TankFeared = false;
				// m_creature->getThreatManager().modifyThreatPercent(pTank, 11000);
                m_creature->getThreatManager().addThreatDirectly(pTank, tankthreat);
                
			}
		}

		//Lava Atem
		if (LavaAtem_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_LAVA_ATEM);
            LavaAtem_Timer = random(8000, 12000);
        } else LavaAtem_Timer -= diff;

        //Lavabomb_Timer
        if (Lavabomb_Timer < diff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_LAVABOMB);
            Lavabomb_Timer = random(8000, 12000);
        } else Lavabomb_Timer -= diff;

        if (hittimer < diff)
        {
            DoMeleeAttackIfReady();
            hittimer = 1400;
        } else hittimer -= diff;
    }
};

CreatureAI* GetAI_boss_magmadar(Creature* pCreature)
{
    return new boss_magmadarAI(pCreature);
}

void AddSC_boss_magmadar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_magmadar";
    newscript->GetAI = &GetAI_boss_magmadar;
    newscript->RegisterSelf();
}
