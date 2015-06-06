/* skum */

#include "precompiled.h"

#define kettenblitzschlag	6254


struct MANGOS_DLL_DECL boss_skumAI : public ScriptedAI
{
    boss_skumAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 kettenblitzschlag_timer;

    void Reset()
    {
        kettenblitzschlag_timer = 1000;
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

        if (kettenblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),kettenblitzschlag);
            kettenblitzschlag_timer = random(5000,6000);
        } else kettenblitzschlag_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_skum(Creature *_Creature)
{
    return new boss_skumAI (_Creature);
}

void AddSC_boss_skum()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_skum";
    newscript->GetAI = &GetAI_boss_skum;
    newscript->RegisterSelf();
}

