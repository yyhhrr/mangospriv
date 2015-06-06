/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#													 Schwarzfelsspitze			#
#												   		   Vealan				#
#################################################################################

ScriptName: Vealan
Instanz: Schwarzfelsspitze
Staus in %: 100%
Abgeschlossen?: Ja
*/

#include "precompiled.h"

#define SPELL_VEALAN_SPAWNS			16634

#define MODEL_BOESE					9642
#define MODEL_MENSCH				9643
#define FACTION_FREUNDLICH			35

#define NAME_VEALAN					"Vealan"

struct MANGOS_DLL_DECL vaelanAI : public ScriptedAI
{
    vaelanAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    void Reset()
    {
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_BOESE);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterSay("Hey, Ihr da! Kommt mal etwas n\303\244her...", LANG_UNIVERSAL, NULL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            return;
        }

        m_creature->MonsterYell("HALT! Ich bin nicht der, den Ihr vermutet...", LANG_UNIVERSAL, NULL);
        m_creature->CastSpell(m_creature, 16634, true, NULL, NULL, ObjectGuid());
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_MENSCH);
        m_creature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, FACTION_FREUNDLICH);
        m_creature->SetName(NAME_VEALAN);

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_vaelan(Creature *_Creature)
{
    return new vaelanAI (_Creature);
}

void AddSC_vaelan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="vaelan";
    newscript->GetAI = &GetAI_vaelan;
    newscript->RegisterSelf();
}