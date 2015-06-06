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
/* General Zornesschmied */

#include "precompiled.h"


#define SPELL_RUESTUNG_ZERREISSEN		15572

struct MANGOS_DLL_DECL general_zornesschmiedAI : public ScriptedAI
{
    general_zornesschmiedAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 ruestung_zerreissen_timer;


    void Reset()
    {
        ruestung_zerreissen_timer = 2000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }

        //RavenousClaw
        if (ruestung_zerreissen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_RUESTUNG_ZERREISSEN);
            ruestung_zerreissen_timer = random(10000,12000);
        }
        else
        {
            ruestung_zerreissen_timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_general_zornesschmied(Creature *_Creature)
{
    return new general_zornesschmiedAI (_Creature);
}

void AddSC_boss_general_zornesschmied()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_general_zornesschmied";
    newscript->GetAI = &GetAI_general_zornesschmied;
    newscript->RegisterSelf();
}