/* sneeds_schredder */

#include "precompiled.h"

#define ablenkender_schmerz	3603
#define erschrecken	7399
#define sneeds_hinauswerfen 5141


struct MANGOS_DLL_DECL boss_sneeds_schredderAI : public ScriptedAI
{
    boss_sneeds_schredderAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 ablenkender_schmerz_timer;
    uint32 erschrecken_timer;

    void Reset()
    {
        ablenkender_schmerz_timer = 2000;
        erschrecken_timer = 5000;
    }

    void JustDied(Unit* Killer)
    {
        Killer->CastCustomSpell(Killer,sneeds_hinauswerfen,0,0,0,false,0,0,ObjectGuid());

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

        if (ablenkender_schmerz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),ablenkender_schmerz);
            ablenkender_schmerz_timer = random(4000,6000);
        } else ablenkender_schmerz_timer -= diff;

        if (erschrecken_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),erschrecken);
            erschrecken_timer = random(8000,10000);
        } else erschrecken_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_sneeds_schredder(Creature *_Creature)
{
    return new boss_sneeds_schredderAI (_Creature);
}

void AddSC_boss_sneeds_schredder()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_sneeds_schredder";
    newscript->GetAI = &GetAI_boss_sneeds_schredder;
    newscript->RegisterSelf();
}