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

#define blinzeln	21655
#define mehrfachschuss	21390
#define rauchbombe	7964
#define schiessen	16100

struct MANGOS_DLL_DECL boss_lord_schlangenzungeAI : public ScriptedAI
{
    boss_lord_schlangenzungeAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blinzeln_timer;
    uint32 mehrfachschuss_timer;
    uint32 rauchbombe_timer;
    uint32 schiessen_timer;

    void Reset()
    {
        blinzeln_timer = 4000;
        mehrfachschuss_timer = 3000;
        rauchbombe_timer = 5000;
        schiessen_timer = 2000;
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

        if (blinzeln_timer < diff)
        {
            DoCastSpellIfCan(m_creature,blinzeln);
            blinzeln_timer = random(10000,12000);
        } else blinzeln_timer -= diff;

        if (mehrfachschuss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),mehrfachschuss);
            mehrfachschuss_timer = random(4000,10000);
        } else mehrfachschuss_timer -= diff;

        if (rauchbombe_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),rauchbombe);
            rauchbombe_timer = random(8000,12000);
        } else rauchbombe_timer -= diff;

        if (schiessen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schiessen);
            schiessen_timer = random(4000,8000);
        } else schiessen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lord_schlangenzunge(Creature *_Creature)
{
    return new boss_lord_schlangenzungeAI (_Creature);
}

void AddSC_boss_lord_schlangenzunge()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_schlangenzunge";
    newscript->GetAI = &GetAI_boss_lord_schlangenzunge;
    newscript->RegisterSelf();
}

