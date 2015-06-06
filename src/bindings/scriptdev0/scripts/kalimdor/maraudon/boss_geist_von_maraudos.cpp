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

#define finsterer_stoss	14030
#define manifestation_endet	21965
#define netz	18651
#define werfen	15620

struct MANGOS_DLL_DECL boss_geist_von_maraudosAI : public ScriptedAI
{
    boss_geist_von_maraudosAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 finsterer_stoss_timer;
    uint32 manifestation_endet_timer;
    uint32 netz_timer;
    uint32 werfen_timer;

    void Reset()
    {
        finsterer_stoss_timer = 2000;
//manifestation_endet_timer = ;
        netz_timer = 3000;
        werfen_timer = 4000;
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

        if (finsterer_stoss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),finsterer_stoss);
            finsterer_stoss_timer = random(3000,8000);
        } else finsterer_stoss_timer -= diff;

//if (manifestation_endet_timer < diff)
//{
//DoCastSpellIfCan(,manifestation_endet);
//manifestation_endet_timer = random(,);
//}else manifestation_endet_timer -= diff;

        if (netz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),netz);
            netz_timer = random(6000,8000);
        } else netz_timer -= diff;

        if (werfen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),werfen);
            werfen_timer = random(2000,8000);
        } else werfen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_geist_von_maraudos(Creature *_Creature)
{
    return new boss_geist_von_maraudosAI (_Creature);
}

void AddSC_boss_geist_von_maraudos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_geist_von_maraudos";
    newscript->GetAI = &GetAI_boss_geist_von_maraudos;
    newscript->RegisterSelf();
}