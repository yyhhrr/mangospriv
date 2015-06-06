/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#														Stratholme				#
#												   	 Timmy der Grausame			#
#################################################################################

ScriptName: Timmy der Grausame
Instanz: Stratholme
Staus in %: 100%
Abgeschlossen?: Ja

*/

#include "precompiled.h"

#define SPELL_GEFRAESSIGE_KLAUE		17470
#define SPELL_WUTANFALL				8599

struct MANGOS_DLL_DECL timmy_der_grausameAI : public ScriptedAI
{
    timmy_der_grausameAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 zauber1_timer;
    uint32 zauber2_timer;

    void Reset()
    {
        zauber1_timer = 10000;
        zauber2_timer = 1000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("TIMMY!",LANG_UNIVERSAL,NULL);

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
        if (zauber1_timer < diff)
        {
            //Cast
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)>5)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            }
            DoCastSpellIfCan(target,SPELL_GEFRAESSIGE_KLAUE);
            //15 seconds until we should cast this again
            zauber1_timer = random(10000,15000);

        }
        else
        {
            zauber1_timer -= diff;
        }

        if (zauber2_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_WUTANFALL);
            zauber2_timer = random(125000,130000);
        }
        else
        {
            zauber2_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_timmy_der_grausame(Creature *_Creature)
{
    return new timmy_der_grausameAI (_Creature);
}

void AddSC_timmy_der_grausame()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="timmy_der_grausame";
    newscript->GetAI = &GetAI_timmy_der_grausame;
    newscript->RegisterSelf();
}

/*Scriptbeschreibung: -> 1 Sek nach Aggro cast Wutanfall alle 2 min recasting Wutanfall
					  -> alle 10 - 15 sek Gefr��ige Klaue auf einen Randomspieler der max 5m vom Boss entfernt steht.
					     Tut das keiner, geht der Cast auf Tank.
*/
