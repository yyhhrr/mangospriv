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
SDName: boss_timmy_the_cruel
SD%Complete: 100
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"

#define SPELL_RAVENOUSCLAW      17470
#define SPELL_WUTANFALL			8599

struct MANGOS_DLL_DECL boss_timmy_the_cruelAI : public ScriptedAI
{
    boss_timmy_the_cruelAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 RavenousClaw_Timer;
	uint32 Wutanfall_Timer;

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Reset()
    {
        RavenousClaw_Timer = 10000;
		Wutanfall_Timer = 1000;
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("TIMMY!", LANG_UNIVERSAL,NULL);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (RavenousClaw_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_RAVENOUSCLAW);
            RavenousClaw_Timer = 15000;
        }else RavenousClaw_Timer -= diff;

        //Wutanfall
		if(m_creature->GetHealthPercent() < 50.0f)
		{
			if (Wutanfall_Timer < diff)
			{
				DoCastSpellIfCan(m_creature, SPELL_WUTANFALL);
				Wutanfall_Timer = random(125000, 130000);
			}else Wutanfall_Timer -= diff;
		}

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_timmy_the_cruel(Creature* pCreature)
{
    return new boss_timmy_the_cruelAI(pCreature);
}

void AddSC_boss_timmy_the_cruel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_timmy_the_cruel";
    newscript->GetAI = &GetAI_boss_timmy_the_cruel;
    newscript->RegisterSelf();
}
