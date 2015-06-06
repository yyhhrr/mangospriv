/* boss_rotkralle */

#include "precompiled.h"
#include "shadowfang_keep.h"

#define seelensauger	7295


struct MANGOS_DLL_DECL boss_rotkralleAI : public ScriptedAI
{
    boss_rotkralleAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 seelensauger_timer;

    void Reset()
    {
        seelensauger_timer = 3000;
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (pInstance)
            pInstance->SetData(TYPE_RETHILGORE, DONE);
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
            Reset();
            return;
        }

        if (seelensauger_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),seelensauger);
            seelensauger_timer = random(15000,25000);
        } else seelensauger_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_rotkralle(Creature *_Creature)
{
    return new boss_rotkralleAI (_Creature);
}

void AddSC_boss_rotkralle()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_rotkralle";
    newscript->GetAI = &GetAI_boss_rotkralle;
    newscript->RegisterSelf();
}

