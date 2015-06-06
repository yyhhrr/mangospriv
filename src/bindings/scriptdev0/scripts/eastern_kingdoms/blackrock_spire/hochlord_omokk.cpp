/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#														 Schwarzfelsspitze		#
#												   		   Hochlord Omokk	    #
#################################################################################

ScriptName: Hochlord Omokk
Instanz: Flammenschlund
Staus in %: 100%
Abgeschlossen?: Ja
*/

#include "precompiled.h"

#define SPELL_ZUSCHLAGEN    10101
#define SPELL_RASEREI       8269

struct MANGOS_DLL_DECL hochlord_omokkAI : public ScriptedAI
{
    hochlord_omokkAI(Creature* pCreature) : ScriptedAI(pCreature) {
        Reset();
    }

    uint32 Zuschlagen_Timer;
    uint32 Raserei_Timer;

    void Reset()
    {
        Zuschlagen_Timer = 5000;
        Raserei_Timer = 45000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (Zuschlagen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ZUSCHLAGEN);
            Zuschlagen_Timer = 6000;
        } else Zuschlagen_Timer -= diff;

        if (Raserei_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_RASEREI);
            Raserei_Timer = 120000;
        } else Raserei_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_hochlord_omokk(Creature* pCreature)
{
    return new hochlord_omokkAI(pCreature);
}

void AddSC_hochlord_omokk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "hochlord_omokk";
    newscript->GetAI = &GetAI_hochlord_omokk;
    newscript->RegisterSelf();
}