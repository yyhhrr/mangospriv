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
#include "blackfathom_deeps.h"

#define gedankenschlag	15587
#define schlaf			8399

struct MANGOS_DLL_DECL boss_lord_kelrisAI : public ScriptedAI
{
    boss_lord_kelrisAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}
	
	ScriptedInstance* pInstance;
	Unit* target;

    uint32 gedankenschlag_timer;
    uint32 schlaf_timer;

    void Reset()
    {
        gedankenschlag_timer	= 7000;
        schlaf_timer			= 12000;
    }

    void JustDied(Unit*)
    {
        if (pInstance)
			pInstance->SetData(TYPE_KELRIS, DONE);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (gedankenschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),gedankenschlag);
            gedankenschlag_timer = random(11000,15000);
        } else gedankenschlag_timer -= diff;

        if (schlaf_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schlaf);
            schlaf_timer = random(10000,30000);
        } else schlaf_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lord_kelris(Creature *_Creature)
{
    return new boss_lord_kelrisAI (_Creature);
}

void AddSC_boss_lord_kelris()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_kelris";
    newscript->GetAI = &GetAI_boss_lord_kelris;
    newscript->RegisterSelf();
}

