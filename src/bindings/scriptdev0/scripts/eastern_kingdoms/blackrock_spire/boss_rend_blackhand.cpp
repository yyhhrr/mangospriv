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
SDName: Boss_Rend_Blackhand
SD%Complete: 100
SDComment: Intro event NYI
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"
#include "blackrock_spire.h"

enum
{
    SPELL_WHIRLWIND   = 15589,
    SPELL_CLEAVE      = 20691,
    SPELL_THUNDERCLAP = 23931
};

struct MANGOS_DLL_DECL boss_rend_blackhandAI : public ScriptedAI
{
    boss_rend_blackhandAI(Creature* pCreature) : ScriptedAI(pCreature)
	{	
		m_pInstance = (instance_blackrock_spire*) pCreature->GetInstanceData();
		Reset();
	}

	instance_blackrock_spire* m_pInstance;

    uint32 m_uiWhirlWindTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiThunderclapTimer;

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Ich rei\303\237e Euch die Arme ab und pr\303\274gle Euch damit Unterw\303\274rfigkeit ein!", LANG_UNIVERSAL, NULL);
    }

    void JustDied(Unit* Killer)
    {
		m_creature->MonsterYell("NEIN! Unm\303\266glich!", LANG_UNIVERSAL, NULL);
    }

	void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GYTH, FAIL);
    }

    void Reset()
    {
        m_uiWhirlWindTimer   = 10000;
        m_uiCleaveTimer      = 5000;
        m_uiThunderclapTimer = 8000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // WhirlWind
        if (m_uiWhirlWindTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_WHIRLWIND);
            m_uiWhirlWindTimer = random(8000, 12000);
        }
        else
            m_uiWhirlWindTimer -= uiDiff;

		if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = random(6000, 10000);
        } else m_uiCleaveTimer -= uiDiff;

        // Thunderclap
        if (m_uiThunderclapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_THUNDERCLAP);
            m_uiThunderclapTimer = random(10000, 15000);
        }
        else
            m_uiThunderclapTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rend_blackhand(Creature* pCreature)
{
    return new boss_rend_blackhandAI(pCreature);
}

void AddSC_boss_rend_blackhand()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_rend_blackhand";
    newscript->GetAI = &GetAI_boss_rend_blackhand;
    newscript->RegisterSelf();
}
