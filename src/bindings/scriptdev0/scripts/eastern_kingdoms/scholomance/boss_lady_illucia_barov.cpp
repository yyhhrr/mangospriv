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
    SPELL_CURSE_OF_AGONY      = 18671,
    SPELL_DOMINATE_MIND       = 14515,
    SPELL_FEAR                = 12542,
    SPELL_SHADOW_SHOCK        = 17289,
    SPELL_SILENCE             = 12528
};

struct MANGOS_DLL_DECL lady_illucia_barovAI : public ScriptedAI
{
    lady_illucia_barovAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();	
	}

	ScriptedInstance* pInstance;

    uint32 m_uiCurseOfAgonyTimer;
    uint32 m_uiDominateMindTimer;
    uint32 m_uiShadowShockTimer;
    uint32 m_uiSilenceTimer;
    uint32 m_uiFearTimer;

    Unit* target;
    Unit* ged_target;

    void Reset()
    {
        m_uiCurseOfAgonyTimer	= urand(12000,14000);
        m_uiDominateMindTimer	= urand(17000,20000);
        m_uiShadowShockTimer	= urand(2000,4000);
        m_uiSilenceTimer		= urand(6000,9000);
        m_uiFearTimer			= urand(23000,25000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
			pInstance->SetData(TYPE_ILLUCIABAROV, IN_PROGRESS);

        m_creature->MonsterYell("Ich bin eine Lady, respektiert das!", LANG_UNIVERSAL, NULL);
    }

    void JustDied(Unit *killer)
    {
        if (pInstance)
			pInstance->SetData(TYPE_ILLUCIABAROV, DONE);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_ILLUCIABAROV, FAIL);
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Curse Of Agony
        if (m_uiCurseOfAgonyTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CURSE_OF_AGONY);
            m_uiCurseOfAgonyTimer = urand(35000, 45000);
        }
        else
            m_uiCurseOfAgonyTimer -= uiDiff;

        // Dominate Mind
        if (m_uiDominateMindTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_DOMINATE_MIND);
            m_uiDominateMindTimer = urand(25000, 35000);
        }
        else
            m_uiDominateMindTimer -= uiDiff;

        // Fear
        if (m_uiFearTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1))
                DoCastSpellIfCan(pTarget, SPELL_FEAR);
            m_uiFearTimer = urand(10000, 15000);
        }
        else
            m_uiFearTimer -= uiDiff;

        // Shadow Shock
        if (m_uiShadowShockTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_SHOCK);
            m_uiShadowShockTimer = urand(6000, 9000);
        }
        else
            m_uiShadowShockTimer -= uiDiff;

        // Silence
        if (m_uiSilenceTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SILENCE);
            m_uiSilenceTimer = urand (10000, 15000);
        }
        else
            m_uiSilenceTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_lady_illucia_barov(Creature *_Creature)
{
    return new lady_illucia_barovAI (_Creature);
}

void AddSC_lady_illucia_barov()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="lady_illucia_barov";
    newscript->GetAI = &GetAI_lady_illucia_barov;
    newscript->RegisterSelf();
}