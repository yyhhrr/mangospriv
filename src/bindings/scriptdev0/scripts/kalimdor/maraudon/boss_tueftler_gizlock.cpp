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

#define blitzstrahlbombe	29419
#define bombe	9143
#define goblindrachenwerfer	13601
#define schiessen	16100

struct MANGOS_DLL_DECL boss_tueftler_gizlockAI : public ScriptedAI
{
    boss_tueftler_gizlockAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blitzstrahlbombe_timer;
    uint32 bombe_timer;
    uint32 goblindrachenwerfer_timer;
    uint32 schiessen_timer;

    void Reset()
    {
        blitzstrahlbombe_timer = 8000;
        bombe_timer = 4000;
        goblindrachenwerfer_timer = 10000;
        schiessen_timer = 3000;
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

        if (blitzstrahlbombe_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),blitzstrahlbombe);
            blitzstrahlbombe_timer = random(12000,14000);
        } else blitzstrahlbombe_timer -= diff;

        if (bombe_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),bombe);
            bombe_timer = random(6000,10000);
        } else bombe_timer -= diff;

        if (goblindrachenwerfer_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),goblindrachenwerfer);
            goblindrachenwerfer_timer = random(10000,20000);
        } else goblindrachenwerfer_timer -= diff;

        if (schiessen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schiessen);
            schiessen_timer = random(3000,7000);
        } else schiessen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_tueftler_gizlock(Creature *_Creature)
{
    return new boss_tueftler_gizlockAI (_Creature);
}

void AddSC_boss_tueftler_gizlock()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_tueftler_gizlock";
    newscript->GetAI = &GetAI_boss_tueftler_gizlock;
    newscript->RegisterSelf();
}

