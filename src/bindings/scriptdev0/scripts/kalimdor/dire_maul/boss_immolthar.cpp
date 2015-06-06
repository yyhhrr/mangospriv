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

#define Spell_Auge_von_Immolthar	22899
#define Spell_Infizierter_Biss		16128
#define Spell_Portal_von_Immolthar	22950
#define Spell_Raserei				8269
#define Spell_Trampeln				5568

struct MANGOS_DLL_DECL boss_immoltharAI : public ScriptedAI
{
    boss_immoltharAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Auge_von_Immolthar_Timer;
    uint32 Infizierter_Biss_Timer;
    uint32 Portal_von_Immolthar_Timer;
    uint32 Raserei_Timer;
	uint32 Trampeln_Timer;

    void Reset()
    {
        Auge_von_Immolthar_Timer 	= 10000;
        Infizierter_Biss_Timer 		= 9000;
        Portal_von_Immolthar_Timer 	= 8000;
        Raserei_Timer 				= 6000;
		Trampeln_Timer				= 7000;
    }

    void Aggro(Unit*)
    {
        if (pInstance)
			pInstance->SetData(TYPE_IMMOLTHAR, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_IMMOLTHAR, FAIL);
	}

    void JustDied(Unit*)
    {
		if (pInstance)
			pInstance->SetData(TYPE_IMMOLTHAR, DONE);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Auge_von_Immolthar_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Auge_von_Immolthar);
            Auge_von_Immolthar_Timer = random(18000,20000);
        } else Auge_von_Immolthar_Timer -= diff;

        if (Infizierter_Biss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Infizierter_Biss);
            Infizierter_Biss_Timer = random(15000,20000);
        } else Infizierter_Biss_Timer -= diff;

        if (Portal_von_Immolthar_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Portal_von_Immolthar);
            Portal_von_Immolthar_Timer = random(9000,15000);
        } else Portal_von_Immolthar_Timer -= diff;

        if (Raserei_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Raserei);
            Raserei_Timer = random(120000,121000);
        } else Raserei_Timer -= diff;
		
        if (Trampeln_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Trampeln);
            Trampeln_Timer = random(8000,10000);
        } else Trampeln_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_immolthar(Creature *_Creature)
{
    return new boss_immoltharAI (_Creature);
}

#define Spell_Auge_von_Immolthar_ADD		22909

struct MANGOS_DLL_DECL npc_Auge_von_ImmoltharAI : public ScriptedAI
{
    npc_Auge_von_ImmoltharAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Auge_von_Immolthar_Timer;

    void Reset()
    {
		Auge_von_Immolthar_Timer		= 2000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Auge_von_Immolthar_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Auge_von_Immolthar_ADD);
            Auge_von_Immolthar_Timer = random(4000,5000);
        } else Auge_von_Immolthar_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_Auge_von_Immolthar(Creature *_Creature)
{
    return new npc_Auge_von_ImmoltharAI (_Creature);
}

void AddSC_boss_immolthar()
{
    Script *newscript;
	
    newscript = new Script;
    newscript->Name="boss_immolthar";
    newscript->GetAI = &GetAI_boss_immolthar;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name="npc_Auge_von_Immolthar";
    newscript->GetAI = &GetAI_npc_Auge_von_Immolthar;
    newscript->RegisterSelf();
}