/* boss_kommandant_gruental */

#include "precompiled.h"

#define hammer_der_gerechtigkeit	5588
#define heiliges_licht	1026


struct MANGOS_DLL_DECL boss_kommandant_gruentalAI : public ScriptedAI
{
    boss_kommandant_gruentalAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 hammer_der_gerechtigkeit_timer;
    uint32 heiliges_licht_timer;

    void Reset()
    {
        hammer_der_gerechtigkeit_timer = 10000;
        heiliges_licht_timer = 15000;
    }

    void Aggro(Unit *who)
    {
        m_creature->CallForHelp(200.0f);
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

        if (hammer_der_gerechtigkeit_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hammer_der_gerechtigkeit);
            hammer_der_gerechtigkeit_timer = random(15000,25000);
        } else hammer_der_gerechtigkeit_timer -= diff;

        if (heiliges_licht_timer < diff)
        {
            DoCastSpellIfCan(m_creature,heiliges_licht);
            heiliges_licht_timer = random(25000,30000);
        } else heiliges_licht_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_kommandant_gruental(Creature *_Creature)
{
    return new boss_kommandant_gruentalAI (_Creature);
}

void AddSC_boss_kommandant_gruental()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kommandant_gruental";
    newscript->GetAI = &GetAI_boss_kommandant_gruental;
    newscript->RegisterSelf();
}

