/* boss_loro */

#include "precompiled.h"

#define schildschlag	8242
#define verbessertes_blocken	3639


struct MANGOS_DLL_DECL boss_loroAI : public ScriptedAI
{
    boss_loroAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 schildschlag_timer;
    uint32 verbessertes_blocken_timer;

    void Reset()
    {
        schildschlag_timer = 4000;
        verbessertes_blocken_timer = 1000;
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

        if (schildschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schildschlag);
            schildschlag_timer = random(3000,10000);
        } else schildschlag_timer -= diff;

        if (verbessertes_blocken_timer < diff)
        {
            DoCastSpellIfCan(m_creature,verbessertes_blocken);
            verbessertes_blocken_timer = random(6000,10000);
        } else verbessertes_blocken_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_loro(Creature *_Creature)
{
    return new boss_loroAI (_Creature);
}

void AddSC_boss_loro()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_loro";
    newscript->GetAI = &GetAI_boss_loro;
    newscript->RegisterSelf();
}

