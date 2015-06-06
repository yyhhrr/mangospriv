/* boss_charlga_klingenflanke */

#include "precompiled.h"

#define erneuerung	6077
#define kettenblitzschlag	8292
#define manastachel	8358
#define reinheit	8361

struct MANGOS_DLL_DECL boss_charlga_klingenflankeAI : public ScriptedAI
{
    boss_charlga_klingenflankeAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 erneuerung_timer;
    uint32 kettenblitzschlag_timer;
    uint32 manastachel_timer;
    uint32 reinheit_timer;

    void Reset()
    {
        erneuerung_timer = 5000;
        kettenblitzschlag_timer = 2000;
        manastachel_timer = 10000;
        reinheit_timer = 12000;
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

        if (erneuerung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,erneuerung);
            erneuerung_timer = random(20000,25000);
        } else erneuerung_timer -= diff;

        if (kettenblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kettenblitzschlag);
            kettenblitzschlag_timer = random(5000,6000);
        } else kettenblitzschlag_timer -= diff;

        if (manastachel_timer < diff)
        {
            DoCastSpellIfCan(m_creature,manastachel);
            manastachel_timer = random(20000,30000);
        } else manastachel_timer -= diff;

        if (reinheit_timer < diff)
        {
            DoCastSpellIfCan(m_creature,reinheit);
            reinheit_timer = random(25000,26000);
        } else reinheit_timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_charlga_klingenflanke(Creature *_Creature)
{
    return new boss_charlga_klingenflankeAI (_Creature);
}

void AddSC_boss_charlga_klingenflanke()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_charlga_klingenflanke";
    newscript->GetAI = &GetAI_boss_charlga_klingenflanke;
    newscript->RegisterSelf();
}

