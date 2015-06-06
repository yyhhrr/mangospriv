/* boss_antu_sul */

#include "precompiled.h"
#include "zulfarrak.h"

#define antu_suls_diener	11894
#define blitzheilung	10915
#define erdschock	10413
#define feuernova	11314
#define heilungszauberschutz	11899
#define kettenblitzschlag	2860
#define totem_des_erdengriffs	8376
#define totem_des_lavaschwalls	8264
#define totem_des_heilenden_flusses	10643
#define welle_der_heilung_von_antu_sul	11895


struct MANGOS_DLL_DECL boss_antu_sulAI : public ScriptedAI
{
    boss_antu_sulAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 antu_suls_diener_timer;
    uint32 blitzheilung_timer;
    uint32 erdschock_timer;
    uint32 feuernova_timer;
    uint32 heilungszauberschutz_timer;
    uint32 kettenblitzschlag_timer;
    uint32 totem_des_erdengriffs_timer;
    uint32 totem_des_lavaschwalls_timer;
    uint32 totem_des_heilenden_flusses_timer;
    uint32 welle_der_heilung_von_antu_sul_timer;


    void Reset()
    {
        antu_suls_diener_timer = 10000;
        blitzheilung_timer = 9000;
        erdschock_timer = 3000;
        feuernova_timer = 4000;
        heilungszauberschutz_timer = 8000;
        kettenblitzschlag_timer = 5000;
        totem_des_erdengriffs_timer = 500;
        totem_des_lavaschwalls_timer = 600;
        totem_des_heilenden_flusses_timer = 700;
        welle_der_heilung_von_antu_sul_timer=
            12000;
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

        if (antu_suls_diener_timer < diff)
        {
            DoCastSpellIfCan(m_creature,antu_suls_diener);
            antu_suls_diener_timer = random(600000,700000);
        } else antu_suls_diener_timer -= diff;

        if (blitzheilung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,blitzheilung);
            blitzheilung_timer = random(9000,15000);
        } else blitzheilung_timer -= diff;

        if (erdschock_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),erdschock);
            erdschock_timer = random(3000,5000);
        } else erdschock_timer -= diff;

        if (feuernova_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),feuernova);
            feuernova_timer = random(4000,8000);
        } else feuernova_timer -= diff;

        if (heilungszauberschutz_timer < diff)
        {
            DoCastSpellIfCan(m_creature,heilungszauberschutz);
            heilungszauberschutz_timer = random(8000,15000);
        } else heilungszauberschutz_timer -= diff;

        if (kettenblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),kettenblitzschlag);
            kettenblitzschlag_timer = random(5000,15000);
        } else kettenblitzschlag_timer -= diff;

        if (totem_des_erdengriffs_timer < diff)
        {
            DoCastSpellIfCan(m_creature,totem_des_erdengriffs);
            totem_des_erdengriffs_timer = random(30500,30700);
        } else totem_des_erdengriffs_timer -= diff;

        if (totem_des_lavaschwalls_timer < diff)
        {
            DoCastSpellIfCan(m_creature,totem_des_lavaschwalls);
            totem_des_lavaschwalls_timer = random(30600,30800);
        } else totem_des_lavaschwalls_timer -= diff;

        if (totem_des_heilenden_flusses_timer < diff)
        {
            DoCastSpellIfCan(m_creature,totem_des_heilenden_flusses);
            totem_des_heilenden_flusses_timer = random(30700,30900);
        } else totem_des_heilenden_flusses_timer -= diff;

        if (welle_der_heilung_von_antu_sul_timer < diff)
        {
            DoCastSpellIfCan(m_creature,welle_der_heilung_von_antu_sul);
            welle_der_heilung_von_antu_sul_timer = random(12000,28000);
        } else welle_der_heilung_von_antu_sul_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_antu_sul(Creature *_Creature)
{
    return new boss_antu_sulAI (_Creature);
}

void AddSC_boss_antu_sul()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_antu_sul";
    newscript->GetAI = &GetAI_boss_antu_sul;
    newscript->RegisterSelf();
}

