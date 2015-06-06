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
#include "scholomance.h"

enum Spells
{
    SPELL_IMMOLATE              = 15570,
    SPELL_UNHOLY_AURA           = 17467,
    SPELL_VEIL_OF_SHADOW        = 17820
};

struct MANGOS_DLL_DECL lord_alexei_barovAI : public ScriptedAI
{
    lord_alexei_barovAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 m_uiImmolateTimer;
    uint32 m_uiVeilofShadowTimer;

    void Reset()
    {
        m_uiImmolateTimer		= urand(4000, 6000);
        m_uiVeilofShadowTimer	= urand(10000, 15000);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
			pInstance->SetData(TYPE_ALEXEIBAROV, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_ALEXEIBAROV, FAIL);
	}

    void JustDied(Unit *killer)
    {
		if (pInstance)
			pInstance->SetData(TYPE_ALEXEIBAROV, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoCastSpellIfCan(m_creature, SPELL_UNHOLY_AURA, CAST_AURA_NOT_PRESENT);

        // Immolate
        if (m_uiImmolateTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_IMMOLATE);
            m_uiImmolateTimer = urand(8000, 10000);
        }
        else
            m_uiImmolateTimer -= uiDiff;

        // Veil of Shadow
        if (m_uiVeilofShadowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_VEIL_OF_SHADOW);
            m_uiVeilofShadowTimer = urand(15000, 20000);
        }
        else
            m_uiVeilofShadowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_lord_alexei_barov(Creature *_Creature)
{
    return new lord_alexei_barovAI (_Creature);
}

void AddSC_lord_alexei_barov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="lord_alexei_barov";
    newscript->GetAI = &GetAI_lord_alexei_barov;
    newscript->RegisterSelf();
}