/* Vectus */
#include "precompiled.h"

#define SPELL_DRUCKWELLE        16046
#define SPELL_FLAMMENSTOSS       18399

struct MANGOS_DLL_DECL vectusAI : public ScriptedAI
{
    vectusAI(Creature *c) : ScriptedAI(c) {}

    uint32 druckwelle_Timer;
    uint32 flammenstoss_Timer;

    void Reset()
    {
        druckwelle_Timer	= 2000;
        flammenstoss_Timer	= 5000;
    }

    void Aggro(Unit *who)
    {
        m_creature->CallForHelp(VISIBLE_RANGE);

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (druckwelle_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DRUCKWELLE);
            druckwelle_Timer = random(5000,10000);
        } else druckwelle_Timer -= diff;


        if (flammenstoss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FLAMMENSTOSS);
            flammenstoss_Timer = 12000;
        } else flammenstoss_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_vectus(Creature *_Creature)
{
    return new vectusAI (_Creature);
}

void AddSC_vectus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="vectus";
    newscript->GetAI = &GetAI_vectus;
    newscript->RegisterSelf();
}