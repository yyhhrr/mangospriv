/* boss_todessprecher_jargba */

#include "precompiled.h"

#define gedanken_beherrschen	14515
#define schattenblitz	9613

struct MANGOS_DLL_DECL boss_todessprecher_jargbaAI : public ScriptedAI
{
    boss_todessprecher_jargbaAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 gedanken_beherrschen_timer;
    uint32 schattenblitz_timer;

    void Reset()
    {
        gedanken_beherrschen_timer = 6000;
        schattenblitz_timer = 2000;
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

        if (gedanken_beherrschen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),gedanken_beherrschen);
            gedanken_beherrschen_timer = random(20000,25000);
        } else gedanken_beherrschen_timer -= diff;

        if (schattenblitz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitz);
            schattenblitz_timer = random(4000,5000);
        } else schattenblitz_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_todessprecher_jargba(Creature *_Creature)
{
    return new boss_todessprecher_jargbaAI (_Creature);
}

void AddSC_boss_todessprecher_jargba()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_todessprecher_jargba";
    newscript->GetAI = &GetAI_boss_todessprecher_jargba;
    newscript->RegisterSelf();
}

