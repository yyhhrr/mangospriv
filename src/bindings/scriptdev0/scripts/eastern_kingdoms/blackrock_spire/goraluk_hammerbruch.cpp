/* goraluk_hammerbruch */

#include "precompiled.h"

#define kopfkracher	16172
#define rueckhand	6253
#define schlag	15580


struct MANGOS_DLL_DECL goraluk_hammerbruchAI : public ScriptedAI
{
    goraluk_hammerbruchAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 kopfkracher_timer;
    uint32 rueckhand_timer;
    uint32 schlag_timer;

    void Reset()
    {
        kopfkracher_timer = 2000;
        rueckhand_timer = 6000;
        schlag_timer = 10000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("So etwas lasse ich mir nicht entgehen!",LANG_UNIVERSAL,NULL);
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

        if (kopfkracher_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kopfkracher);
            kopfkracher_timer = random(15000,25000);
        } else kopfkracher_timer -= diff;

        if (rueckhand_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),rueckhand);
            rueckhand_timer = random(8000,13000);
        } else rueckhand_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(5000,10000);
        } else schlag_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_goraluk_hammerbruch(Creature *_Creature)
{
    return new goraluk_hammerbruchAI (_Creature);
}

void AddSC_goraluk_hammerbruch()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="goraluk_hammerbruch";
    newscript->GetAI = &GetAI_goraluk_hammerbruch;
    newscript->RegisterSelf();
}