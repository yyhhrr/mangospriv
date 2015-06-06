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
/* Golemlord Argelmach */

#include "precompiled.h"



#define SPELL_BLITZSCHLAGSCHILD			15507
#define SPELL_KETTENBLITZSCHLAG			15305
#define SPELL_SCHOCK					15605

struct MANGOS_DLL_DECL golemlord_argelmachAI : public ScriptedAI
{
    golemlord_argelmachAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 blitzschlagschild_timer;
    uint32 kettenblitzschlag_timer;
    uint32 schock_timer;

    void Reset()
    {
        blitzschlagschild_timer = 12000;
        kettenblitzschlag_timer = 2000;
        schock_timer = 4000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Die Elemente werden Euch holen!",LANG_UNIVERSAL,NULL);
        DoCastSpellIfCan(m_creature,SPELL_BLITZSCHLAGSCHILD);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (blitzschlagschild_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BLITZSCHLAGSCHILD);
            blitzschlagschild_timer = random(10000,12000);
        }
        else
        {
            blitzschlagschild_timer -= diff;
        }

        if (kettenblitzschlag_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_KETTENBLITZSCHLAG);
            kettenblitzschlag_timer = random(6000,8000);
        }
        else
        {
            kettenblitzschlag_timer -=diff;
        }

        if (schock_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_SCHOCK);
            schock_timer = random(6000,8000);
        }
        else
        {
            schock_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_golemlord_argelmach(Creature *_Creature)
{
    return new golemlord_argelmachAI (_Creature);
}

void AddSC_boss_golemlord_argelmach()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_golemlord_argelmach";
    newscript->GetAI = &GetAI_golemlord_argelmach;
    newscript->RegisterSelf();
}