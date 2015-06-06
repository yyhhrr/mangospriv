/* boss_lord_pythas */

#include "precompiled.h"
#include "def_wailing_caverns.h"

#define blizschlag	9532
#define donnerknall	8147
#define heilende_beruehrung	5187
#define schlaf	700

struct MANGOS_DLL_DECL boss_lord_pythasAI : public ScriptedAI
{
    boss_lord_pythasAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blizschlag_timer;
    uint32 donnerknall_timer;
    uint32 heilende_beruehrung_timer;
    uint32 schlaf_timer;

    void Reset()
    {
        blizschlag_timer = 1000;
        donnerknall_timer = 7000;
        heilende_beruehrung_timer = 20000;
        schlaf_timer = 8000;
    }

    void JustDied(Unit*)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (!pInstance)
            return ;

        pInstance->SetData(TYPE_PYTHAS, DONE);
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

        if (blizschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),blizschlag);
            blizschlag_timer = random(3000,4000);
        } else blizschlag_timer -= diff;

        if (donnerknall_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),donnerknall);
            donnerknall_timer = random(15000,20000);
        } else donnerknall_timer -= diff;

        if ((m_creature->GetHealthPercent() < 60.0f) && (heilende_beruehrung_timer < diff))
        {
            DoCastSpellIfCan(m_creature,heilende_beruehrung);
            heilende_beruehrung_timer = random(6000,7000);
        } else heilende_beruehrung_timer -= diff;

        if (schlaf_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schlaf);
            schlaf_timer = random(17000,25000);
        } else schlaf_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lord_pythas(Creature *_Creature)
{
    return new boss_lord_pythasAI (_Creature);
}

void AddSC_boss_lord_pythas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_pythas";
    newscript->GetAI = &GetAI_boss_lord_pythas;
    newscript->RegisterSelf();
}

