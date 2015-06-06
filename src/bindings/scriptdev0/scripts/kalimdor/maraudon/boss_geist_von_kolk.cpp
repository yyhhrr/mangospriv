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

#define demoralisierender_ruf	16244
#define manifestation_endet	21965
#define schlag	14516


struct MANGOS_DLL_DECL boss_geist_von_kolkAI : public ScriptedAI
{
    boss_geist_von_kolkAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 demoralisierender_ruf_timer;
    uint32 manifestation_endet_timer;
    uint32 schlag_timer;

    void Reset()
    {
        demoralisierender_ruf_timer = 2000;
//manifestation_endet_timer = ;
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

        if (demoralisierender_ruf_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),demoralisierender_ruf);
            demoralisierender_ruf_timer = random(30500,31000);
        } else demoralisierender_ruf_timer -= diff;

//if (manifestation_endet_timer < diff)
//{
//DoCastSpellIfCan(,manifestation_endet);
//manifestation_endet_timer = random(,);
//}else manifestation_endet_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(3000,4000);
        } else schlag_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_geist_von_kolk(Creature *_Creature)
{
    return new boss_geist_von_kolkAI (_Creature);
}

void AddSC_boss_geist_von_kolk()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_geist_von_kolk";
    newscript->GetAI = &GetAI_boss_geist_von_kolk;
    newscript->RegisterSelf();
}

