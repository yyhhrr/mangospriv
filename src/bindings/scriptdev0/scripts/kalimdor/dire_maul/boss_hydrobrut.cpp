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

#define hydroling_beschwoeren	22714
#define massiver_geysir			22421
#define springflut				22419
#define untertauchen			22420

struct MANGOS_DLL_DECL boss_hydrobrutAI : public ScriptedAI
{
    boss_hydrobrutAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

    Unit* target;

    uint32 hydroling_beschwoeren_timer;
    uint32 massiver_geysir_timer;
    uint32 springflut_timer;
    uint32 untertauchen_timer;

    void Reset()
    {
        hydroling_beschwoeren_timer 	= 10000;
        massiver_geysir_timer 			= 7000;
        springflut_timer 				= 4000;
        untertauchen_timer 				= 5000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (hydroling_beschwoeren_timer < diff)
        {
            DoCastSpellIfCan(m_creature,hydroling_beschwoeren);
            hydroling_beschwoeren_timer = random(15000,21000);
        } else hydroling_beschwoeren_timer -= diff;

        if (massiver_geysir_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),massiver_geysir);
            massiver_geysir_timer = random(8000,12000);
        } else massiver_geysir_timer -= diff;

        if (springflut_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),springflut);
            springflut_timer = random(7000,10000);
        } else springflut_timer -= diff;

        if (untertauchen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),untertauchen);
            untertauchen_timer = random(60000,61000);
        } else untertauchen_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hydrobrut(Creature *_Creature)
{
    return new boss_hydrobrutAI (_Creature);
}

void AddSC_boss_hydrobrut()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hydrobrut";
    newscript->GetAI = &GetAI_boss_hydrobrut;
    newscript->RegisterSelf();
}

