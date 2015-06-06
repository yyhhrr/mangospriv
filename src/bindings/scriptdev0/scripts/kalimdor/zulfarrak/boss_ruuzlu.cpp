/* boss_ruuzlu */

#include "precompiled.h"
#include "zulfarrak.h"

#define berserkerhaltung	7366
#define hinrichten	7160
#define spalten	15496


struct MANGOS_DLL_DECL boss_ruuzluAI : public ScriptedAI
{
    boss_ruuzluAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 berserkerhaltung_timer;
    uint32 hinrichten_timer;
    uint32 spalten_timer;

    void Reset()
    {
        berserkerhaltung_timer = 1000;
        hinrichten_timer = 10000;
        spalten_timer = 4000;
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
            berserkerhaltung_timer = random(9999999,99999999);
        } else berserkerhaltung_timer -= diff;

        if (hinrichten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hinrichten);
            hinrichten_timer = random(12000,12500);
        } else hinrichten_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(7000,12000);
        } else spalten_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_ruuzlu(Creature *_Creature)
{
    return new boss_ruuzluAI (_Creature);
}

void AddSC_boss_ruuzlu()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ruuzlu";
    newscript->GetAI = &GetAI_boss_ruuzlu;
    newscript->RegisterSelf();
}

