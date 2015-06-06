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

/* Maleki der Leichenblasse

-> Manaentzug nur auf Player mit Mana!
*/

#include "precompiled.h"
#include "stratholme.h"

#define SPELL_FROSTBLITZ	    17503
#define SPELL_BLUTSAUGER		20743
#define SPELL_MANA_ENTZIEHEN    17243
#define SPELL_EISGRABMAL		16869

struct MANGOS_DLL_DECL maleki_der_leichenblasseAI : public ScriptedAI
{
    maleki_der_leichenblasseAI(Creature *c) : ScriptedAI(c)
    {
    }

    uint32	Frostblitz_Timer;
    uint32	Eisgrabmal_Timer;
    uint32	Blutsauger_Timer;
    uint32	Mana_entziehen_Timer;
    Unit*	target;

    void Reset()
    {
        Frostblitz_Timer = 1000;
        Eisgrabmal_Timer = 16000;
        Blutsauger_Timer = 14000;
        Mana_entziehen_Timer = 15000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        if (pInstance) {
            pInstance->SetData(TYPE_PALLID,IN_PROGRESS);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }

        //Frostbolt
        if (Frostblitz_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FROSTBLITZ);
            Frostblitz_Timer = random(4000,5000);
        } else Frostblitz_Timer -= diff;

        //IceTomb
        if (Eisgrabmal_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_EISGRABMAL);
            Eisgrabmal_Timer = random(10000,20000);
        } else Eisgrabmal_Timer -= diff;

        //DrainLife
        if (Blutsauger_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_BLUTSAUGER);
            Blutsauger_Timer = random(15000,20000);
        } else Blutsauger_Timer -= diff;

        if (Mana_entziehen_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_MANA_ENTZIEHEN);
            Mana_entziehen_Timer = random(15000,20000);
        } else Mana_entziehen_Timer -=diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_maleki_der_leichenblasse(Creature *_Creature)
{
    return new maleki_der_leichenblasseAI (_Creature);
}

void AddSC_maleki_der_leichenblasse()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="maleki_der_leichenblasse";
    newscript->GetAI = &GetAI_maleki_der_leichenblasse;
    newscript->RegisterSelf();
}