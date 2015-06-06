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

#define furcherregender_schrei	19134
#define gegenschlag				22857
#define haescher_rufen			22860
#define toedliches_spalten		22859
#define wutanfall				8599


struct MANGOS_DLL_DECL boss_hauptmann_krombruchAI : public ScriptedAI
{
    boss_hauptmann_krombruchAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

    Unit* target;

    uint32 furcherregender_schrei_timer;
    uint32 gegenschlag_timer;
    uint32 toedliches_spalten_timer;
    bool wutanfall_stat;
	bool heascher_rufen_stat;

    void Reset()
    {
        furcherregender_schrei_timer = 11000;
        gegenschlag_timer = 7000;
        toedliches_spalten_timer = 9000;
        wutanfall_stat =  false;
		heascher_rufen_stat = false;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (furcherregender_schrei_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),furcherregender_schrei);
            furcherregender_schrei_timer = random(12000,15000);
        } else furcherregender_schrei_timer -= diff;

        if (gegenschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),gegenschlag);
            gegenschlag_timer = random(15000,20000);
        } else gegenschlag_timer -= diff;

		if (heascher_rufen_stat == false && m_creature->GetHealthPercent() < 50.0f)
		{
			DoCastSpellIfCan(m_creature,haescher_rufen);
			heascher_rufen_stat = true;
		}

        if (toedliches_spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),toedliches_spalten);
            toedliches_spalten_timer = random(14000,18000);
        } else toedliches_spalten_timer -= diff;

        if (wutanfall_stat == false && m_creature->GetHealthPercent() < 10.0f)
        {
            DoCastSpellIfCan(m_creature,wutanfall);
			wutanfall_stat = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hauptmann_krombruch(Creature *_Creature)
{
    return new boss_hauptmann_krombruchAI (_Creature);
}

#define Spell_Ruestung_Zerreissen		16145
#define Spell_Spalten					15284
#define Spell_Toedlicher_Stoss			13737

struct MANGOS_DLL_DECL npc_Haescher_der_GordokAI : public ScriptedAI
{
    npc_Haescher_der_GordokAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Ruestung_Zerreissen_Timer;
	uint32 Spalten_Timer;
	uint32 Toedlicher_Stoss_Timer;

    void Reset()
    {
		Ruestung_Zerreissen_Timer		= 2000;
		Spalten_Timer					= 2000;
		Toedlicher_Stoss_Timer			= 2000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Ruestung_Zerreissen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Ruestung_Zerreissen);
            Ruestung_Zerreissen_Timer = random(7000,10000);
        } else Ruestung_Zerreissen_Timer -= diff;
		
        if (Spalten_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Spalten);
            Spalten_Timer = random(9000,15000);
        } else Spalten_Timer -= diff;
		
        if (Toedlicher_Stoss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Toedlicher_Stoss);
            Toedlicher_Stoss_Timer = random(15000,21000);
        } else Toedlicher_Stoss_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_Haescher_der_Gordok(Creature *_Creature)
{
    return new npc_Haescher_der_GordokAI (_Creature);
}

void AddSC_boss_hauptmann_krombruch()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hauptmann_krombruch";
    newscript->GetAI = &GetAI_boss_hauptmann_krombruch;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name="npc_Haescher_der_Gordok";
    newscript->GetAI = &GetAI_npc_Haescher_der_Gordok;
    newscript->RegisterSelf();
}

