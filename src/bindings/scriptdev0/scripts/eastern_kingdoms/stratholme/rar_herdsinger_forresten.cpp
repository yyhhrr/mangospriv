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
#define SPELL_DEMORALISIERENDER_RUF		16244
#define SPELL_MEHRFACHSCHUSS			14443
#define SPELL_SCHIESSEN					16100
#define SPELL_VERZAUBERNDES_SCHLAFLIED	16798


struct MANGOS_DLL_DECL herdsinger_forrestenAI : public ScriptedAI
{
    herdsinger_forrestenAI(Creature *c) : ScriptedAI(c) {}

    uint32 zauber1_timer;
    uint32 zauber2_timer;
    Unit* target;

    void Reset()
    {//	ZauberName
        zauber1_timer = 4000;//10 sec
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }



    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Sterbt!",LANG_UNIVERSAL,NULL);
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
            switch (rand()%5)
            {
            case 0:

                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_SCHIESSEN);
                break;

            case 1:

                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_SCHIESSEN);
                break;
            case 2:

                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                DoCastSpellIfCan(target,SPELL_MEHRFACHSCHUSS);
                break;

            case 3:
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                while (target->HasAura(SPELL_VERZAUBERNDES_SCHLAFLIED));
                {
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                }
                DoCastSpellIfCan(target,SPELL_VERZAUBERNDES_SCHLAFLIED);
                break;

            case 4:
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_DEMORALISIERENDER_RUF);
                break;
            }
            zauber1_timer = random(2000,7000);
        }
        else
        {
            zauber1_timer -= diff;
        }
        /////////////////////////////////////////////////////////////


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_herdsinger_forresten(Creature *_Creature)
{
    return new herdsinger_forrestenAI (_Creature);
}

void AddSC_herdsinger_forresten()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="herdsinger_forresten";
    newscript->GetAI = &GetAI_herdsinger_forresten;
    newscript->RegisterSelf();
}


/*Scriptbeschreibung: -> bei Aggro einmalig Frostr�stungsbuff
					  -> alle 4 sek Arkanblitz, Frostblitz oder Frostschock
*/
