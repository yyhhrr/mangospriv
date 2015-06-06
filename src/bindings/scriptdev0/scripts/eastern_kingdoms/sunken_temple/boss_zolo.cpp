/* boss_zolo */

#include "precompiled.h"

#define kettenblitzschlag	12058
#define skeletttotem_der_atal_ai	12506


struct MANGOS_DLL_DECL boss_zoloAI : public ScriptedAI
{
    boss_zoloAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 kettenblitzschlag_timer;
    uint32 skeletttotem_der_atal_ai_timer;

    void Reset()
    {
        kettenblitzschlag_timer = 2000;
        skeletttotem_der_atal_ai_timer = 500;
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

        if (kettenblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kettenblitzschlag);
            kettenblitzschlag_timer = random(6000,7000);
        } else kettenblitzschlag_timer -= diff;

        if (skeletttotem_der_atal_ai_timer < diff)
        {
            DoCastSpellIfCan(m_creature,skeletttotem_der_atal_ai);
            skeletttotem_der_atal_ai_timer = random(8000,12000);
        } else skeletttotem_der_atal_ai_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_zolo(Creature *_Creature)
{
    return new boss_zoloAI (_Creature);
}

void AddSC_boss_zolo()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_zolo";
    newscript->GetAI = &GetAI_boss_zolo;
    newscript->RegisterSelf();
}

