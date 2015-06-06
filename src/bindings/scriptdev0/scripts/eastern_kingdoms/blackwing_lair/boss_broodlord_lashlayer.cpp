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
SDName: Boss_Broodlord_Lashlayer
SD%Complete: 100
SDComment:
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"
#include "../system/ProgressMonitor.h"

enum
{
    SAY_AGGRO                   = -1469000,
    SAY_LEASH                   = -1469001,

    SPELL_CLEAVE                = 26350,
    SPELL_BLAST_WAVE            = 23331,
    SPELL_MORTAL_STRIKE         = 24573,
	SPELL_KNOCK_AWAY			= 25778
};

struct MANGOS_DLL_DECL boss_broodlordAI : public ScriptedAI
{
    boss_broodlordAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiCleaveTimer;
    uint32 m_uiBlastWaveTimer;
    uint32 m_uiMortalStrikeTimer;
    uint32 m_uiKnockAwayTimer;

    void Reset() override
    {
        m_uiCleaveTimer         = 6000;                     // These times are probably wrong
        m_uiBlastWaveTimer      = 12000;
        m_uiMortalStrikeTimer   = 20000;
        m_uiKnockAwayTimer      = 30000;

		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LASHLAYER, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* killer) override
    {
        if (m_pInstance)
			m_pInstance->SetData(TYPE_LASHLAYER, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::BlackWingLair, Bosses::Lashlayer);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LASHLAYER, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Cleave Timer
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = 8000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Blast Wave
        if (m_uiBlastWaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BLAST_WAVE) == CAST_OK)
                m_uiBlastWaveTimer = 12000;
        }
        else
            m_uiBlastWaveTimer -= uiDiff;

        // Mortal Strike Timer
        if (m_uiMortalStrikeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTAL_STRIKE) == CAST_OK)
                m_uiMortalStrikeTimer = urand(30000, 35000);
        }
        else
            m_uiMortalStrikeTimer -= uiDiff;

        // Knock away
        if (m_uiKnockAwayTimer < uiDiff)
        {
			Unit* pTarget = m_creature->getVictim();
            if (DoCastSpellIfCan(pTarget, SPELL_KNOCK_AWAY) == CAST_OK)
            {
                // Drop 50% aggro
                if (m_creature->getThreatManager().getThreat(pTarget))
                    m_creature->getThreatManager().modifyThreatPercent(pTarget, -50);

				m_uiKnockAwayTimer = 20000;
            }
        }
        else
            m_uiKnockAwayTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (EnterEvadeIfOutOfCombatArea(uiDiff))
            DoScriptText(SAY_LEASH, m_creature);
    }
};
CreatureAI* GetAI_boss_broodlord(Creature* pCreature)
{
    return new boss_broodlordAI(pCreature);
}

void AddSC_boss_broodlord()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_broodlord";
    pNewScript->GetAI = &GetAI_boss_broodlord;
    pNewScript->RegisterSelf();
}
