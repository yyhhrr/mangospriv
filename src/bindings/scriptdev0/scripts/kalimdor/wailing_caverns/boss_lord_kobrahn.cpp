/* boss_lord_kobrahn */

#include "precompiled.h"
#include "def_wailing_caverns.h"

#define blizschlag	9532
#define druidenschlummer	8040
#define gift	744
#define heilende_beruehrung	5188
#define schlangengestalt_von_kobrahn	7965

struct MANGOS_DLL_DECL boss_lord_kobrahnAI : public ScriptedAI
{
    boss_lord_kobrahnAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blizschlag_timer;
    uint32 druidenschlummer_timer;
    uint32 gift_timer;
    uint32 heilende_beruehrung_timer;
    uint32 schlangengestalt_von_kobrahn_timer;

    void Reset()
    {
        blizschlag_timer = 1000;
        druidenschlummer_timer = 7500;
        gift_timer = 2000;
        heilende_beruehrung_timer = 1000;
        schlangengestalt_von_kobrahn_timer = 15000;
    }

    void JustDied(Unit*)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (!pInstance)
            return ;

        pInstance->SetData(TYPE_COBRAHN, DONE);
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

        if (druidenschlummer_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),druidenschlummer);
            druidenschlummer_timer = random(15000,20000);
        } else druidenschlummer_timer -= diff;

        if (gift_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),gift);
            gift_timer = random(10000,15000);
        } else gift_timer -= diff;

        if ((m_creature->GetHealthPercent() < 60.0f) && (heilende_beruehrung_timer < diff))
        {
            DoCastSpellIfCan(m_creature,heilende_beruehrung);
            heilende_beruehrung_timer = random(6000,7000);
        } else heilende_beruehrung_timer -= diff;

        if (schlangengestalt_von_kobrahn_timer < diff)
        {
            DoCastSpellIfCan(m_creature,schlangengestalt_von_kobrahn);
            schlangengestalt_von_kobrahn_timer = random(120000,120000);
        } else schlangengestalt_von_kobrahn_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lord_kobrahn(Creature *_Creature)
{
    return new boss_lord_kobrahnAI (_Creature);
}

void AddSC_boss_lord_kobrahn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_kobrahn";
    newscript->GetAI = &GetAI_boss_lord_kobrahn;
    newscript->RegisterSelf();
}

