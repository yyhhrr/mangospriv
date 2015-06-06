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
#												   	 Archivar Galford			#
#################################################################################

ScriptName: Archivar Galford
Instanz: Stratholme
Staus in %: 100%
Abgeschlossen?: Ja

*/

#include "precompiled.h"

#define SPELL_BRENNENDE_WINDE		17293
#define SPELL_FEUERNOVA				17366
#define SPELL_PYROSCHLAG			17274

struct MANGOS_DLL_DECL archivar_galfordAI : public ScriptedAI
{
    archivar_galfordAI(Creature *c) : ScriptedAI(c) {}

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
        m_creature->MonsterYell("Das werdet Ihr bereuen!",LANG_UNIVERSAL,NULL);

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
            switch (rand()%2)
            {
            case 0:
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_PYROSCHLAG);

            case 1:
                DoCastSpellIfCan(m_creature,SPELL_FEUERNOVA);
            }
            zauber1_timer = random(8000,12000);
        }
        else
        {
            zauber1_timer -= diff;
        }

        if (zauber2_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_BRENNENDE_WINDE);
            zauber2_timer = random(10000,15000);
        }
        else
        {
            zauber2_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_archivar_galford(Creature *_Creature)
{
    return new archivar_galfordAI (_Creature);
}


void AddSC_archivar_galford()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="archivar_galford";
    newscript->GetAI = &GetAI_archivar_galford;
    newscript->RegisterSelf();
}

/*Scriptbeschreibung: -> alle 10 bis 15 sek Brennende Winde auf random target
					  -> alle 8 bis 12 sek zus�tzlich Feuernova oder Pyroschlag*/
