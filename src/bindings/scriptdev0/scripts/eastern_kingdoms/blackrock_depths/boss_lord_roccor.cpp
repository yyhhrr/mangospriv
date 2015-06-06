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
/* Lord Roccor */

#include "precompiled.h"



#define SPELL_ERDSCHOCK			13728
#define SPELL_FLAMMENSCHOCK		13729
#define SPELL_BEBENDE_ERDE		6524

struct MANGOS_DLL_DECL lord_roccorAI : public ScriptedAI
{
    lord_roccorAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 Erdschock_timer;
    uint32 Flammenschock_timer;
    uint32 Bebende_Erde_timer;

    void Reset()
    {
        Erdschock_timer = 3000;
        Flammenschock_timer = 5000;
        Bebende_Erde_timer = 8000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Das werdet ihr bereuen!",LANG_UNIVERSAL,NULL);

    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (Erdschock_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_ERDSCHOCK);

            Erdschock_timer = random(4000,6000);
        }
        else
        {
            Erdschock_timer -= diff;
        }

        if (Flammenschock_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FLAMMENSCHOCK);

            Flammenschock_timer = random(5000,7000);
        }
        else
        {
            Flammenschock_timer -= diff;
        }
        if (Bebende_Erde_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BEBENDE_ERDE);
            Bebende_Erde_timer = random(5000,7000);
        }
        else
        {
            Bebende_Erde_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_lord_roccor(Creature *_Creature)
{
    return new lord_roccorAI (_Creature);
}

void AddSC_boss_lord_roccor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_roccor";
    newscript->GetAI = &GetAI_lord_roccor;
    newscript->RegisterSelf();
}