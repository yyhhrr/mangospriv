/* boss_agathelos_der_tobende */

#include "precompiled.h"

#define raserei	8269
#define sturmattacke	8260
#define toben	8285

struct MANGOS_DLL_DECL boss_agathelos_der_tobendeAI : public ScriptedAI
{
    boss_agathelos_der_tobendeAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 raserei_timer;
    uint32 sturmattacke_timer;
    uint32 toben_timer;

    void Reset()
    {
        raserei_timer = 10000;
        sturmattacke_timer = 100;
        toben_timer = 4000;
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

        if (raserei_timer < diff)
        {
            DoCastSpellIfCan(m_creature,raserei);
            raserei_timer = random(130000,135000);
        } else raserei_timer -= diff;

        if (sturmattacke_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),sturmattacke);
        } else sturmattacke_timer -= diff;

        if (toben_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),toben);
            toben_timer = random(25000,30000);
        } else toben_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_agathelos_der_tobende(Creature *_Creature)
{
    return new boss_agathelos_der_tobendeAI (_Creature);
}

void AddSC_boss_agathelos_der_tobende()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_agathelos_der_tobende";
    newscript->GetAI = &GetAI_boss_agathelos_der_tobende;
    newscript->RegisterSelf();
}

