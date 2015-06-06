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
#												    Schattenjägerin Vosh'gajin	#
#################################################################################

ScriptName: Schattenjägerin Vosh'gajin
Instanz: Schwarzfelsspitze
Staus in %: 100%
Abgeschlossen?: Ja
*/

#include "precompiled.h"

#define SPELL_BLUTFLUCH    16098
#define SPELL_VERHEXEN     16097

struct MANGOS_DLL_DECL schattenjaegerin_voshAI : public ScriptedAI
{
    schattenjaegerin_voshAI(Creature* pCreature) : ScriptedAI(pCreature) {
        Reset();
    }

    uint32 Blutfluch_Timer;
    uint32 Verhexen_Timer;

    void Reset()
    {
        Blutfluch_Timer = 5000;
        Verhexen_Timer = 8000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (Blutfluch_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_BLUTFLUCH);
            Blutfluch_Timer = 6000;
        } else Blutfluch_Timer -= diff;

        if (Verhexen_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target) DoCastSpellIfCan(target,SPELL_VERHEXEN);
            Verhexen_Timer = 12000;
        } else Verhexen_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_schattenjaegerin_vosh(Creature* pCreature)
{
    return new schattenjaegerin_voshAI(pCreature);
}

void AddSC_schattenjaegerin_vosh()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "schattenjaegerin_vosh";
    newscript->GetAI = &GetAI_schattenjaegerin_vosh;
    newscript->RegisterSelf();
}