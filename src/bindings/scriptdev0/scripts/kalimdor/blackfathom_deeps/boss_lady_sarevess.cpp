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

#define frostnova			865
#define gabelblitzschlag	8435
#define verlangsamen		246

struct MANGOS_DLL_DECL boss_lady_sarevessAI : public ScriptedAI
{
    boss_lady_sarevessAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 frostnova_timer;
    uint32 gabelblitzschlag_timer;
    uint32 verlangsamen_timer;

    void Reset()
    {
        frostnova_timer = 3000;
        gabelblitzschlag_timer = 7000;
        verlangsamen_timer = 15000;
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Ihr solltet nicht hier sein! T\303\266tet sie!",LANG_UNIVERSAL,NULL);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (frostnova_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),frostnova);
            frostnova_timer = random(9000,14000);
        } else frostnova_timer -= diff;

        if (gabelblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),gabelblitzschlag);
            gabelblitzschlag_timer = random(7000,11000);
        } else gabelblitzschlag_timer -= diff;

        if (verlangsamen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),verlangsamen);
            verlangsamen_timer = random(12000,20000);
        } else verlangsamen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lady_sarevess(Creature *_Creature)
{
    return new boss_lady_sarevessAI (_Creature);
}

void AddSC_boss_lady_sarevess()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lady_sarevess";
    newscript->GetAI = &GetAI_boss_lady_sarevess;
    newscript->RegisterSelf();
}

