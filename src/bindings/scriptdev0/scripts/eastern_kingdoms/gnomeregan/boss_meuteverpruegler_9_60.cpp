/* boss_meuteverpruegler_9_60 */

#include "precompiled.h"

#define bogenzerkracher	8374
#define meute_verpruegeln	10887
#define trampeln	5568


struct MANGOS_DLL_DECL boss_meuteverpruegler_9_60AI : public ScriptedAI
{
    boss_meuteverpruegler_9_60AI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 bogenzerkracher_timer;
    uint32 meute_verpruegeln_timer;
    uint32 trampeln_timer;

    void Reset()
    {
        bogenzerkracher_timer = 7000;
        meute_verpruegeln_timer = 5000;
        trampeln_timer = 3000;
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

        if (bogenzerkracher_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),bogenzerkracher);
            bogenzerkracher_timer = random(8000,11000);
        } else bogenzerkracher_timer -= diff;

        if (meute_verpruegeln_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),meute_verpruegeln);
            meute_verpruegeln_timer = random(7000,10000);
        } else meute_verpruegeln_timer -= diff;

        if (trampeln_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),trampeln);
            trampeln_timer = random(4000,5000);
        } else trampeln_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_meuteverpruegler_9_60(Creature *_Creature)
{
    return new boss_meuteverpruegler_9_60AI (_Creature);
}

void AddSC_boss_meuteverpruegler_9_60()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_meuteverpruegler_9_60";
    newscript->GetAI = &GetAI_boss_meuteverpruegler_9_60;
    newscript->RegisterSelf();
}

