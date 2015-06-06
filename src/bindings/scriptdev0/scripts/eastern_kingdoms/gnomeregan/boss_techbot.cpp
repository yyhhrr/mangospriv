/* boss_techbot */

#include "precompiled.h"

#define flicken	10860
#define kampfnetz	10852
#define taeuschkaefer_beschwoeren	10858
#define verzoegerung	10855


struct MANGOS_DLL_DECL boss_techbotAI : public ScriptedAI
{
    boss_techbotAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 flicken_timer;
    uint32 kampfnetz_timer;
    uint32 taeuschkaefer_beschwoeren_timer;
    uint32 verzoegerung_timer;
    bool kaefer;

    void Reset()
    {
        flicken_timer;
        kampfnetz_timer = 6000;
        taeuschkaefer_beschwoeren_timer = 10000;
        verzoegerung_timer = 4000;
        kaefer = false;
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
            return;

        if (flicken_timer < diff && kaefer ==true)
        {
            DoCastSpellIfCan(m_creature,flicken);
            kaefer = false;
        } else flicken_timer -= diff;

        if (kampfnetz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kampfnetz);
            kampfnetz_timer = random(16000,18000);
        } else kampfnetz_timer -= diff;

        if (taeuschkaefer_beschwoeren_timer < diff)
        {
            DoCastSpellIfCan(m_creature,taeuschkaefer_beschwoeren);
            taeuschkaefer_beschwoeren_timer = random(25000,35000);
            kaefer=true;
            flicken_timer = random(3000,5000);
        } else taeuschkaefer_beschwoeren_timer -= diff;

        if (verzoegerung_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),verzoegerung);
            verzoegerung_timer = random(17000,20000);
        } else verzoegerung_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_techbot(Creature *_Creature)
{
    return new boss_techbotAI (_Creature);
}

void AddSC_boss_techbot()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_techbot";
    newscript->GetAI = &GetAI_boss_techbot;
    newscript->RegisterSelf();
}

