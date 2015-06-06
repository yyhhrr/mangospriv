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

#define ruestung_zerreissen	15572
#define schlachtruf	9128

struct MANGOS_DLL_DECL boss_geist_von_magraAI : public ScriptedAI
{
    boss_geist_von_magraAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 ruestung_zerreissen_timer;
    uint32 schlachtruf_timer;

    void Reset()
    {
        ruestung_zerreissen_timer = 3000;
        schlachtruf_timer = 1000;
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

        if (ruestung_zerreissen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),ruestung_zerreissen);
            ruestung_zerreissen_timer = random(3000,4000);
        } else ruestung_zerreissen_timer -= diff;

        if (schlachtruf_timer < diff)
        {
            DoCastSpellIfCan(m_creature,schlachtruf);
            schlachtruf_timer = random(120000,125000);
        } else schlachtruf_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_geist_von_magra(Creature *_Creature)
{
    return new boss_geist_von_magraAI (_Creature);
}

void AddSC_boss_geist_von_magra()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_geist_von_magra";
    newscript->GetAI = &GetAI_boss_geist_von_magra;
    newscript->RegisterSelf();
}