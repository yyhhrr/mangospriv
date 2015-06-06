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

#define opferung			22651
#define starke_schmerzen	22478

struct MANGOS_DLL_DECL boss_zevrim_dornhufAI : public ScriptedAI
{
    boss_zevrim_dornhufAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* pInstance;
    Unit* target;

    uint32 opferung_timer;
    uint32 starke_schmerzen_timer;

    void Reset()
    {
        opferung_timer 			= 8000;
        starke_schmerzen_timer 	= 3000;
    }

    void Aggro(Unit*)
    {
        if (pInstance)
			pInstance->SetData(TYPE_ZEVRIM, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_ZEVRIM, FAIL);
	}

    void JustDied()
    {
		if (pInstance)
			pInstance->SetData(TYPE_ZEVRIM, DONE);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (opferung_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), opferung);
            opferung_timer = random(13000,18000);
        } else opferung_timer -= diff;

        if (starke_schmerzen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), starke_schmerzen);
            starke_schmerzen_timer = random(15000,20000);
        } else starke_schmerzen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_zevrim_dornhuf(Creature *_Creature)
{
    return new boss_zevrim_dornhufAI (_Creature);
}

void AddSC_boss_zevrim_dornhuf()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_zevrim_dornhuf";
    newscript->GetAI = &GetAI_boss_zevrim_dornhuf;
    newscript->RegisterSelf();
}