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

enum
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,

    SPELL_BACKHAND              = 18103,
    SPELL_FRENZY                = 8269,
    SPELL_REND                  = 16509
};

struct MANGOS_DLL_DECL doktor_theolen_krastinovAI : public ScriptedAI
{
    doktor_theolen_krastinovAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();	
	}

	ScriptedInstance* pInstance;

    uint32 m_uiBackhandTimer;
    uint32 m_uiFrenzyTimer;
    uint32 m_uiRendTimer;

    void Reset()
    {
        m_uiBackhandTimer	= urand(5000, 6000);
        m_uiFrenzyTimer		= 1000;
        m_uiRendTimer		= urand(9000, 11000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
			pInstance->SetData(TYPE_THEOLEN, IN_PROGRESS);

        m_creature->MonsterYell("Mal sehen was wir hier haben!", LANG_UNIVERSAL, NULL);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_THEOLEN, FAIL);
	}

    void JustDied(Unit* Killer)
    {
        if (pInstance)
			pInstance->SetData(TYPE_THEOLEN, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Backhand
        if (m_uiBackhandTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BACKHAND);
            m_uiBackhandTimer = urand(5000, 7000);
        }
        else
            m_uiBackhandTimer -= uiDiff;

        // Frenzy
        if (m_creature->GetHealthPercent() < 33.0f)
        {
            if (m_uiFrenzyTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
                {
                    DoScriptText(EMOTE_GENERIC_FRENZY_KILL, m_creature);
                    m_uiFrenzyTimer = 120000;
                }
            }
            else
                m_uiFrenzyTimer -= uiDiff;
        }

        // Rend
        if (m_uiRendTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_REND);
            m_uiRendTimer = urand(8000, 10000);
        }
        else
            m_uiRendTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_doktor_theolen_krastinov(Creature *_Creature)
{
    return new doktor_theolen_krastinovAI (_Creature);
}

void AddSC_doktor_theolen_krastinov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="doktor_theolen_krastinov";
    newscript->GetAI = &GetAI_doktor_theolen_krastinov;
    newscript->RegisterSelf();
}