/* boss_zul_lor */

#include "precompiled.h"

#define gebrechlichkeit	12530
#define spalten	20691


struct MANGOS_DLL_DECL boss_zul_lorAI : public ScriptedAI
{
    boss_zul_lorAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 gebrechlichkeit_timer;
    uint32 spalten_timer;

    void Reset()
    {
        gebrechlichkeit_timer = 3000;
        spalten_timer = 1500;
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
            Reset();
            return;
        }

        if (gebrechlichkeit_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),gebrechlichkeit);
            gebrechlichkeit_timer = random(500000,600000);
        } else gebrechlichkeit_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(3000,5000);
        } else spalten_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_zul_lor(Creature *_Creature)
{
    return new boss_zul_lorAI (_Creature);
}

void AddSC_boss_zul_lor()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_zul_lor";
    newscript->GetAI = &GetAI_boss_zul_lor;
    newscript->RegisterSelf();
}

