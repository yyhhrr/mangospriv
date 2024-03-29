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
SDName: Boss_Mother_Smolderweb
SD%Complete: 100
SDComment: Uncertain how often mother's milk is casted
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_CRYSTALIZE              = 16104,
    SPELL_MOTHERSMILK             = 16468,
    SPELL_SUMMON_SPIRE_SPIDERLING = 16103
};

struct MANGOS_DLL_DECL boss_mothersmolderwebAI : public ScriptedAI
{
    boss_mothersmolderwebAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiCrystalizeTimer;
    uint32 m_uiMothersMilkTimer;

    void Reset()
    {
        m_uiCrystalizeTimer  = 20000;
        m_uiMothersMilkTimer = 10000;
    }

    void DamageTaken(Unit*, uint32 &uiDamage)
    {
        if (m_creature->GetHealth() <= uiDamage)
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_SPIRE_SPIDERLING, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Crystalize
        if (m_uiCrystalizeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_CRYSTALIZE);
            m_uiCrystalizeTimer = 14000;
        }
        else
            m_uiCrystalizeTimer -= uiDiff;

        // Mothers Milk
        if (m_uiMothersMilkTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_MOTHERSMILK);
            m_uiMothersMilkTimer = urand(5000, 10000);
        }
        else
            m_uiMothersMilkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_mothersmolderweb(Creature* pCreature)
{
    return new boss_mothersmolderwebAI(pCreature);
}

void AddSC_boss_mothersmolderweb()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_mother_smolderweb";
    newscript->GetAI = &GetAI_boss_mothersmolderweb;
    newscript->RegisterSelf();
}
