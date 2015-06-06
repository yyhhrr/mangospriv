/* boss_hexendoktor_zum_rah */

#include "precompiled.h"
#include "zulfarrak.h"

#define schattenblitz	12739
#define schattenblitzsalve	15245
#define welle_der_heilung	12491
#define zauberschutz_von_zum_rah	11086


struct MANGOS_DLL_DECL boss_hexendoktor_zum_rahAI : public ScriptedAI
{
    boss_hexendoktor_zum_rahAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 schattenblitz_timer;
    uint32 schattenblitzsalve_timer;
    uint32 welle_der_heilung_timer;
    uint32 zauberschutz_von_zum_rah_timer;

    void Reset()
    {
        schattenblitz_timer = 2000;
        schattenblitzsalve_timer = 4000;
        welle_der_heilung_timer = 6000;
        zauberschutz_von_zum_rah_timer = 500;
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

        if (schattenblitz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitz);
            schattenblitz_timer = random(2000,8000);
        } else schattenblitz_timer -= diff;

        if (schattenblitzsalve_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitzsalve);
            schattenblitzsalve_timer = random(4000,12000);
        } else schattenblitzsalve_timer -= diff;

        if (welle_der_heilung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,welle_der_heilung);
            welle_der_heilung_timer = random(5000,12000);
        } else welle_der_heilung_timer -= diff;

        if (zauberschutz_von_zum_rah_timer < diff)
        {
            DoCastSpellIfCan(m_creature,zauberschutz_von_zum_rah);
            zauberschutz_von_zum_rah_timer = random(20500,20600);
        } else zauberschutz_von_zum_rah_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_hexendoktor_zum_rah(Creature *_Creature)
{
    return new boss_hexendoktor_zum_rahAI (_Creature);
}

void AddSC_boss_hexendoktor_zum_rah()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hexendoktor_zum_rah";
    newscript->GetAI = &GetAI_boss_hexendoktor_zum_rah;
    newscript->RegisterSelf();
}

