/* boss_hukku */

#include "precompiled.h"

#define blutfluch	12279
#define hukkus_waechter	12790
#define schattenblitz	12471
#define schattenblitzsalve	9081


struct MANGOS_DLL_DECL boss_hukkuAI : public ScriptedAI
{
    boss_hukkuAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 blutfluch_timer;
    uint32 hukkus_waechter_timer;
    uint32 schattenblitz_timer;
    uint32 schattenblitzsalve_timer;

    void Reset()
    {
        blutfluch_timer = 500;
        hukkus_waechter_timer = 0000;
        schattenblitz_timer = 2000;
        schattenblitzsalve_timer = 4000;
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

        if (blutfluch_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),blutfluch);
            blutfluch_timer = random(50000,100000);
        } else blutfluch_timer -= diff;

        if (hukkus_waechter_timer < diff)
        {
            DoCastSpellIfCan(m_creature,hukkus_waechter);
            hukkus_waechter_timer = random(50000,100000);
        } else hukkus_waechter_timer -= diff;

        if (schattenblitz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitz);
            schattenblitz_timer = random(4000,10000);
        } else schattenblitz_timer -= diff;

        if (schattenblitzsalve_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitzsalve);
            schattenblitzsalve_timer = random(5000,12000);
        } else schattenblitzsalve_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_hukku(Creature *_Creature)
{
    return new boss_hukkuAI (_Creature);
}

void AddSC_boss_hukku()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hukku";
    newscript->GetAI = &GetAI_boss_hukku;
    newscript->RegisterSelf();
}

