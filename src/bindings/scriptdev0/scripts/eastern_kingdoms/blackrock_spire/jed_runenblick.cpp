/* jed_runenblick */

#include "precompiled.h"

#define schildattacke	15749
#define schildhieb	11972
#define schlag	14516


struct MANGOS_DLL_DECL jed_runenblickAI : public ScriptedAI
{
    jed_runenblickAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 schildattacke_timer;
    uint32 schildhieb_timer;
    uint32 schlag_timer;

    void Reset()
    {
        schildattacke_timer = 500;
        schildhieb_timer = 3000;
        schlag_timer = 2000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("So nicht!",LANG_UNIVERSAL,NULL);
	error_log("SD2: JED RETURNED");
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

        if (schildattacke_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schildattacke);
            schildattacke_timer = random(15000,30000);
        } else schildattacke_timer -= diff;

        if (schildhieb_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schildhieb);
            schildhieb_timer = random(10000,15000);
        } else schildhieb_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(6000,10000);
        } else schlag_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_jed_runenblick(Creature *_Creature)
{
    return new jed_runenblickAI (_Creature);
}

void AddSC_jed_runenblick()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="jed_runenblick";
    newscript->GetAI = &GetAI_jed_runenblick;
    newscript->RegisterSelf();
}