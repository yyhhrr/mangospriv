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
SDName: Boss_Highlord_Omokk
SD%Complete: 100
SDComment:
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_WARSTOMP    = 24375,
    SPELL_STRIKE      = 18368,
    SPELL_REND        = 18106,
    SPELL_SUNDERARMOR = 24317,
    SPELL_KNOCKAWAY   = 20686,
    SPELL_SLOW        = 22356
};

struct MANGOS_DLL_DECL boss_highlordomokkAI : public ScriptedAI
{
    boss_highlordomokkAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiWarStompTimer;
    uint32 m_uiStrikeTimer;
    uint32 m_uiRendTimer;
    uint32 m_uiSunderArmorTimer;
    uint32 m_uiKnockAwayTimer;
    uint32 m_uiSlowTimer;

    void Reset()
    {
        m_uiWarStompTimer    = 15000;
        m_uiStrikeTimer      = 9000;
        m_uiRendTimer        = 14000;
        m_uiSunderArmorTimer = 2000;
        m_uiKnockAwayTimer   = 18000;
        m_uiSlowTimer        = 22000;
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Ha Ha Ha, ich stark sein.",LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit*)
    {
		m_creature->MonsterYell("Oh.. Ihr st\303\244rker sein als Omokk..",LANG_UNIVERSAL,NULL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // WarStomp
        if (m_uiWarStompTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WARSTOMP);
            m_uiWarStompTimer = 14000;
        }
        else
            m_uiWarStompTimer -= uiDiff;

        // Strike
        if (m_uiStrikeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_STRIKE);
            m_uiStrikeTimer = 10000;
        }
        else
            m_uiStrikeTimer -= uiDiff;

        // Rend
        if (m_uiRendTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_REND);
            m_uiRendTimer = 18000;
        }
        else
            m_uiRendTimer -= uiDiff;

        // Sunder Armor
        if (m_uiSunderArmorTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SUNDERARMOR);
            m_uiSunderArmorTimer = 25000;
        }
        else
            m_uiSunderArmorTimer -= uiDiff;

        // KnockAway
        if (m_uiKnockAwayTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_KNOCKAWAY);
            m_uiKnockAwayTimer = 12000;
        }
        else
            m_uiKnockAwayTimer -= uiDiff;

        // Slow
        if (m_uiSlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SLOW);
            m_uiSlowTimer = 18000;
        }
        else
            m_uiSlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_highlordomokk(Creature* pCreature)
{
    return new boss_highlordomokkAI(pCreature);
}

void AddSC_boss_highlordomokk()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_highlord_omokk";
    newscript->GetAI = &GetAI_boss_highlordomokk;
    newscript->RegisterSelf();
}
