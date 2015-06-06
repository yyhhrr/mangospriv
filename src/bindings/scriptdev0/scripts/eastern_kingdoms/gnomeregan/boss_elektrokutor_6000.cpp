/* boss_elektrokutor_6000 */

#include "precompiled.h"

#define kettenblitzschlag	11085
#define megavolt	11082
#define schock	11084


struct MANGOS_DLL_DECL boss_elektrokutor_6000AI : public ScriptedAI
{
    boss_elektrokutor_6000AI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 kettenblitzschlag_timer;
    uint32 megavolt_timer;
    uint32 schock_timer;

    void Reset()
    {
        kettenblitzschlag_timer = 3000;
        megavolt_timer = 5000;
        schock_timer = 7000;
    }

    void JustDied(Unit* Killer)
    {
    }

    void KilledUnit(Unit *victim)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (kettenblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),kettenblitzschlag);
            kettenblitzschlag_timer = random(6000,8000);
        } else kettenblitzschlag_timer -= diff;

        if (megavolt_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),megavolt);
            megavolt_timer = random(5000,8000);
        } else megavolt_timer -= diff;

        if (schock_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schock);
            schock_timer = random(6000,6500);
        } else schock_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_elektrokutor_6000(Creature *_Creature)
{
    return new boss_elektrokutor_6000AI (_Creature);
}

void AddSC_boss_elektrokutor_6000()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_elektrokutor_6000";
    newscript->GetAI = &GetAI_boss_elektrokutor_6000;
    newscript->RegisterSelf();
}

