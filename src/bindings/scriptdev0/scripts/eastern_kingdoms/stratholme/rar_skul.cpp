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

/*
# Copyright (C) 2009 SiN-Gaming Scripts <https://www.sin-gaming.net/>
*/


#include "precompiled.h"



//	   | Zauberbezeichnung				SpellID
#define SPELL_ARKANBLITZ				15230
#define SPELL_FROSTBLITZ				16799
#define SPELL_FROSTRUESTUNG				12544
#define SPELL_FROSTSCHOCK				15499


struct MANGOS_DLL_DECL skulAI : public ScriptedAI
{
    skulAI(Creature *c) : ScriptedAI(c) {}

    uint32 zauber1_timer;
    Unit* target;

    void Reset()
    {//	ZauberName
        zauber1_timer = 10000;//10 sec
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }



    void Aggro(Unit *who)
    {
        //m_creature->MonsterYell("Ich frier euch ein!",LANG_UNIVERSAL,NULL);
        DoCastSpellIfCan(m_creature,SPELL_FROSTRUESTUNG);
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
        {	//							  |Zauberbezeichnung|
            switch (rand()%3)
            {
            case 0:

                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_ARKANBLITZ);
                break;

            case 1:

                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_FROSTBLITZ);
                break;
            case 2:

                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_FROSTSCHOCK);
                break;

            }
            zauber1_timer = random(2000,10000);
        }
        else
        {
            zauber1_timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_skul(Creature *_Creature)
{
    return new skulAI (_Creature);
}

void AddSC_skul()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_rar_skul";
    newscript->GetAI = &GetAI_skul;
    newscript->RegisterSelf();
}

/*Scriptbeschreibung: -> bei Aggro einmalig Frostr�stungsbuff
					  -> alle 4 sek Arkanblitz, Frostblitz oder Frostschock
*/