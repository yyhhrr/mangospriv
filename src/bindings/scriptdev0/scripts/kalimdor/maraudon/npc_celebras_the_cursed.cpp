/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################
*/

/* ScriptData
SDName: Boss_Celebras_the_Cursed
SD%Complete: 100
SDComment:
SDCategory: Maraudon
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_WRATH             = 21807,
    SPELL_ENTANGLINGROOTS   = 12747,
    SPELL_CORRUPT_FORCES    = 21968,
	SPELL_Gelassenheit		= 21793
};

struct MANGOS_DLL_DECL celebras_the_cursedAI : public ScriptedAI
{
    celebras_the_cursedAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 Wrath_Timer;
    uint32 EntanglingRoots_Timer;
    uint32 CorruptForces_Timer;
	uint32 Gelassenheit_Timer;

    void Reset()
    {
        Wrath_Timer = 8000;
        EntanglingRoots_Timer = 2000;
        CorruptForces_Timer = 30000;
		Gelassenheit_Timer	= 25000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
	
    void JustDied(Unit*)
    {
        m_creature->SummonCreature(13716, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 600000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Wrath
        if (Wrath_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target)
                DoCastSpellIfCan(target,SPELL_WRATH);
            Wrath_Timer = random(6000,8000);
        }else Wrath_Timer -= diff;

        //Gelassenheit
        if (Gelassenheit_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target)
                DoCastSpellIfCan(target,SPELL_Gelassenheit);
            Gelassenheit_Timer = random(20000,25000);
        }else Gelassenheit_Timer -= diff;

        //EntanglingRoots
        if (EntanglingRoots_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ENTANGLINGROOTS);
            EntanglingRoots_Timer = random(18000,23000);
        }else EntanglingRoots_Timer -= diff;

        //CorruptForces
        if (CorruptForces_Timer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCastSpellIfCan(m_creature,SPELL_CORRUPT_FORCES);
            CorruptForces_Timer = random(14000,18000);
        }else CorruptForces_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_celebras_the_cursed(Creature* pCreature)
{
    return new celebras_the_cursedAI(pCreature);
}

void AddSC_boss_celebras_the_cursed()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "celebras_the_cursed";
    pNewScript->GetAI = &GetAI_celebras_the_cursed;
    pNewScript->RegisterSelf();
}