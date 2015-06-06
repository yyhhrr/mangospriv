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

/* Pyromant Weisenkorn */

#include "precompiled.h"



#define SPELL_FEUERZAUBERSCHUTZ		15041
#define SPELL_FLAMMENSCHOCK			15616
#define SPELL_SCHMELZSCHLAG			15095
#define SPELL_VERSENGENDES_TOTEM	15038

struct MANGOS_DLL_DECL pyromant_weisenkornAI : public ScriptedAI
{
    pyromant_weisenkornAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 feuerzauberschutz_timer;
    uint32 flammenschock_timer;
    uint32 schmelzschlag_timer;
    uint32 versengendes_totem_timer;

    void Reset()
    {
        feuerzauberschutz_timer = 60000;
        flammenschock_timer = 1000;
        schmelzschlag_timer= 3000;
        versengendes_totem_timer = 10000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit*)
    {
        DoCastSpellIfCan(m_creature,SPELL_FEUERZAUBERSCHUTZ);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
            return;

        //RavenousClaw
        if (feuerzauberschutz_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FEUERZAUBERSCHUTZ);
            feuerzauberschutz_timer = random(61000,63000);
        }
        else
        {
            feuerzauberschutz_timer -= diff;
        }

        if (flammenschock_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FLAMMENSCHOCK);
            flammenschock_timer = random(7000,8000);
        }
        else
        {
            flammenschock_timer -=diff;
        }

        if (schmelzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SCHMELZSCHLAG);
            schmelzschlag_timer = random(4000,6000);
        }
        else
        {
            schmelzschlag_timer -=diff;
        }
        if (versengendes_totem_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_VERSENGENDES_TOTEM);
            versengendes_totem_timer = random(10000,15000);
        }
        else
        {
            versengendes_totem_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_pyromant_weisenkorn(Creature *_Creature)
{
    return new pyromant_weisenkornAI (_Creature);
}

void AddSC_boss_pyromant_weisenkorn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_pyromant_weisenkorn";
    newscript->GetAI = &GetAI_pyromant_weisenkorn;
    newscript->RegisterSelf();
}
