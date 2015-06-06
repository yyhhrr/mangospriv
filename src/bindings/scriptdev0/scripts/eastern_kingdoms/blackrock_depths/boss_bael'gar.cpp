/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#													 Schwarzfelstiefen			#
#												   			...					#
#################################################################################

ScriptName:
Instanz: Schwarzfelstiefen
Staus in %: 100%
Abgeschlossen?: Ja

*/
/* Bael'Gar */

#include "precompiled.h"

#define SPELL_BRUT_VON_BAELGAR_BESCHWOEREN		13895

struct MANGOS_DLL_DECL boss_baelgarAI : public ScriptedAI
{
    boss_baelgarAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 brut_beschwoeren_timer;

    void Reset()
    {
        brut_beschwoeren_timer = 5000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (brut_beschwoeren_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target, SPELL_BRUT_VON_BAELGAR_BESCHWOEREN);

            brut_beschwoeren_timer = random(6000,10000);
        }
        else
        {
            brut_beschwoeren_timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baelgar(Creature *_Creature)
{
    return new boss_baelgarAI (_Creature);
}

void AddSC_boss_baelgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baelgar";
    newscript->GetAI = &GetAI_boss_baelgar;
    newscript->RegisterSelf();
}
