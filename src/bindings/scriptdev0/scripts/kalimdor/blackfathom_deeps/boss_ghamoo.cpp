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

#define trampeln	5568

struct MANGOS_DLL_DECL boss_ghamooAI : public ScriptedAI
{
    boss_ghamooAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 trampeln_timer;

    void Reset()
    {
        trampeln_timer = 8000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (trampeln_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),trampeln);
            trampeln_timer = random(10000,14000);
        } else trampeln_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ghamoo(Creature *_Creature)
{
    return new boss_ghamooAI (_Creature);
}

void AddSC_boss_ghamoo()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ghamoo";
    newscript->GetAI = &GetAI_boss_ghamoo;
    newscript->RegisterSelf();
}

