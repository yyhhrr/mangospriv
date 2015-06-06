/* rar_karaburan */

#include "precompiled.h"
#include "zulfarrak.h"

#define finsterer_stoss	15667


struct MANGOS_DLL_DECL rar_karaburanAI : public ScriptedAI
{
    rar_karaburanAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 finsterer_stoss_timer;

    void Reset()
    {
        finsterer_stoss_timer = 3000;
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

        if (finsterer_stoss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),finsterer_stoss);
            finsterer_stoss_timer = random(3000,7000);
        } else finsterer_stoss_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_rar_karaburan(Creature *_Creature)
{
    return new rar_karaburanAI (_Creature);
}

void AddSC_rar_karaburan()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="rar_karaburan";
    newscript->GetAI = &GetAI_rar_karaburan;
    newscript->RegisterSelf();
}

