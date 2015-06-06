/* boss_atal_alarion */

#include "precompiled.h"

#define bebende_erde	6524
#define weitreichendes_zerschmettern	12887


struct MANGOS_DLL_DECL boss_atal_alarionAI : public ScriptedAI
{
    boss_atal_alarionAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 bebende_erde_timer;
    uint32 weitreichendes_zerschmettern_timer;

    void Reset()
    {
        bebende_erde_timer = 2000;
        weitreichendes_zerschmettern_timer = 3000;
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

        if (bebende_erde_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),bebende_erde);
            bebende_erde_timer = random(5000,8000);
        } else bebende_erde_timer -= diff;

        if (weitreichendes_zerschmettern_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),weitreichendes_zerschmettern);
            weitreichendes_zerschmettern_timer = random(4000,9000);
        } else weitreichendes_zerschmettern_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_atal_alarion(Creature *_Creature)
{
    return new boss_atal_alarionAI (_Creature);
}

void AddSC_boss_atal_alarion()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_atal_alarion";
    newscript->GetAI = &GetAI_boss_atal_alarion;
    newscript->RegisterSelf();
}

