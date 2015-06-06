/* boss_haeuptling_ukorz_sandskalp */

#include "precompiled.h"
#include "zulfarrak.h"

#define berserkerhaltung	7366
#define raserei	8269
#define spalten	15496
#define weiter_streich	11837


struct MANGOS_DLL_DECL boss_haeuptling_ukorz_sandskalpAI : public ScriptedAI
{
    boss_haeuptling_ukorz_sandskalpAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 berserkerhaltung_timer;
    uint32 raserei_timer;
    uint32 spalten_timer;
    uint32 weiter_streich_timer;

    void Reset()
    {
        berserkerhaltung_timer = 500;
        raserei_timer = 15000;
        spalten_timer = 2000;
        weiter_streich_timer = 5000;
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
            berserkerhaltung_timer = random(9000000,9999999);
        } else berserkerhaltung_timer -= diff;

        if (raserei_timer < diff)
        {
            DoCastSpellIfCan(m_creature,raserei);
            raserei_timer = random(121000,122000);
        } else raserei_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(7000,10000);
        } else spalten_timer -= diff;

        if (weiter_streich_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),weiter_streich);
            weiter_streich_timer = random(4000,9000);
        } else weiter_streich_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_haeuptling_ukorz_sandskalp(Creature *_Creature)
{
    return new boss_haeuptling_ukorz_sandskalpAI (_Creature);
}

void AddSC_boss_haeuptling_ukorz_sandskalp()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_haeuptling_ukorz_sandskalp";
    newscript->GetAI = &GetAI_boss_haeuptling_ukorz_sandskalp;
    newscript->RegisterSelf();
}

