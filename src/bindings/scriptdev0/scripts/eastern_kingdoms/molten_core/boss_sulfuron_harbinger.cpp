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
SDName: Boss_Sulfuron_Harbringer
SD%Complete: 80
SDComment: Adds NYI
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "../system/ProgressMonitor.h"

#define SPELL_DARKSTRIKE            19777
#define SPELL_DEMORALIZINGSHOUT     19778
#define SPELL_INSPIRE               19779
#define SPELL_KNOCKDOWN             19780
#define SPELL_FLAMESPEAR            19781

//Adds Spells
#define SPELL_HEAL                  19775
#define SPELL_SHADOWWORDPAIN        19776
#define SPELL_IMMOLATE              20294

struct MANGOS_DLL_DECL boss_sulfuronAI : public ScriptedAI
{
    boss_sulfuronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 DemoralizingShout_Timer;
    uint32 Inspire_Timer;
    uint32 Knockdown_Timer;
    uint32 Flamespear_Timer;

    uint32 hittimer;

    void Reset()
    {
        DemoralizingShout_Timer = 10000;
        Inspire_Timer			= 11000;
        Knockdown_Timer			= 6000;
        Flamespear_Timer		= 3000;
        hittimer				= 1600;
    }

    void JustDied(Unit *killer)
    {
		if (pInstance)
			pInstance->SetData(TYPE_SULFURON, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::MoltenCore, Bosses::Sulfuronherold);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //DemoralizingShout_Timer
        if (DemoralizingShout_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEMORALIZINGSHOUT);
            DemoralizingShout_Timer = urand(10000, 15000);
        } else DemoralizingShout_Timer -= diff;

        //Inspire_Timer
        if (Inspire_Timer < diff)
        {
            Creature* target = NULL;
            std::list<Creature*> pList = DoFindFriendlyMissingBuff(75.0f, SPELL_INSPIRE);
            if (!pList.empty())
            {
                std::list<Creature*>::iterator i = pList.begin();
                advance(i, (rand()%pList.size()));
                target = (*i);
            }

            if (target)
                DoCastSpellIfCan(target, SPELL_INSPIRE);

            Inspire_Timer = urand(15000, 20000);
        } else Inspire_Timer -= diff;

        //Knockdown_Timer
        if (Knockdown_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_KNOCKDOWN);
            Knockdown_Timer = urand(10000, 15000);
        } else Knockdown_Timer -= diff;

        //Flamespear_Timer
        if (Flamespear_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_FLAMESPEAR);

            Flamespear_Timer = urand(12000, 16000);
        } else Flamespear_Timer -= diff;

        if (hittimer < diff)
        {
            DoMeleeAttackIfReady();
            hittimer = 1500;
        } else hittimer -= diff;
    }
};

CreatureAI* GetAI_boss_sulfuron(Creature* pCreature)
{
    return new boss_sulfuronAI(pCreature);
}

struct MANGOS_DLL_DECL mob_flamewaker_priestAI : public ScriptedAI
{
    mob_flamewaker_priestAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Heal_Timer;
    uint32 ShadowWordPain_Timer;
    uint32 Immolate_Timer;
	uint32 Darkstrike_Timer;
	uint32 Boss_Check_Hitpoints_Timer;

    void Reset()
    {
        Heal_Timer					= urand(18000, 22000);
        ShadowWordPain_Timer		= 6000;
        Immolate_Timer				= 12000;
        Darkstrike_Timer			= 10000;
		Boss_Check_Hitpoints_Timer	= 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Boss_Check_Hitpoints_Timer < diff)
        {
			if (Creature* pSulfuron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SULFURON)))
            {
				if (pSulfuron->GetHealthPercent() < 90.0f)
                {
                    m_creature->SetHealth(pSulfuron->GetMaxHealth());
                }
			}

            Boss_Check_Hitpoints_Timer = 10000;
        } else Boss_Check_Hitpoints_Timer -= diff;

        //Casting Heal to Sulfuron or other Guards.
        if (Heal_Timer < diff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;

            DoCastSpellIfCan(pUnit, SPELL_HEAL);

            Heal_Timer = 5000;
        } else Heal_Timer -= diff;

        //ShadowWordPain_Timer
        if (ShadowWordPain_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SHADOWWORDPAIN);

            ShadowWordPain_Timer = urand(12000, 15000);
        } else ShadowWordPain_Timer -= diff;

        //Immolate_Timer
        if (Immolate_Timer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_IMMOLATE);

            Immolate_Timer = urand(15000, 20000);
        } else Immolate_Timer -= diff;

        //DarkStrike_Timer
        if (Darkstrike_Timer < diff)
        {
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARKSTRIKE);
            Darkstrike_Timer = urand(12000, 16000);
        } else Darkstrike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_flamewaker_priest(Creature* pCreature)
{
    return new mob_flamewaker_priestAI(pCreature);
}

void AddSC_boss_sulfuron()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sulfuron";
    newscript->GetAI = &GetAI_boss_sulfuron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flamewaker_priest";
    newscript->GetAI = &GetAI_mob_flamewaker_priest;
    newscript->RegisterSelf();
}
