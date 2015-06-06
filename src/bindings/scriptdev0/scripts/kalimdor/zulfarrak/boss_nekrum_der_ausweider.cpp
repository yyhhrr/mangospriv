/* boss_nekrum_der_ausweider */

#include "precompiled.h"
#include "zulfarrak.h"

#define fieberseuche	8600


struct MANGOS_DLL_DECL boss_nekrum_der_ausweiderAI : public ScriptedAI
{
    boss_nekrum_der_ausweiderAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 fieberseuche_timer;

    void Reset()
    {
        fieberseuche_timer = 2000;
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

        if (fieberseuche_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),fieberseuche);
            fieberseuche_timer = random(3000,7000);
        } else fieberseuche_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_nekrum_der_ausweider(Creature *_Creature)
{
    return new boss_nekrum_der_ausweiderAI (_Creature);
}

void AddSC_boss_nekrum_der_ausweider()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_nekrum_der_ausweider";
    newscript->GetAI = &GetAI_boss_nekrum_der_ausweider;
    newscript->RegisterSelf();
}

