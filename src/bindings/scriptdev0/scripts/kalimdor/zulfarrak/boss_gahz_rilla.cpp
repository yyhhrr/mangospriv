/* boss_gahz_rilla */

#include "precompiled.h"
#include "zulfarrak.h"

#define eiszapfen	11131
#define gahz_rillas_schmetterschlag	11902
#define steif_frieren	11836

struct MANGOS_DLL_DECL boss_gahz_rillaAI : public ScriptedAI
{
    boss_gahz_rillaAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 eiszapfen_timer;
    uint32 gahz_rillas_schmetterschlag_timer;
    uint32 steif_frieren_timer;

    void Reset()
    {
        eiszapfen_timer = 12000;
        gahz_rillas_schmetterschlag_timer = 8000;
        steif_frieren_timer = 10000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (eiszapfen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),eiszapfen);
            eiszapfen_timer = random(4000,10000);
        } else eiszapfen_timer -= diff;

        if (gahz_rillas_schmetterschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),gahz_rillas_schmetterschlag);
            gahz_rillas_schmetterschlag_timer = random(5000,10000);
        } else gahz_rillas_schmetterschlag_timer -= diff;

        if (steif_frieren_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),steif_frieren);
            steif_frieren_timer = random(8000,15000);
        } else steif_frieren_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gahz_rilla(Creature *_Creature)
{
    return new boss_gahz_rillaAI (_Creature);
}

void AddSC_boss_gahz_rilla()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gahz_rilla";
    newscript->GetAI = &GetAI_boss_gahz_rilla;
    newscript->RegisterSelf();
}

