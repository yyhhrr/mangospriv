/* boss_lord_serpentis */

#include "precompiled.h"
#include "def_wailing_caverns.h"

#define blizschlag	9532
#define heilende_beruehrung	6778
#define schlaf	700

struct MANGOS_DLL_DECL boss_lord_serpentisAI : public ScriptedAI
{
    boss_lord_serpentisAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blizschlag_timer;
    uint32 heilende_beruehrung_timer;
    uint32 schlaf_timer;

    void Reset()
    {
        blizschlag_timer = 1;
        heilende_beruehrung_timer = 20000;
        schlaf_timer = 7000;
    }

    void JustDied(Unit*)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (!pInstance)
            return ;

        pInstance->SetData(TYPE_SERPENTIS, DONE);
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

        if ((m_creature->GetHealthPercent() < 35.0f) && (heilende_beruehrung_timer < diff))
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

CreatureAI* GetAI_boss_lord_serpentis(Creature *_Creature)
{
    return new boss_lord_serpentisAI (_Creature);
}

void AddSC_boss_lord_serpentis()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_serpentis";
    newscript->GetAI = &GetAI_boss_lord_serpentis;
    newscript->RegisterSelf();
}

