/* boss_mijan */

#include "precompiled.h"

#define dornen_aura	8148
#define erneureung	8362
#define heilungszauberschutz	11899
#define welle_der_heilung	12492


struct MANGOS_DLL_DECL boss_mijanAI : public ScriptedAI
{
    boss_mijanAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 dornen_aura_timer;
    uint32 erneureung_timer;
    uint32 heilungszauberschutz_timer;
    uint32 welle_der_heilung_timer;

    void Reset()
    {
        dornen_aura_timer = 500;
        erneureung_timer = 1000;
        heilungszauberschutz_timer = 6000;
        welle_der_heilung_timer = 3000;
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
            Reset();
            return;
        }

        if (dornen_aura_timer < diff)
        {
            DoCastSpellIfCan(m_creature,dornen_aura);
            dornen_aura_timer = random(60000,60500);
        } else dornen_aura_timer -= diff;

        if (erneureung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,erneureung);
            erneureung_timer = random(3000,8000);
        } else erneureung_timer -= diff;

        if (heilungszauberschutz_timer < diff)
        {
            DoCastSpellIfCan(m_creature,heilungszauberschutz);
            heilungszauberschutz_timer = random(4000,10000);
        } else heilungszauberschutz_timer -= diff;

        if (welle_der_heilung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,welle_der_heilung);
            welle_der_heilung_timer = random(5000,7000);
        } else welle_der_heilung_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_mijan(Creature *_Creature)
{
    return new boss_mijanAI (_Creature);
}

void AddSC_boss_mijan()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_mijan";
    newscript->GetAI = &GetAI_boss_mijan;
    newscript->RegisterSelf();
}

