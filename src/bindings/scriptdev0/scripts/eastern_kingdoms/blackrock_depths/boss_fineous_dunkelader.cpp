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
/* Fineous Dunkelader */

#include "precompiled.h"


#define SPELL_AURA_DER_HINGABE		8258
#define SPELL_HEILIGER_STOSS		13953
#define SPELL_HEILIGES_LICHT		15493
#define SPELL_SIEGEL_DER_ABRECHNUNG	15346
#define SPELL_TRITT					11978

struct MANGOS_DLL_DECL fineous_dunkeladerAI : public ScriptedAI
{
    fineous_dunkeladerAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 aura_der_hingabe_timer;
    uint32 heiliger_stoss_timer;
    uint32 heiliges_licht_timer;
    uint32 siegel_der_abrechnung_timer;
    uint32 tritt_timer;

    void Reset()
    {
        aura_der_hingabe_timer = 2400000;
        heiliger_stoss_timer = 2000;
        heiliges_licht_timer = 10000;
        siegel_der_abrechnung_timer = 30000;
        tritt_timer = 7000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Frieren tut euch gut!",LANG_UNIVERSAL,NULL);
        DoCastSpellIfCan(m_creature,SPELL_AURA_DER_HINGABE);
        DoCastSpellIfCan(m_creature,SPELL_SIEGEL_DER_ABRECHNUNG);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (aura_der_hingabe_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_AURA_DER_HINGABE);
            aura_der_hingabe_timer = random(240000,250000);
        }
        else
        {
            aura_der_hingabe_timer -= diff;
        }

        if (heiliger_stoss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_HEILIGER_STOSS);
            heiliger_stoss_timer = random(5000,8000);
        }
        else
        {
            heiliger_stoss_timer -=diff;
        }

        if (heiliges_licht_timer < diff)
        {
            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40)
            {
                DoCastSpellIfCan(m_creature,SPELL_HEILIGES_LICHT);
            }
            heiliges_licht_timer = random(8000,10000);
        }
        else
        {
            heiliges_licht_timer -=diff;
        }


        if (siegel_der_abrechnung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SIEGEL_DER_ABRECHNUNG);
            siegel_der_abrechnung_timer = random(30000,35000);
        }
        else
        {
            siegel_der_abrechnung_timer -=diff;
        }
        if (tritt_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_TRITT);
            tritt_timer = random(8000,10000);
        }
        else
        {
            tritt_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_fineous_dunkelader(Creature *_Creature)
{
    return new fineous_dunkeladerAI (_Creature);
}

void AddSC_boss_fineous_dunkelader()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_fineous_dunkelader";
    newscript->GetAI = &GetAI_fineous_dunkelader;
    newscript->RegisterSelf();
}
