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

#define netz	6533

struct MANGOS_DLL_DECL boss_gelihastAI : public ScriptedAI
{
    boss_gelihastAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 netz_timer;

    void Reset()
    {
        netz_timer = 3000;
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Mrrrrrglglglglrrrr",LANG_UNIVERSAL,NULL);
        m_creature->CallForHelp(200.0f);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (netz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),netz);
            netz_timer = random(7000,11000);
        } else netz_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gelihast(Creature *_Creature)
{
    return new boss_gelihastAI (_Creature);
}

void AddSC_boss_gelihast()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gelihast";
    newscript->GetAI = &GetAI_boss_gelihast;
    newscript->RegisterSelf();
}

