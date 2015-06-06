/* rar_zerillis */

#include "precompiled.h"
#include "zulfarrak.h"

#define frostschuss	12551
#define netz	6533
#define schiessen	15547


struct MANGOS_DLL_DECL rar_zerillisAI : public ScriptedAI
{
    rar_zerillisAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 frostschuss_timer;
    uint32 netz_timer;
    uint32 schiessen_timer;

    void Reset()
    {
        frostschuss_timer = 1000;
        netz_timer = 2500;
        schiessen_timer = 100;
    }

    void JustDied(Unit*)
    {
    }

    void KilledUnit(Unit*)
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

        if (frostschuss_timer < diff)
        {
            DoCastSpellIfCan(m_creature,frostschuss);
            frostschuss_timer = random(3000,8000);
        } else frostschuss_timer -= diff;

        if (netz_timer < diff)
        {
            DoCastSpellIfCan(m_creature,netz);
            netz_timer = random(2000,10000);
        } else netz_timer -= diff;

        if (schiessen_timer < diff)
        {
            DoCastSpellIfCan(m_creature,schiessen);
            schiessen_timer = random(1000,4000);
        } else schiessen_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_rar_zerillis(Creature *_Creature)
{
    return new rar_zerillisAI (_Creature);
}

void AddSC_rar_zerillis()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="rar_zerillis";
    newscript->GetAI = &GetAI_rar_zerillis;
    newscript->RegisterSelf();
}

