/* boss_ogom_der_elende */

#include "precompiled.h"

#define fluch_der_schwaeche	12493
#define schattenblitz	12471
#define schattenwort_schmerz	11639


struct MANGOS_DLL_DECL boss_ogom_der_elendeAI : public ScriptedAI
{
    boss_ogom_der_elendeAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 fluch_der_schwaeche_timer;
    uint32 schattenblitz_timer;
    uint32 schattenwort_schmerz_timer;

    void Reset()
    {
        fluch_der_schwaeche_timer = 500;
        schattenblitz_timer = 2000;
        schattenwort_schmerz_timer = 1000;
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

        if (fluch_der_schwaeche_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),fluch_der_schwaeche);
            fluch_der_schwaeche_timer = random(120000,120500);
        } else fluch_der_schwaeche_timer -= diff;

        if (schattenblitz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitz);
            schattenblitz_timer = random(3000,7000);
        } else schattenblitz_timer -= diff;

        if (schattenwort_schmerz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenwort_schmerz);
            schattenwort_schmerz_timer = random(4000,10000);
        } else schattenwort_schmerz_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_ogom_der_elende(Creature *_Creature)
{
    return new boss_ogom_der_elendeAI (_Creature);
}

void AddSC_boss_ogom_der_elende()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ogom_der_elende";
    newscript->GetAI = &GetAI_boss_ogom_der_elende;
    newscript->RegisterSelf();
}

