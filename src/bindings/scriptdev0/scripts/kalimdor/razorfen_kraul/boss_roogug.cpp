/* boss_roogug */

#include "precompiled.h"

#define blitzschlag	9532
#define erdrumpler_beschwoeren	8270

struct MANGOS_DLL_DECL boss_roogugAI : public ScriptedAI
{
    boss_roogugAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blitzschlag_timer;
    uint32 erdrumpler_beschwoeren_timer;

    bool spawned;

    void Reset()
    {
        blitzschlag_timer = 2000;
        erdrumpler_beschwoeren_timer = 8000;
        spawned = false;
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

        if (blitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),blitzschlag);
            blitzschlag_timer = random(4500,6000);
        } else blitzschlag_timer -= diff;

        if (erdrumpler_beschwoeren_timer < diff && spawned == false)
        {
            DoCastSpellIfCan(m_creature,erdrumpler_beschwoeren);
            spawned = true;
        } else erdrumpler_beschwoeren_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_roogug(Creature *_Creature)
{
    return new boss_roogugAI (_Creature);
}

void AddSC_boss_roogug()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_roogug";
    newscript->GetAI = &GetAI_boss_roogug;
    newscript->RegisterSelf();
}

