/* boss_baron_silberlein */

#include "precompiled.h"

#define schleier_des_schattens	7068


struct MANGOS_DLL_DECL boss_baron_silberleinAI : public ScriptedAI
{
    boss_baron_silberleinAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 schleier_des_schattens_timer;

    void Reset()
    {
        schleier_des_schattens_timer = 8000;
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

        if (schleier_des_schattens_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schleier_des_schattens);
            schleier_des_schattens_timer = random(10000,12000);
        } else schleier_des_schattens_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_baron_silberlein(Creature *_Creature)
{
    return new boss_baron_silberleinAI (_Creature);
}

void AddSC_boss_baron_silberlein()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_baron_silberlein";
    newscript->GetAI = &GetAI_boss_baron_silberlein;
    newscript->RegisterSelf();
}

