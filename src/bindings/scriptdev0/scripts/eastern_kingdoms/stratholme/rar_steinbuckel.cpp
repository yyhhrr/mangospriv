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

/* Steinbuckel */


#include "precompiled.h"

#define SPELL_HEIMTUECKISCHES_ZERFLEISCHEN	14331


struct MANGOS_DLL_DECL steinbuckelAI : public ScriptedAI
{
    steinbuckelAI(Creature *c) : ScriptedAI(c) {}

    uint32 zauber1_timer;
    Unit* target;
    void Reset()
    {//	ZauberName
        zauber1_timer = 5000;//10 sec
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Ich werde Euch zerfleischen!",LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit* Killer)
    {

    }

    void KilledUnit(Unit *victim)
    {

    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }


        if (zauber1_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            int i=0;
            while (m_creature->GetDistance(target)>5 && i>10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,SPELL_HEIMTUECKISCHES_ZERFLEISCHEN);
            zauber1_timer = random(4000,8000);
        }
        else
        {
            zauber1_timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_steinbuckel(Creature *_Creature)
{
    return new steinbuckelAI (_Creature);
}

void AddSC_steinbuckel()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_rar_steinbuckel";
    newscript->GetAI = &GetAI_steinbuckel;
    newscript->RegisterSelf();
}
/* Scriptbeschreibung: -> Alle 4 sek Finsterer Stoß */
