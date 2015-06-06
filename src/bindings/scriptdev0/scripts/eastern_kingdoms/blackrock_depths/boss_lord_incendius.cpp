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
/* Lord Incendius */

#include "precompiled.h"



#define SPELL_FEUEREXPLOSION			13900
#define SPELL_FEUERSTURM				13899
#define SPELL_FLUCH_DES_ELEMENTARLORDS	26977
#define SPELL_MAECHTIGER_DRAUFSCHLAG	14099

struct MANGOS_DLL_DECL lord_incendiusAI : public ScriptedAI
{
    lord_incendiusAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 feuerexplosion_timer;
    uint32 feuersturm_timer;
    uint32 fluch_des_elementarlords_timer;
    uint32 maechtiger_draufschlag_timer;

    void Reset()
    {
        feuerexplosion_timer = 5000;
        feuersturm_timer = 10000;
        fluch_des_elementarlords_timer = 40000;
        maechtiger_draufschlag_timer = 3000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Nun lass ich euch brennen!",LANG_UNIVERSAL,NULL);

    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (feuerexplosion_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FEUEREXPLOSION);
            feuerexplosion_timer = random(4000,6000);
        }
        else
        {
            feuerexplosion_timer -= diff;
        }

        if (feuersturm_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FEUERSTURM);
            feuersturm_timer = random(9000,11000);
        }
        else
        {
            feuersturm_timer -=diff;
        }

        if (fluch_des_elementarlords_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FLUCH_DES_ELEMENTARLORDS);
            fluch_des_elementarlords_timer = 40000;
        }
        else
        {
            fluch_des_elementarlords_timer -=diff;
        }

        if (maechtiger_draufschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MAECHTIGER_DRAUFSCHLAG);
            maechtiger_draufschlag_timer = random(2500,3500);
        }
        else
        {
            maechtiger_draufschlag_timer -=diff;
        }


        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_lord_incendius(Creature *_Creature)
{
    return new lord_incendiusAI (_Creature);
}


void AddSC_boss_lord_incendius()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_incendius";
    newscript->GetAI = &GetAI_lord_incendius;
    newscript->RegisterSelf();
}
