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

#include "precompiled.h"

#define SPELL_BRENNENDER_GEIST		14744
// add unten
#define add1x 978.385071f
#define add1y -240.758484f
#define add1z -61.399963f
#define add1o 2.303830f
// add oben
#define add2x 1046.264648f
#define add2y -240.773376f
#define add2z -61.969902f
#define add2o 3.092264f
//add links
#define add3x 1012.375854f
#define add3y -207.150696f
#define add3z -61.984966f
#define add3o 4.749455f
//add rechts
#define add4x 1012.144043f
#define add4y -275.383148f
#define add4z -61.973907f
#define add4o 1.617287f

double bossx;
double bossy;
double bossz;
uint32 bosshealth;
uint32 bosshealplus=0;

struct MANGOS_DLL_DECL botschafter_flammenschlagAI : public ScriptedAI
{
    botschafter_flammenschlagAI(Creature *c) : ScriptedAI(c) {}

    uint32 brennender_geist_Timer;
    uint32 summon_Timer;
    uint32 bosspos_Timer;
    Unit* target;

    void Reset()
    {
        brennender_geist_Timer = 2000;
        summon_Timer = 5000;
        bosspos_Timer = 500;
        bosshealplus=0;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (bosspos_Timer < diff)
        {
            bossx=m_creature->GetPositionX();
            bossy=m_creature->GetPositionY();
            bossz=m_creature->GetPositionZ();
            bosspos_Timer = 500;
        } else bosspos_Timer -= diff;

        if (brennender_geist_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BRENNENDER_GEIST);
            brennender_geist_Timer = 150000;
        } else brennender_geist_Timer -= diff;


        if (summon_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            m_creature->SummonCreature(9178, add1x, add1y , add1z, add1o, TEMPSUMMON_TIMED_DESPAWN, 60000);
            m_creature->SummonCreature(9178, add2x, add2y , add2z, add2o, TEMPSUMMON_TIMED_DESPAWN, 60000);
            m_creature->SummonCreature(9178, add3x, add3y , add3z, add3o, TEMPSUMMON_TIMED_DESPAWN, 60000);
            m_creature->SummonCreature(9178, add4x, add4y , add4z, add4o, TEMPSUMMON_TIMED_DESPAWN, 60000);

            summon_Timer = 35000;
        } else summon_Timer -= diff;

        if (bosshealplus==4000)
        {
            bosshealth=m_creature->GetHealth();
            bosshealth=bosshealth+bosshealplus;
            m_creature->SetHealth(bosshealth);
            bosshealplus=0;
        } else

            DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_botschafter_flammenschlag(Creature *_Creature)
{
    return new botschafter_flammenschlagAI (_Creature);
}

struct MANGOS_DLL_DECL brennender_geistAI : public ScriptedAI
{
    brennender_geistAI(Creature *c) : ScriptedAI(c) {}

    uint32 pos_Timer;
    uint32 heal_Timer;
    Unit* target;

    void Reset()
    {
        pos_Timer = 500;
        heal_Timer = 10000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {

        if ( pos_Timer < diff)
        {
            m_creature->MonsterMoveWithSpeed(bossx, bossy, bossz, 6.0f, false, false, 0);
            pos_Timer = 500;

        } else pos_Timer -= diff;


        if (heal_Timer < diff )
        {
            bosshealplus=4000;
            m_creature->ForcedDespawn();
            heal_Timer = random(10000,15000);
        } else heal_Timer -= diff;
    }
};

CreatureAI* GetAI_brennender_geist(Creature *_Creature)
{
    return new brennender_geistAI (_Creature);
}

void AddSC_botschafter_flammenschlag()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="botschafter_flammenschlag";
    newscript->GetAI = &GetAI_botschafter_flammenschlag;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="brennender_geist";
    newscript->GetAI = &GetAI_brennender_geist;
    newscript->RegisterSelf();
}

