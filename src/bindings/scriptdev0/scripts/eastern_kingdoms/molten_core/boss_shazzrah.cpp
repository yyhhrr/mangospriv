/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Shazzrah
SD%Complete: 75
SDComment: Teleport NYI
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "../system/ProgressMonitor.h"

enum
{
    SPELL_ARCANE_EXPLOSION          = 19712,
    SPELL_SHAZZRAHCURSE             = 19713,
    SPELL_DEADENMAGIC               = 19714,
    SPELL_COUNTERSPELL              = 19715,
    SPELL_SHAZZRAH_GATE             = 23138,
    SPELL_SHAZZRAH_GATE_EFFECT      = 23139
};

struct MANGOS_DLL_DECL boss_shazzrahAI : public ScriptedAI
{
    boss_shazzrahAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 ArcaneExplosion_Timer;
    uint32 ShazzrahCurse_Timer;
    uint32 DeadenMagic_Timer;
    uint32 Countspell_Timer;
    uint32 GateOfShazzrahTimer;

    void Reset()
    {
        ArcaneExplosion_Timer	= 5000;                       //These times are probably wrong
        ShazzrahCurse_Timer		= 8000;
        DeadenMagic_Timer		= 15000;
        Countspell_Timer		= 10000;
        GateOfShazzrahTimer		= 30000;
    }

	void Aggro(Unit*)
	{
		if (pInstance)
			pInstance->SetData(TYPE_SHAZZRAH, IN_PROGRESS);

		m_creature->SetInCombatWithZone();
	}

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_SHAZZRAH, NOT_STARTED);
	}

    void JustDied(Unit* killer)
    {
		if (pInstance)
			pInstance->SetData(TYPE_SHAZZRAH, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::MoltenCore, Bosses::Shazzrah);
    }

	int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void SpellHit(Unit*, const SpellEntry* entry)
	{
		if(entry->Effect[0] == SPELL_EFFECT_DISPEL || entry->Effect[1] == SPELL_EFFECT_DISPEL || entry->Effect[2] == SPELL_EFFECT_DISPEL)
		{
			m_creature->RemoveAura(SPELL_DEADENMAGIC, EFFECT_INDEX_0);
			m_creature->RemoveAura(SPELL_DEADENMAGIC, EFFECT_INDEX_1);
			m_creature->RemoveAura(SPELL_DEADENMAGIC, EFFECT_INDEX_2);
		}
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            ArcaneExplosion_Timer = random(7000, 10000);
        } else ArcaneExplosion_Timer -= diff;

        //ShazzrahCurse_Timer
        if (ShazzrahCurse_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (target)
				DoCastSpellIfCan(target, SPELL_SHAZZRAHCURSE);
            ShazzrahCurse_Timer = random(15000, 20000);
        } else ShazzrahCurse_Timer -= diff;

        //DeadenMagic_Timer
        if (DeadenMagic_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DEADENMAGIC);
            DeadenMagic_Timer = random(20000, 25000);
        } else DeadenMagic_Timer -= diff;

        //Countspell_Timer
        if (Countspell_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_COUNTERSPELL);
            Countspell_Timer = random(12000, 15000);
        } else Countspell_Timer -= diff;

        // Gate of Shazzrah
        if (GateOfShazzrahTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                DoResetThreat();
                DoCastSpellIfCan(pTarget, SPELL_SHAZZRAH_GATE_EFFECT, CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_ARCANE_EXPLOSION);
                m_creature->AddThreat(pTarget, 50.0f);
                m_creature->AI()->AttackStart(pTarget);
            }
            GateOfShazzrahTimer = 45000;
        }
        else
            GateOfShazzrahTimer -= diff;

		DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_shazzrah(Creature* pCreature)
{
    return new boss_shazzrahAI(pCreature);
}

void AddSC_boss_shazzrah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_shazzrah";
    newscript->GetAI = &GetAI_boss_shazzrah;
    newscript->RegisterSelf();
}
