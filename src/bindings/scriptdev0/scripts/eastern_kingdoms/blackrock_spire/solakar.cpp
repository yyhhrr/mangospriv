/* solakar */

#include "precompiled.h"

#define kriegsdonner	16727

struct MANGOS_DLL_DECL solakarAI : public ScriptedAI
{
    solakarAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 kriegsdonner_timer;

    void Reset()
    {
        kriegsdonner_timer = 4000;
    }


    void Aggro(Unit *who)
    {

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

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            return;
        }


        if (kriegsdonner_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kriegsdonner);
            kriegsdonner_timer = random(10000,12000);
        } else kriegsdonner_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_solakar(Creature *_Creature)
{
    return new solakarAI (_Creature);
}

void AddSC_solakar()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="solakar";
    newscript->GetAI = &GetAI_solakar;
    newscript->RegisterSelf();
}