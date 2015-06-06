/* kristallfangzahn */

#include "precompiled.h"

#define spitzenspinnling_beschwoeren	16416


struct MANGOS_DLL_DECL kristallfangzahnAI : public ScriptedAI
{
    kristallfangzahnAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 spitzenspinnling_beschwoeren_timer;

    void Reset()
    {
        spitzenspinnling_beschwoeren_timer = 5000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Soo ist das also!",LANG_UNIVERSAL,NULL);
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

        if (spitzenspinnling_beschwoeren_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spitzenspinnling_beschwoeren);
            spitzenspinnling_beschwoeren_timer = random(10000,15000);
        } else spitzenspinnling_beschwoeren_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_kristallfangzahn(Creature *_Creature)
{
    return new kristallfangzahnAI (_Creature);
}

void AddSC_kristallfangzahn()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="kristallfangzahn";
    newscript->GetAI = &GetAI_kristallfangzahn;
    newscript->RegisterSelf();
}