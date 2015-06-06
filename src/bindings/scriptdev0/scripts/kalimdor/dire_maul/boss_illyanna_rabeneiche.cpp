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

#define Spell_Erschuetternder_Schuss	22914
#define Spell_Feuerbrandfalle			22910
#define Spell_Mehrfachschuss			20735
#define Spell_Salve						22908

struct MANGOS_DLL_DECL boss_illyanna_rabeneicheAI : public ScriptedAI
{
    boss_illyanna_rabeneicheAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;
    Unit* target;

    uint32 Erschuetternder_Schuss_Timer;
    uint32 Feuerbrandfalle_Timer;
    uint32 Mehrfachschuss_Timer;
    uint32 Salve_Timer;

    void Reset()
    {
        Erschuetternder_Schuss_Timer 	= 6000;
        Feuerbrandfalle_Timer 			= 15000;
        Mehrfachschuss_Timer 			= 8000;
        Salve_Timer 					= 10000;
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

        if (Erschuetternder_Schuss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Erschuetternder_Schuss);
            Erschuetternder_Schuss_Timer = random(6000,8000);
        } else Erschuetternder_Schuss_Timer-=diff;

        if (Feuerbrandfalle_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Feuerbrandfalle);
            Feuerbrandfalle_Timer = random(15000,30000);
        } else Feuerbrandfalle_Timer -= diff;

        if (Mehrfachschuss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Mehrfachschuss);
            Mehrfachschuss_Timer = random(8000,11000);
        } else Mehrfachschuss_Timer -= diff;

        if (Salve_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Salve);
            Salve_Timer = random(16000,19000);
        } else Salve_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_illyanna_rabeneiche(Creature *_Creature)
{
    return new boss_illyanna_rabeneicheAI (_Creature);
}

#define Spell_Sturmangriff		22911
#define Spell_Zermalmen			17156

struct MANGOS_DLL_DECL npc_FerraAI : public ScriptedAI
{
    npc_FerraAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Sturmangriff_Timer;
	uint32 Zermalmen_Timer;

    void Reset()
    {
		Sturmangriff_Timer		= 8000;
		Zermalmen_Timer			= 5000;
    }

    void UpdateAI(const uint32 diff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

        if (Sturmangriff_Timer < diff)
        {
			Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
			if (DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), Spell_Sturmangriff) == CAST_OK)
				Sturmangriff_Timer = urand(5000, 10000);

        }
		else
			Sturmangriff_Timer -= diff;
		
        if (Zermalmen_Timer < diff)
        {
			if (DoCastSpellIfCan(m_creature->getVictim(), Spell_Zermalmen) == CAST_OK)
				Zermalmen_Timer = urand(5000, 7000);

        }
		else
			Zermalmen_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_Ferra(Creature *_Creature)
{
    return new npc_FerraAI (_Creature);
}

void AddSC_boss_illyanna_rabeneiche()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_illyanna_rabeneiche";
    newscript->GetAI = &GetAI_boss_illyanna_rabeneiche;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name="npc_Ferra";
    newscript->GetAI = &GetAI_npc_Ferra;
    newscript->RegisterSelf();
}