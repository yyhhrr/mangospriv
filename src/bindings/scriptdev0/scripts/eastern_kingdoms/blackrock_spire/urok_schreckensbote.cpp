/* urok_schreckensbote */

#include "precompiled.h"

#define einschuechterndes_gebruell	16508
#define schlag	15580
#define verwunden	16509


struct MANGOS_DLL_DECL urok_schreckensboteAI : public ScriptedAI
{
    urok_schreckensboteAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 einschuechterndes_gebruell_timer;
    uint32 schlag_timer;
    uint32 verwunden_timer;

    void Reset()
    {
        einschuechterndes_gebruell_timer = 5000;
        schlag_timer = 3000;
        verwunden_timer = 1000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Wen haben wir denn hier ? Neulinge!",LANG_UNIVERSAL,NULL);
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
            return;
        }

        if (einschuechterndes_gebruell_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),einschuechterndes_gebruell);
            einschuechterndes_gebruell_timer = random(15000,25000);
        } else einschuechterndes_gebruell_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(6000,10000);
        } else schlag_timer -= diff;

        if (verwunden_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),verwunden);
            verwunden_timer = random(10000,20000);
        } else verwunden_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_urok_schreckensbote(Creature *_Creature)
{
    return new urok_schreckensboteAI (_Creature);
}

void AddSC_urok_schreckensbote()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="urok_schreckensbote";
    newscript->GetAI = &GetAI_urok_schreckensbote;
    newscript->RegisterSelf();
}

