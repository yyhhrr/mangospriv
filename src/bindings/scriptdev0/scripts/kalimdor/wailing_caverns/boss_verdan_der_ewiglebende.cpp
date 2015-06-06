/* verdan_der_ewiglebende */

#include "precompiled.h"

#define greifende_ranken	8142

struct MANGOS_DLL_DECL boss_verdan_der_ewiglebendeAI : public ScriptedAI
{
    boss_verdan_der_ewiglebendeAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 greifende_ranken_timer;

    void Reset()
    {
        greifende_ranken_timer = 4000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (greifende_ranken_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),greifende_ranken);
            greifende_ranken_timer = random(12000,14000);
        } else greifende_ranken_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_verdan_der_ewiglebende(Creature *_Creature)
{
    return new boss_verdan_der_ewiglebendeAI (_Creature);
}

void AddSC_boss_verdan_der_ewiglebende()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_verdan_der_ewiglebende";
    newscript->GetAI = &GetAI_boss_verdan_der_ewiglebende;
    newscript->RegisterSelf();
}

