/* gizrul_der_geifernde */

#include "precompiled.h"

#define infizierter_biss	16128
#define raserei	8269
#define toedlicher_biss	16495


struct MANGOS_DLL_DECL gizrul_der_geiferndeAI : public ScriptedAI
{
    gizrul_der_geiferndeAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 infizierter_biss_timer;
    uint32 raserei_timer;
    uint32 toedlicher_biss_timer;

    void Reset()
    {
        infizierter_biss_timer = 2000;
        raserei_timer = 8000;
        toedlicher_biss_timer = 5000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Wuff! Wuff wuff Wuff!",LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit* Killer)
    {
    }

    void KilledUnit(Unit *victim)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (infizierter_biss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),infizierter_biss);
            infizierter_biss_timer = random(30000,60000);
        } else infizierter_biss_timer -= diff;

        if (raserei_timer < diff)
        {
            DoCastSpellIfCan(m_creature,raserei);
            raserei_timer = random(100000,120000);
        } else raserei_timer -= diff;

        if (toedlicher_biss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),toedlicher_biss);
            toedlicher_biss_timer = random(10000,15000);
        } else toedlicher_biss_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_gizrul_der_geifernde(Creature *_Creature)
{
    return new gizrul_der_geiferndeAI (_Creature);
}

void AddSC_gizrul_der_geifernde()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="gizrul_der_geifernde";
    newscript->GetAI = &GetAI_gizrul_der_geifernde;
    newscript->RegisterSelf();
}