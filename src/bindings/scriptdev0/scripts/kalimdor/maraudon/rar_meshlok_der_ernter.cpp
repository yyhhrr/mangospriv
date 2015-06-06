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

#define kriegsdonner	15593
#define schlag			13446

struct MANGOS_DLL_DECL rar_meshlok_der_ernterAI : public ScriptedAI
{
    rar_meshlok_der_ernterAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 kriegsdonner_timer;
    uint32 schlag_timer;

    void Reset()
    {
        kriegsdonner_timer = 4000;
        schlag_timer = 3000;
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

        if (kriegsdonner_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kriegsdonner);
            kriegsdonner_timer = random(4000,10000);
        } else kriegsdonner_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(4000,6000);
        } else schlag_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_rar_meshlok_der_ernter(Creature *_Creature)
{
    return new rar_meshlok_der_ernterAI (_Creature);
}

void AddSC_rar_meshlok_der_ernter()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="rar_meshlok_der_ernter";
    newscript->GetAI = &GetAI_rar_meshlok_der_ernter;
    newscript->RegisterSelf();
}