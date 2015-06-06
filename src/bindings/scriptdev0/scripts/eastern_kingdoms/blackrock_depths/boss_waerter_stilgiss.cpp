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
/* W�rter Stilgiss */

#include "precompiled.h"



#define SPELL_FROSTZAUBERSCHUTZ		15044
#define SPELL_FROSTRUESTUNG			12544
#define SPELL_FROSTNOVA				12674
#define SPELL_FROSTBLITZ			12675

struct MANGOS_DLL_DECL waerter_stilgissAI : public ScriptedAI
{
    waerter_stilgissAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 frostzauberschutz_timer;
    uint32 frostnova_timer;
    uint32 frostblitz_timer;

    void Reset()
    {
        frostzauberschutz_timer = 3000;
        frostnova_timer = 6000;
        frostblitz_timer= 5000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Frieren tut euch gut!",LANG_UNIVERSAL,NULL);
        DoCastSpellIfCan(m_creature,SPELL_FROSTRUESTUNG);
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
        if (frostzauberschutz_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FROSTZAUBERSCHUTZ);
            frostzauberschutz_timer = random(61000,63000);
        }
        else
        {
            frostzauberschutz_timer -= diff;
        }

        if (frostblitz_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FROSTBLITZ);
            frostblitz_timer = random(5000,8000);
        }
        else
        {
            frostblitz_timer -=diff;
        }

        if (frostnova_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTNOVA);
            frostnova_timer = random(10000,12000);
        }
        else
        {
            frostnova_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_waerter_stilgiss(Creature *_Creature)
{
    return new waerter_stilgissAI (_Creature);
}


void AddSC_boss_waerter_stilgiss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_waerter_stilgiss";
    newscript->GetAI = &GetAI_waerter_stilgiss;
    newscript->RegisterSelf();
}
