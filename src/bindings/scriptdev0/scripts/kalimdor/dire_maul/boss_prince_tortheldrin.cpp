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

#define Spell_Arkanschlag	22920
#define Spell_Beschwoeren	22995
#define Spell_Gegenzauber	20537
#define Spell_Hauen			3391
#define Spell_Wirbelwind	15589

struct MANGOS_DLL_DECL boss_prince_tortheldrinAI : public ScriptedAI
{
    boss_prince_tortheldrinAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Arkanschlag_Timer;
    uint32 Beschwoeren_Timer;
    uint32 Gegenzauber_Timer;
    uint32 Hauen_Timer;
	uint32 Wirbelwind_Timer;

    void Reset()
    {
        Arkanschlag_Timer 	= 5000;
        Beschwoeren_Timer 	= 9000;
        Gegenzauber_Timer 	= 8000;
        Hauen_Timer 		= 6000;
		Wirbelwind_Timer	= 7000;
    }

    void Aggro(Unit*)
    {
        if (pInstance)
			pInstance->SetData(TYPE_PRINCE, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_PRINCE, FAIL);
	}

    void JustDied(Unit*)
    {
		if (pInstance)
			pInstance->SetData(TYPE_PRINCE, DONE);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Arkanschlag_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Arkanschlag);
			m_creature->DeleteThreatList();
			m_creature->SetInCombatWithZone();
            Arkanschlag_Timer = random(15000,18000);
        } else Arkanschlag_Timer -= diff;

        if (Beschwoeren_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Beschwoeren);
			m_creature->SetInCombatWithZone();
            Beschwoeren_Timer = random(15000,20000);
        } else Beschwoeren_Timer -= diff;

        if (Gegenzauber_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Gegenzauber);
            Gegenzauber_Timer = random(9000,15000);
        } else Gegenzauber_Timer -= diff;

        if (Hauen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Hauen);
            Hauen_Timer = random(7000,11000);
        } else Hauen_Timer -= diff;
		
        if (Wirbelwind_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Wirbelwind);
            Wirbelwind_Timer = random(10000,12000);
        } else Wirbelwind_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_prince_tortheldrin(Creature *_Creature)
{
    return new boss_prince_tortheldrinAI (_Creature);
}

void AddSC_boss_prince_tortheldrin()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_prince_tortheldrin";
    newscript->GetAI = &GetAI_boss_prince_tortheldrin;
    newscript->RegisterSelf();
}