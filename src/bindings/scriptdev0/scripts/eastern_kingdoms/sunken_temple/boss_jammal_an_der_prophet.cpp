/* boss_jammal_an_der_prophet */

#include "precompiled.h"

#define flammenstoss	12468
#define totem_des_erdengriffs	8376
#define verhexung_von_jammal_an	12480
#define welle_der_heilung	12492


struct MANGOS_DLL_DECL boss_jammal_an_der_prophetAI : public ScriptedAI
{
    boss_jammal_an_der_prophetAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 flammenstoss_timer;
    uint32 totem_des_erdengriffs_timer;
    uint32 verhexung_von_jammal_an_timer;
    uint32 welle_der_heilung_timer;

    void Reset()
    {
        flammenstoss_timer = 2000;
        totem_des_erdengriffs_timer = 500;
        verhexung_von_jammal_an_timer = 500;
        welle_der_heilung_timer = 5000;
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

        if (flammenstoss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),flammenstoss);
            flammenstoss_timer = random(5000,12000);
        } else flammenstoss_timer -= diff;

        if (totem_des_erdengriffs_timer < diff)
        {
            DoCastSpellIfCan(m_creature,totem_des_erdengriffs);
            totem_des_erdengriffs_timer = random(30500,31000);
        } else totem_des_erdengriffs_timer -= diff;

        if (verhexung_von_jammal_an_timer < diff)
        {
            DoCastSpellIfCan(m_creature,verhexung_von_jammal_an);
            verhexung_von_jammal_an_timer = random(90000,100000);
        } else verhexung_von_jammal_an_timer -= diff;

        if (welle_der_heilung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,welle_der_heilung);
            welle_der_heilung_timer = random(7000,12000);
        } else welle_der_heilung_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_jammal_an_der_prophet(Creature *_Creature)
{
    return new boss_jammal_an_der_prophetAI (_Creature);
}

void AddSC_boss_jammal_an_der_prophet()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_jammal_an_der_prophet";
    newscript->GetAI = &GetAI_boss_jammal_an_der_prophet;
    newscript->RegisterSelf();
}

