/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################
*/

#include "precompiled.h"
#include "dire_maul.h"

#define fengus_wildheit	22817
#define raserei			8269
#define schildattacke	15749
#define schildhieb		11972
#define schlag			15580
#define wegschlagen		10101

struct MANGOS_DLL_DECL boss_guard_fengusAI : public ScriptedAI
{
    boss_guard_fengusAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

    Unit* target;

    uint32 fengus_wildheit_timer;
    bool raserei_stat;
    uint32 schildattacke_timer;
    uint32 schildhieb_timer;
    uint32 schlag_timer;
    uint32 wegschlagen_timer;

    void Reset()
    {
        fengus_wildheit_timer 	= 1000;
        raserei_stat 			= false;
        schildattacke_timer 	= 5000;
        schildhieb_timer 		= 7000;
        schlag_timer 			= 8000;
        wegschlagen_timer 		= 12000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (fengus_wildheit_timer < diff)
        {
            DoCastSpellIfCan(m_creature,fengus_wildheit);
            fengus_wildheit_timer = random(3600000,3700000);
        } else fengus_wildheit_timer -= diff;

        if (raserei_stat ==false && m_creature->GetHealthPercent() < 10.0f)
        {
            DoCastSpellIfCan(m_creature,raserei);
			m_creature->CallForHelp(15.0f);
            raserei_stat = true;
        }

        if (schildattacke_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schildattacke);
            schildattacke_timer = random(15000,16000);
        } else schildattacke_timer -= diff;

        if (schildhieb_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schildhieb);
            schildhieb_timer = random(8000,10000);
        } else schildhieb_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(6000,8000);
        } else schlag_timer -= diff;

        if (wegschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),wegschlagen);
            wegschlagen_timer = random(12000,15000);
        } else wegschlagen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_guard_fengus(Creature *_Creature)
{
    return new boss_guard_fengusAI (_Creature);
}

void AddSC_boss_guard_fengus()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_guard_fengus";
    newscript->GetAI = &GetAI_boss_guard_fengus;
    newscript->RegisterSelf();
}

