/* boss_aggem_dornfluch */

#include "precompiled.h"

#define ebergeist_beschwoeren	8286
#define kettenheilung	14900
#define schlachtruf	6192

struct MANGOS_DLL_DECL boss_aggem_dornfluchAI : public ScriptedAI
{
    boss_aggem_dornfluchAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 ebergeist_beschwoeren_timer;
    uint32 kettenheilung_timer;
    uint32 schlachtruf_timer;

    void Reset()
    {
        ebergeist_beschwoeren_timer = 8000;
        kettenheilung_timer = 9000;
        schlachtruf_timer = 3000;
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

        if (ebergeist_beschwoeren_timer < diff)
        {
            DoCastSpellIfCan(m_creature,ebergeist_beschwoeren);
            ebergeist_beschwoeren_timer = random(25000,30000);
        } else ebergeist_beschwoeren_timer -= diff;

        if (kettenheilung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,kettenheilung);
            kettenheilung_timer = random(7000,10000);
        } else kettenheilung_timer -= diff;

        if (schlachtruf_timer < diff)
        {
            DoCastSpellIfCan(m_creature,schlachtruf);
            schlachtruf_timer = random(110000,115000);
        } else schlachtruf_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_aggem_dornfluch(Creature *_Creature)
{
    return new boss_aggem_dornfluchAI (_Creature);
}

void AddSC_boss_aggem_dornfluch()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_aggem_dornfluch";
    newscript->GetAI = &GetAI_boss_aggem_dornfluch;
    newscript->RegisterSelf();
}

