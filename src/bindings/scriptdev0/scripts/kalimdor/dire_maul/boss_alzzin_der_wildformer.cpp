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

#define baumgestalt				22688
#define dornen					22128
#define entwaffnen				22691
#define heimtueckischer_biss	19319
#define manaschwaeche			22661
#define terrorwolf				22660
#define wegschlagen				10101
#define welken					22662
#define wilde_regeneration		7948
#define zerfleischen			22689

struct MANGOS_DLL_DECL boss_alzzin_der_wildformerAI : public ScriptedAI
{
    boss_alzzin_der_wildformerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

    Unit* target;

    uint32 gestalt_timer;
    uint32 dornen_timer;
    uint32 entwaffnen_timer;
    uint32 heimtueckischer_biss_timer;
    uint32 manaschwaeche_timer;
    uint32 wegschlagen_timer;
    uint32 welken_timer;
    uint32 wilde_regeneration_timer;
    uint32 zerfleischen_timer;

    uint32 gestalt;

    void Reset()
    {
        gestalt_timer 				= 10000;
        dornen_timer 				= 2000;
        entwaffnen_timer 			= 4000;
        heimtueckischer_biss_timer 	= 3000;
        manaschwaeche_timer 		= 5000;
        wegschlagen_timer 			= 7000;
        welken_timer 				= 3000;
        wilde_regeneration_timer 	= 5000;
        zerfleischen_timer			= 5000;
        gestalt 					= 1000;
    }

    void Aggro(Unit*)
    {
        if (pInstance)
			pInstance->SetData(TYPE_ALZZIN, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_ALZZIN, FAIL);
	}

    void JustDied(Unit*)
    {
		if (pInstance)
			pInstance->SetData(TYPE_ALZZIN, DONE);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (gestalt_timer < diff)
        {
            switch (rand()%3)
            {
            case 0:
                DoCastSpellIfCan(m_creature,baumgestalt);
                gestalt_timer = 40000;
                gestalt = 1;
                break;
            case 1:
                DoCastSpellIfCan(m_creature,terrorwolf);
                gestalt_timer = 40000;
                gestalt = 2;
                break;
            case 2:
                gestalt_timer = 20000;
                gestalt = 0;
                break;
            }
        } else gestalt_timer -= diff;


        if (gestalt == 0)
        {
            if (entwaffnen_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),entwaffnen);
                entwaffnen_timer = random(10000,12000);
            } else entwaffnen_timer -= diff;

            if (manaschwaeche_timer < diff)
            {
                DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),manaschwaeche);
                manaschwaeche_timer = random(15000,16000);
            } else manaschwaeche_timer -= diff;

            if (wegschlagen_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),wegschlagen);
                wegschlagen_timer = random(8000,10000);
            } else wegschlagen_timer -= diff;
        }

        if (gestalt == 1)
        {
            if (dornen_timer < diff)
            {
                DoCastSpellIfCan(m_creature,dornen);
                dornen_timer = random(600000,610000);
            } else dornen_timer -= diff;

            if (welken_timer < diff)
            {
                DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),welken);
                welken_timer = random(5000,8000);
            } else welken_timer -= diff;

            if (wilde_regeneration_timer < diff)
            {
                DoCastSpellIfCan(m_creature,wilde_regeneration);
                wilde_regeneration_timer = random(16000,20000);
            } else wilde_regeneration_timer -= diff;

        }

        if ( gestalt == 2)
        {
            if (heimtueckischer_biss_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),heimtueckischer_biss);
                heimtueckischer_biss_timer = random(4000,6000);
            } else heimtueckischer_biss_timer -= diff;

            if (zerfleischen_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),zerfleischen);
                zerfleischen_timer = random(5000,10000);
            } else zerfleischen_timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_alzzin_der_wildformer(Creature *_Creature)
{
    return new boss_alzzin_der_wildformerAI (_Creature);
}

void AddSC_boss_alzzin_der_wildformer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_alzzin_der_wildformer";
    newscript->GetAI = &GetAI_boss_alzzin_der_wildformer;
    newscript->RegisterSelf();
}

