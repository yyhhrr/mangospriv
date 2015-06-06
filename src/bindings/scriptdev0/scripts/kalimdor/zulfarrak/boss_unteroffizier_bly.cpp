/* boss_unteroffizier_bly */

#include "precompiled.h"

#define rache	12170
#define schildhieb	11972


struct MANGOS_DLL_DECL boss_unteroffizier_blyAI : public ScriptedAI
{
    boss_unteroffizier_blyAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 rache_timer;
    uint32 schildhieb_timer;

    void Reset()
    {
        rache_timer = 3000;
        schildhieb_timer = 4000;
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
        {
            Reset();
            return;
        }

        if (rache_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),rache);
            rache_timer = random(3000,9000);
        } else rache_timer -= diff;

        if (schildhieb_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schildhieb);
            schildhieb_timer = random(5000,11000);
        } else schildhieb_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_unteroffizier_bly(Creature *_Creature)
{
    return new boss_unteroffizier_blyAI (_Creature);
}

void AddSC_boss_unteroffizier_bly()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_unteroffizier_bly";
    newscript->GetAI = &GetAI_boss_unteroffizier_bly;
    newscript->RegisterSelf();
}

