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

#define toedlicher_biss	16495

struct MANGOS_DLL_DECL boss_faulschnapperAI : public ScriptedAI
{
    boss_faulschnapperAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 toedlicher_biss_timer;

    void Reset()
    {
        toedlicher_biss_timer = 3000;
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

        if (toedlicher_biss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),toedlicher_biss);
            toedlicher_biss_timer = random(6000,10000);
        } else toedlicher_biss_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_faulschnapper(Creature *_Creature)
{
    return new boss_faulschnapperAI (_Creature);
}

void AddSC_boss_faulschnapper()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_faulschnapper";
    newscript->GetAI = &GetAI_boss_faulschnapper;
    newscript->RegisterSelf();
}