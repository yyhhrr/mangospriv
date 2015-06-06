/* boss_oberanfuehrer_rammhauer */

#include "precompiled.h"

#define donnerknall	15548
#define kampfhaltung	7165
#define schlachtruf	9128

struct MANGOS_DLL_DECL boss_oberanfuehrer_rammhauerAI : public ScriptedAI
{
    boss_oberanfuehrer_rammhauerAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 donnerknall_timer;
    uint32 kampfhaltung_timer;
    uint32 schlachtruf_timer;

    void Reset()
    {
        donnerknall_timer = 5000;
        kampfhaltung_timer = 1000;
        schlachtruf_timer = 2000;
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

        if (donnerknall_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),donnerknall);
            donnerknall_timer = random(15000,20000);
        } else donnerknall_timer -= diff;

        if (kampfhaltung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,kampfhaltung);
            kampfhaltung_timer = random(120000,160000);
        } else kampfhaltung_timer -= diff;

        if (schlachtruf_timer < diff)
        {
            DoCastSpellIfCan(m_creature,schlachtruf);
            schlachtruf_timer = random(110000,115000);
        } else schlachtruf_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_oberanfuehrer_rammhauer(Creature *_Creature)
{
    return new boss_oberanfuehrer_rammhauerAI (_Creature);
}

void AddSC_boss_oberanfuehrer_rammhauer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_oberanfuehrer_rammhauer";
    newscript->GetAI = &GetAI_boss_oberanfuehrer_rammhauer;
    newscript->RegisterSelf();
}

