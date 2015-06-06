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
#												 Ramstein der Verschlinger		#
#################################################################################

ScriptName: Ramstein der Verschlinger
Instanz: Stratholme
Staus in %: 100%
Abgeschlossen?: Ja
*/

#include "precompiled.h"
#include "stratholme.h"

#define SPELL_TRAMPELN      5568
#define SPELL_KNOCKOUT	    17307

#define C_MINDLESS_UNDEAD   11030

struct MANGOS_DLL_DECL ramstein_der_verschlingerAI : public ScriptedAI
{
    ramstein_der_verschlingerAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    Unit* target;
    uint32 Trampeln_Timer;
    uint32 Knockout_Timer;

    void Reset()
    {
        Trampeln_Timer = 3000;
        Knockout_Timer = 12000;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit* Killer)
    {
	    if (pInstance)
			pInstance->SetData(TYPE_RAMSTEIN,DONE);
			
        for(uint8 i = 0; i < 30; i++)
			m_creature->SummonCreature(C_MINDLESS_UNDEAD, 3969.35f, -3391.87f, 119.11f, 5.91f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }

        //Trample
        if (Trampeln_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_TRAMPELN);
            Trampeln_Timer = random(6000,8000);
        } else Trampeln_Timer -= diff;

        //Knockout
        if (Knockout_Timer < diff)
        {
            int i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)>10 && i<10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KNOCKOUT);
            Knockout_Timer = random(9000,11000);
        } else Knockout_Timer -= diff;



        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_ramstein_der_verschlinger(Creature *_Creature)
{
    return new ramstein_der_verschlingerAI (_Creature);
}

void AddSC_ramstein_der_verschlinger()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="ramstein_der_verschlinger";
    newscript->GetAI = &GetAI_ramstein_der_verschlinger;
    newscript->RegisterSelf();
}