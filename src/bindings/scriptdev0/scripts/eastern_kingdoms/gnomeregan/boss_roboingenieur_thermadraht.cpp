/* boss_robogenieur_thermadraht */

#include "precompiled.h"

#define wegschlagen_tank	10101
#define wegschlagen	11130


struct MANGOS_DLL_DECL boss_roboingenieur_thermadrahtAI : public ScriptedAI
{
    boss_roboingenieur_thermadrahtAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 wegschlagen_tank_timer;
    uint32 wegschlagen_timer;

    void Reset()
    {
        wegschlagen_tank_timer = 4000;
        wegschlagen_timer = 8000;
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

        if (wegschlagen_tank_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),wegschlagen_tank);
            wegschlagen_tank_timer = random(5000,7000);
        } else wegschlagen_tank_timer -= diff;

        if (wegschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),wegschlagen);
            wegschlagen_timer = random(7000,12000);
        } else wegschlagen_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_roboingenieur_thermadraht(Creature *_Creature)
{
    return new boss_roboingenieur_thermadrahtAI (_Creature);
}

void AddSC_boss_roboingenieur_thermadraht()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_roboingenieur_thermadraht";
    newscript->GetAI = &GetAI_boss_roboingenieur_thermadraht;
    newscript->RegisterSelf();
}

