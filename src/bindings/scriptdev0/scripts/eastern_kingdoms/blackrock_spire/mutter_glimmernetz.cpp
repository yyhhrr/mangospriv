/* mutter_glimmernetz */

#include "precompiled.h"

#define kristallieren	16104
#define muttermilch	16468
#define spitzenspinnling_beschwoeren	161103


struct MANGOS_DLL_DECL mutter_glimmernetzAI : public ScriptedAI
{
    mutter_glimmernetzAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 kristallieren_timer;
    uint32 muttermilch_timer;
    uint32 spitzenspinnling_beschwoeren_timer;

    void Reset()
    {
        kristallieren_timer = 8000;
        muttermilch_timer = 10000;
        spitzenspinnling_beschwoeren_timer = 12000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Kommt meine Kinder! Kommt und fresst!",LANG_UNIVERSAL,NULL);
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

        if (kristallieren_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kristallieren);
            kristallieren_timer = random(14000,18000);
        } else kristallieren_timer -= diff;

        if (muttermilch_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),muttermilch);
            muttermilch_timer = random(15000,25000);
        } else muttermilch_timer -= diff;

        if (spitzenspinnling_beschwoeren_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spitzenspinnling_beschwoeren);
            spitzenspinnling_beschwoeren_timer = random(20000,25000);
        } else spitzenspinnling_beschwoeren_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mutter_glimmernetz(Creature *_Creature)
{
    return new mutter_glimmernetzAI (_Creature);
}

void AddSC_mutter_glimmernetz()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="mutter_glimmernetz";
    newscript->GetAI = &GetAI_mutter_glimmernetz;
    newscript->RegisterSelf();
}