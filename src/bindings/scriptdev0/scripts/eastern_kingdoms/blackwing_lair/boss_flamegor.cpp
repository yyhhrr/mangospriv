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
SDName: Boss_Flamegor
SD%Complete: 90
SDComment: Thrash is missing
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"
#include "../system/ProgressMonitor.h"

enum
{
    EMOTE_GENERIC_FRENZY        = -1000002,

    SPELL_SHADOW_FLAME          = 22539,
    SPELL_WING_BUFFET           = 23339,
    SPELL_FRENZY                = 23342,                    // This spell periodically triggers fire nova
    SPELL_THRASH                = 3391,                     // TODO missing
};

struct MANGOS_DLL_DECL boss_flamegorAI : public ScriptedAI
{
    boss_flamegorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowFlameTimer;
    uint32 m_uiWingBuffetTimer;
    uint32 m_uiFrenzyTimer;
	uint32 m_uiThrashTimer;

    void Reset() override
    {
        m_uiShadowFlameTimer	= 15000;                       // These times are probably wrong
        m_uiWingBuffetTimer		= 35000;
        m_uiFrenzyTimer			= 10000;
		m_uiThrashTimer			= 4000;

		m_creature->SetBoundingValue(6, 6);
		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FLAMEGOR, IN_PROGRESS);
    }

    void JustDied(Unit* killer) override
    {
        if (m_pInstance)
			m_pInstance->SetData(TYPE_FLAMEGOR, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::BlackWingLair, Bosses::Flamegor);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FLAMEGOR, FAIL);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_WING_BUFFET )
        {
            m_creature->getThreatManager().modifyThreatPercent(pTarget, -75);
        }
    }
    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Wing Buffet
        if (m_uiWingBuffetTimer <= uiDiff)
        {
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_WING_BUFFET, CAST_FORCE_CAST) == CAST_OK)
                m_uiWingBuffetTimer = 30000;      
        }
        else
            m_uiWingBuffetTimer -= uiDiff;

        // Shadow Flame Timer
        if (m_uiShadowFlameTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_FLAME) == CAST_OK)
                m_uiShadowFlameTimer = urand(15000, 18000);
        }
        else
            m_uiShadowFlameTimer -= uiDiff;

        // Frenzy Timer
        if (m_uiFrenzyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
            {
                DoScriptText(EMOTE_GENERIC_FRENZY, m_creature);
                m_uiFrenzyTimer = urand(8000, 10000);
            }
        }
        else
            m_uiFrenzyTimer -= uiDiff;

		// Thrash
		if (m_uiThrashTimer <= uiDiff)
		{
			Unit* pTarget = m_creature->getVictim();
			if (pTarget && pTarget->isAlive())
				m_creature->AttackerStateUpdate(pTarget, BASE_ATTACK, true);

			m_uiThrashTimer = urand(6000, 8000);
		}
		else
			m_uiThrashTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_flamegor(Creature* pCreature)
{
    return new boss_flamegorAI(pCreature);
}

void AddSC_boss_flamegor()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_flamegor";
    pNewScript->GetAI = &GetAI_boss_flamegor;
    pNewScript->RegisterSelf();
}
