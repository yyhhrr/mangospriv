/* boss_henker_der_sandwueter */

#include "precompiled.h"
#include "zulfarrak.h"

#define berserkerhaltung	7366
#define hinrichten	7160
#define spalten	15496


struct MANGOS_DLL_DECL boss_henker_der_sandwueterAI : public ScriptedAI
{
    boss_henker_der_sandwueterAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 berserkerhaltung_timer;
    uint32 hinrichten_timer;
    uint32 spalten_timer;

    void Reset()
    {
        berserkerhaltung_timer = 500;
        hinrichten_timer = 6000;
        spalten_timer = 2000;
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

        if (berserkerhaltung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,berserkerhaltung);
            berserkerhaltung_timer = random(900000,9000000);
        } else berserkerhaltung_timer -= diff;

        if (hinrichten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hinrichten);
            hinrichten_timer = random(6000,10000);
        } else hinrichten_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(5000,10000);
        } else spalten_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_henker_der_sandwueter(Creature *_Creature)
{
    return new boss_henker_der_sandwueterAI (_Creature);
}

void AddSC_boss_henker_der_sandwueter()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_henker_der_sandwueter";
    newscript->GetAI = &GetAI_boss_henker_der_sandwueter;
    newscript->RegisterSelf();
}

