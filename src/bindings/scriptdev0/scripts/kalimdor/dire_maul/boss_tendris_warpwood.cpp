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

#define Spell_Umschlingen		22994
#define Spell_Trampeln			5568
#define Spell_Greifende_Ranken	22924
#define Spell_Uppercut			22916

enum
{
	ANCIENT_EQUINE_SPIRIT = 14566
};

struct MANGOS_DLL_DECL boss_tendris_warpwoodAI : public ScriptedAI
{
    boss_tendris_warpwoodAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Umschlingen_Timer;
    uint32 Trampeln_Timer;
    uint32 Greifende_Ranken_Timer;
    uint32 Uppercut_Timer;

    void Reset()
    {
        Umschlingen_Timer 		= 5000;
        Trampeln_Timer 			= 6000;
        Greifende_Ranken_Timer 	= 3000;
        Uppercut_Timer 			= 6000;
    }

    void Aggro(Unit*)
    {
//        if (pInstance)
//			pInstance->SetData(TYPE_WARPWOOD, IN_PROGRESS);
    }

	void JustReachedHome()
	{
//		if (pInstance)
//			pInstance->SetData(TYPE_WARPWOOD, FAIL);
	}

    void JustDied(Unit*)
    {
//		if (pInstance)
//			pInstance->SetData(TYPE_WARPWOOD, DONE);
		float x, y, z;
		m_creature->GetPosition(x, y, z);
		const uint32 fiveHours = 18000000;
		m_creature->SummonCreature(ANCIENT_EQUINE_SPIRIT, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, fiveHours);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Umschlingen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Umschlingen);
            Umschlingen_Timer = random(10000,12000);
        } else Umschlingen_Timer -= diff;

        if (Trampeln_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Trampeln);
            Trampeln_Timer = random(8000,12000);
        } else Trampeln_Timer -= diff;

        if (Greifende_Ranken_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Greifende_Ranken_Timer);
            Greifende_Ranken_Timer = random(9000,13000);
        } else Greifende_Ranken_Timer -= diff;

        if (Uppercut_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Uppercut);
            Uppercut_Timer = random(9000,14000);
        } else Uppercut_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_tendris_warpwood(Creature *_Creature)
{
    return new boss_tendris_warpwoodAI (_Creature);
}

void AddSC_boss_tendris_warpwood()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_tendris_warpwood";
    newscript->GetAI = &GetAI_boss_tendris_warpwood;
    newscript->RegisterSelf();
}
