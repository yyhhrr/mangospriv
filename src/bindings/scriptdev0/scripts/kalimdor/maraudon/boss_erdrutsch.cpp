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

#include "precompiled.h"

#define felssturz	21808
#define trampeln	5568
#define wegschlagen	18670

struct MANGOS_DLL_DECL boss_erdrutschAI : public ScriptedAI
{
    boss_erdrutschAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 felssturz_timer;
    uint32 trampeln_timer;
    uint32 wegschlagen_timer;

    void Reset()
    {
        felssturz_timer 	= 1000;
        trampeln_timer 		= 5000;
        wegschlagen_timer 	= 8000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (felssturz_timer < diff && m_creature->GetHealthPercent() < 50.0f)
        {
            DoCastSpellIfCan(m_creature->getVictim(),felssturz);
            felssturz_timer = random(640000,650000);
        } else felssturz_timer -= diff;

        if (trampeln_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),trampeln);
            trampeln_timer = random(9000,12000);
        } else trampeln_timer -= diff;

        if (wegschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),wegschlagen);
            wegschlagen_timer = random(12000,16000);
        } else wegschlagen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_erdrutsch(Creature *_Creature)
{
    return new boss_erdrutschAI (_Creature);
}

void AddSC_boss_erdrutsch()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_erdrutsch";
    newscript->GetAI = &GetAI_boss_erdrutsch;
    newscript->RegisterSelf();
}