/* lord_valthalak */

#include "precompiled.h"

#define schattenblitzsalve	27382
#define schattenstab	27338
#define schattenwut	27286
#define spektralen_assassinen_beschwoeren	27249


struct MANGOS_DLL_DECL lord_valthalakAI : public ScriptedAI
{
    lord_valthalakAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 schattenblitzsalve_timer;
    uint32 schattenstab_timer;
    uint32 schattenwut_timer;
    uint32 spektralen_assassinen_beschwoeren_timer;

    void Reset()
    {
        schattenblitzsalve_timer = 10000;
        schattenstab_timer = 3000;
        schattenwut_timer = 5000;
        spektralen_assassinen_beschwoeren_timer = 15000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Ihr wollt es also wirklich versuchen?",LANG_UNIVERSAL,NULL);
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
            return;
        }

        if (schattenblitzsalve_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schattenblitzsalve);
            schattenblitzsalve_timer = random(8000,15000);
        } else schattenblitzsalve_timer -= diff;

        if (schattenstab_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schattenstab);
            schattenstab_timer = random(5000,15000);
        } else schattenstab_timer -= diff;

        if (schattenwut_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenwut);
            schattenwut_timer = random(7000,12000);
        } else schattenwut_timer -= diff;

        if (spektralen_assassinen_beschwoeren_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spektralen_assassinen_beschwoeren);
            spektralen_assassinen_beschwoeren_timer = random(25000,30000);
        } else spektralen_assassinen_beschwoeren_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_lord_valthalak(Creature *_Creature)
{
    return new lord_valthalakAI (_Creature);
}

void AddSC_lord_valthalak()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="lord_valthalak";
    newscript->GetAI = &GetAI_lord_valthalak;
    newscript->RegisterSelf();
}