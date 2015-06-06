/* boss_eranikus_schemen */

#include "precompiled.h"

#define hauen	3391
#define kriegsdonner	11876
#define saeureatem	12891
#define tiefer_schlummer	12890


struct MANGOS_DLL_DECL boss_eranikus_schemenAI : public ScriptedAI
{
    boss_eranikus_schemenAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 hauen_timer;
    uint32 kriegsdonner_timer;
    uint32 saeureatem_timer;
    uint32 tiefer_schlummer_timer;

    void Reset()
    {
        hauen_timer = 3000;
        kriegsdonner_timer = 6000;
        saeureatem_timer = 4500;
        tiefer_schlummer_timer = 6500;
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

        if (hauen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hauen);
            hauen_timer = random(5000,10000);
        } else hauen_timer -= diff;

        if (kriegsdonner_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kriegsdonner);
            kriegsdonner_timer = random(7000,12000);
        } else kriegsdonner_timer -= diff;

        if (saeureatem_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),saeureatem);
            saeureatem_timer = random(4000,8000);
        } else saeureatem_timer -= diff;

        if (tiefer_schlummer_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),tiefer_schlummer);
            tiefer_schlummer_timer = random(10000,15000);
        } else tiefer_schlummer_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_eranikus_schemen(Creature *_Creature)
{
    return new boss_eranikus_schemenAI (_Creature);
}

void AddSC_boss_eranikus_schemen()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_eranikus_schemen";
    newscript->GetAI = &GetAI_boss_eranikus_schemen;
    newscript->RegisterSelf();
}

