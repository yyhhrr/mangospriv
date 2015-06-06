/* boss_lady_anacondra */

#include "precompiled.h"
#include "def_wailing_caverns.h"

#define blizschlag	9532
#define dornen_aura	8148
#define heilende_beruehrung	5187
#define schlaf	700

struct MANGOS_DLL_DECL boss_lady_anacondraAI : public ScriptedAI
{
    boss_lady_anacondraAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blizschlag_timer;
    uint32 dornen_aura_timer;
    uint32 heilende_beruehrung_timer;
    uint32 schlaf_timer;

    void Reset()
    {
        blizschlag_timer = 1000;
        dornen_aura_timer = 1000;
        heilende_beruehrung_timer = 1000;
        schlaf_timer = 7000;
    }

    void JustDied(Unit*)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (!pInstance)
            return ;

        pInstance->SetData(TYPE_ANACONDRA, DONE);
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

        if (dornen_aura_timer < diff)
        {
            DoCastSpellIfCan(m_creature,dornen_aura);
            dornen_aura_timer = random(50000,60000);
        } else dornen_aura_timer -= diff;

        if ((m_creature->GetHealthPercent() < 35.0f) && (heilende_beruehrung_timer < diff))
        {
            DoCastSpellIfCan(m_creature,heilende_beruehrung);
            heilende_beruehrung_timer = random(6000,7000);
        } else heilende_beruehrung_timer -= diff;

        if (schlaf_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schlaf);
            schlaf_timer = random(17000,20000);
        } else schlaf_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lady_anacondra(Creature *_Creature)
{
    return new boss_lady_anacondraAI (_Creature);
}

void AddSC_boss_lady_anacondra()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lady_anacondra";
    newscript->GetAI = &GetAI_boss_lady_anacondra;
    newscript->RegisterSelf();
}

