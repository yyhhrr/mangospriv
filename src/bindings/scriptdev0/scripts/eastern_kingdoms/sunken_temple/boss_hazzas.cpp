/* boss_hazzas */

#include "precompiled.h"

#define fluegelschlag	12882
#define saeureatem	12884


struct MANGOS_DLL_DECL boss_hazzasAI : public ScriptedAI
{
    boss_hazzasAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 fluegelschlag_timer;
    uint32 saeureatem_timer;

    void Reset()
    {
        fluegelschlag_timer = 1000;
        saeureatem_timer = 5000;
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

        if (fluegelschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),fluegelschlag);
            fluegelschlag_timer = random(4000,8000);
        } else fluegelschlag_timer -= diff;

        if (saeureatem_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),saeureatem);
            saeureatem_timer = random(4000,8500);
        } else saeureatem_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_hazzas(Creature *_Creature)
{
    return new boss_hazzasAI (_Creature);
}

void AddSC_boss_hazzas()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hazzas";
    newscript->GetAI = &GetAI_boss_hazzas;
    newscript->RegisterSelf();
}

