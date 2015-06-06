/* mutanus_der_verschlinger */

#include "precompiled.h"
#include "def_wailing_caverns.h"

#define donnerkracher	8150
#define erschrecken	7399
#define naralex_alptraum	7967

struct MANGOS_DLL_DECL boss_mutanus_der_verschlingerAI : public ScriptedAI
{
    boss_mutanus_der_verschlingerAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 donnerkracher_timer;
    uint32 erschrecken_timer;
    uint32 naralex_alptraum_timer;

    void Reset()
    {
        donnerkracher_timer = 10000;
        erschrecken_timer = 13000;
        naralex_alptraum_timer = 15000;
    }

    void JustDied(Unit*)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (!pInstance)
            return ;

        pInstance->SetData(TYPE_MUTANOUS, DONE);
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

        if (donnerkracher_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),donnerkracher);
            donnerkracher_timer = random(9000,14000);
        } else donnerkracher_timer -= diff;

        if (erschrecken_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),erschrecken);
            erschrecken_timer = random(8000,16000);
        } else erschrecken_timer -= diff;

        if (naralex_alptraum_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),naralex_alptraum);
            naralex_alptraum_timer = random(15000,20000);
        } else naralex_alptraum_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_mutanus_der_verschlinger(Creature *_Creature)
{
    return new boss_mutanus_der_verschlingerAI (_Creature);
}

void AddSC_boss_mutanus_der_verschlinger()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_mutanus_der_verschlinger";
    newscript->GetAI = &GetAI_boss_mutanus_der_verschlinger;
    newscript->RegisterSelf();
}

