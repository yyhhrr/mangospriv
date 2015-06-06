/* boss_morphaz */

#include "precompiled.h"

#define fluegelschlag	12882
#define sauureatem	12884


struct MANGOS_DLL_DECL boss_morphazAI : public ScriptedAI
{
    boss_morphazAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 fluegelschlag_timer;
    uint32 sauureatem_timer;

    void Reset()
    {
        fluegelschlag_timer = 2000;
        sauureatem_timer = 4000;
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
            fluegelschlag_timer = random(4000,9000);
        } else fluegelschlag_timer -= diff;

        if (sauureatem_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),sauureatem);
            sauureatem_timer = random(4000,9000);
        } else sauureatem_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_morphaz(Creature *_Creature)
{
    return new boss_morphazAI (_Creature);
}

void AddSC_boss_morphaz()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_morphaz";
    newscript->GetAI = &GetAI_boss_morphaz;
    newscript->RegisterSelf();
}

