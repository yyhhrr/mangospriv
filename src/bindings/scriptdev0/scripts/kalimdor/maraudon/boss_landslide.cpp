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

/* ScriptData
SDName: Boss_Landslide
SD%Complete: 100
SDComment:
SDCategory: Maraudon
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_KNOCKAWAY   = 18670,
    SPELL_TRAMPLE     = 5568,
    SPELL_LANDSLIDE   = 21808
};

struct MANGOS_DLL_DECL boss_landslideAI : public ScriptedAI
{
    boss_landslideAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 KnockAway_Timer;
    uint32 Trample_Timer;
    uint32 Landslide_Timer;

    void Reset()
    {
        KnockAway_Timer = 8000;
        Trample_Timer = 4000;
        Landslide_Timer = 1000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
	
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //KnockAway_Timer
        if (KnockAway_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KNOCKAWAY);
            KnockAway_Timer = random(12000,15000);
        }else KnockAway_Timer -= diff;

        //Trample_Timer
        if (Trample_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_TRAMPLE);
            Trample_Timer = random(6000,9000);
        }else Trample_Timer -= diff;

        //Landslide
        if (m_creature->GetHealthPercent() < 50.0f)
        {
            if (Landslide_Timer < diff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(m_creature,SPELL_LANDSLIDE);
                Landslide_Timer = 60000;
            } else Landslide_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_landslide(Creature* pCreature)
{
    return new boss_landslideAI(pCreature);
}

void AddSC_boss_landslide()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_landslide";
    pNewScript->GetAI = &GetAI_boss_landslide;
    pNewScript->RegisterSelf();
}
