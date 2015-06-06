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
/* Archivar Galford */

#include "precompiled.h"

#define SPELL_MANABRAND				14033
#define SPELL_PSYCHISCHER_SCHREI	13704
#define SPELL_SCHATTENSCHILD		12040
#define SPELL_SCHATTENWORT_SCHMERZ	14032

struct MANGOS_DLL_DECL verhoermeisterin_gerstahnAI : public ScriptedAI
{
    verhoermeisterin_gerstahnAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 Manabrand_timer;
    uint32 Psychischer_Schrei_timer;
    uint32 Schattenschild_timer;
    uint32 Schattenwort_Schmerz_timer;

    void Reset()
    {
        Manabrand_timer = 10000;
        Psychischer_Schrei_timer = 1000;
        Schattenschild_timer = 31000;
        Schattenwort_Schmerz_timer = 7000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature, SPELL_SCHATTENSCHILD);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }


        if (Manabrand_timer < diff)
        {
            int mana =0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (target->getPowerType()!=POWER_MANA)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                mana++;
                if (mana == 10)
                {
                    goto end;
                }
            }
            DoCastSpellIfCan(target,SPELL_MANABRAND);
end:
            Manabrand_timer = random(8000,12000);
        }
        else
        {
            Manabrand_timer -= diff;
        }

        if (Psychischer_Schrei_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_PSYCHISCHER_SCHREI);
            Psychischer_Schrei_timer = random(10000,15000);
        }
        else
        {
            Psychischer_Schrei_timer -=diff;
        }


        if (Schattenschild_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHATTENSCHILD);
            Schattenschild_timer = random(32000,35000);
        }
        else
        {
            Schattenschild_timer -=diff;
        }

        if (Schattenwort_Schmerz_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_SCHATTENWORT_SCHMERZ);
            Schattenwort_Schmerz_timer = random(8000,10000);
        }
        else
        {
            Schattenwort_Schmerz_timer -=diff;
        }


        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_verhoermeisterin_gerstahn(Creature *_Creature)
{
    return new verhoermeisterin_gerstahnAI (_Creature);
}


void AddSC_boss_verhoermeisterin_gerstahn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_verhoermeisterin_gerstahn";
    newscript->GetAI = &GetAI_verhoermeisterin_gerstahn;
    newscript->RegisterSelf();
}
