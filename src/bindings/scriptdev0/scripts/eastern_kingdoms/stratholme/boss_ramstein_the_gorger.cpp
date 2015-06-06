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
SDName: boss_ramstein_the_gorger
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "precompiled.h"
#include "stratholme.h"

#define C_MINDLESS_UNDEAD   11030

enum
{
   SPELL_TRAMPLE    = 5568,
   SPELL_KNOCKOUT   = 17307
};

struct MANGOS_DLL_DECL boss_ramstein_the_gorgerAI : public ScriptedAI
{
    boss_ramstein_the_gorgerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	Unit* target;
	Creature *Summoned;
    uint32 m_uiTrample_Timer;
    uint32 m_uiKnockout_Timer;

    void Reset()
    {
        m_uiTrample_Timer    = 3000;
        m_uiKnockout_Timer   = 12000;
    }

    void Aggro(Unit*)
    {
	    if (m_pInstance)
			m_pInstance->SetData(TYPE_RAMSTEIN, IN_PROGRESS);
    }

    void JustDied(Unit*)
    {
	    if (m_pInstance)
			m_pInstance->SetData(TYPE_RAMSTEIN, DONE);
		/*
		Unit* target = NULL;
		target = m_creature->SelectRandomUnfriendlyTarget(target, 100.0f);
        if (target && target->GetTypeId() == TYPEID_PLAYER)
        {
			if(m_creature->getThreatManager().getThreat(target))
            m_creature->getThreatManager().modifyThreatPercent(target, -100);

			for(uint8 i = 0; i < 30; i++)
			Summoned = m_creature->SummonCreature(C_MINDLESS_UNDEAD, 3969.35f, -3391.87f, 119.11f, 5.91f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
			((CreatureAI*)Summoned->AI())->AttackStart(target);
		}*/
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
			if (m_pInstance)
				m_pInstance->SetData(TYPE_RAMSTEIN, FAIL);

            Reset();
            return;
        }

        // Trample
        if (m_uiTrample_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_TRAMPLE) == CAST_OK)
                m_uiTrample_Timer = 7000;
        }
        else
            m_uiTrample_Timer -= uiDiff;

        // Knockout
        if (m_uiKnockout_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_KNOCKOUT) == CAST_OK)
                m_uiKnockout_Timer = 10000;
        }
        else
            m_uiKnockout_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ramstein_the_gorger(Creature* pCreature)
{
    return new boss_ramstein_the_gorgerAI(pCreature);
}

void AddSC_boss_ramstein_the_gorger()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ramstein_the_gorger";
    pNewScript->GetAI = &GetAI_boss_ramstein_the_gorger;
    pNewScript->RegisterSelf();
}
