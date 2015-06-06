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

#define hakennetz	14030
#define manifestation_endet	21965
#define mehrfachschuss	18651
#define schiessen	15620

struct MANGOS_DLL_DECL boss_geist_von_vengAI : public ScriptedAI
{
    boss_geist_von_vengAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 hakennetz_timer;
    uint32 manifestation_endet_timer;
    uint32 mehrfachschuss_timer;
    uint32 schiessen_timer;

    void Reset()
    {
        hakennetz_timer = 5000;
//manifestation_endet_timer = ;
        mehrfachschuss_timer = 1000;
        schiessen_timer = 4000;
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

        if (hakennetz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),hakennetz);
            hakennetz_timer = random(3000,8000);
        } else hakennetz_timer -= diff;

//if (manifestation_endet_timer < diff)
//{
//DoCastSpellIfCan(,manifestation_endet);
//manifestation_endet_timer = random(,);
//}else manifestation_endet_timer -= diff;

        if (mehrfachschuss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),mehrfachschuss);
            mehrfachschuss_timer = random(3000,12000);
        } else mehrfachschuss_timer -= diff;

        if (schiessen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schiessen);
            schiessen_timer = random(2000,8000);
        } else schiessen_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_geist_von_veng(Creature *_Creature)
{
    return new boss_geist_von_vengAI (_Creature);
}

void AddSC_boss_geist_von_veng()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_geist_von_veng";
    newscript->GetAI = &GetAI_boss_geist_von_veng;
    newscript->RegisterSelf();
}