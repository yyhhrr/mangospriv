/* boss_theka_der_maertyrer */

#include "precompiled.h"
#include "zulfarrak.h"

#define fieberseuche	8600


struct MANGOS_DLL_DECL boss_theka_der_maertyrerAI : public ScriptedAI
{
    boss_theka_der_maertyrerAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 fieberseuche_timer;

    void Reset()
    {
        fieberseuche_timer = 3000;
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

        if (fieberseuche_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),fieberseuche);
            fieberseuche_timer = random(7000,10000);
        } else fieberseuche_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_theka_der_maertyrer(Creature *_Creature)
{
    return new boss_theka_der_maertyrerAI (_Creature);
}

void AddSC_boss_theka_der_maertyrer()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_theka_der_maertyrer";
    newscript->GetAI = &GetAI_boss_theka_der_maertyrer;
    newscript->RegisterSelf();
}

