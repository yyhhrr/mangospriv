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

//.npc add 14327

#include "precompiled.h"
#include "dire_maul.h"

#define dornenfluch			16247
#define feuerbrand			12742
#define fluch_der_sprachen	13338
#define leereblitz			22709
#define schattenblitzsalve	17228
#define vergroesserung		22710

struct MANGOS_DLL_DECL boss_lethtendrisAI : public ScriptedAI
{
    boss_lethtendrisAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;
    Unit* target;

    uint32 dornenfluch_timer;
    uint32 feuerbrand_timer;
    uint32 fluch_der_sprachen_timer;
    uint32 leereblitz_timer;
    uint32 schattenblitzsalve_timer;
	uint32 vergroesserung_timer;

    void Reset()
    {
        dornenfluch_timer 			= 3000;
        feuerbrand_timer 			= 6000;
        fluch_der_sprachen_timer 	= 10000;
        leereblitz_timer 			= 4000;
        schattenblitzsalve_timer 	= 9000;
		vergroesserung_timer 		= 20000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (dornenfluch_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),dornenfluch);
            dornenfluch_timer = random(18000,24000);
        } else dornenfluch_timer -= diff;

        if (feuerbrand_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),feuerbrand);
            feuerbrand_timer = random(17000,22000);
        } else feuerbrand_timer -= diff;

        if (fluch_der_sprachen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),fluch_der_sprachen);
            fluch_der_sprachen_timer = random(15000,17000);
        } else fluch_der_sprachen_timer -= diff;

        if (leereblitz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),leereblitz);
            leereblitz_timer = random(7000,10000);
        } else leereblitz_timer -= diff;

        if (schattenblitzsalve_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitzsalve);
            schattenblitzsalve_timer = random(12000,16000);
        } else schattenblitzsalve_timer -= diff;

		if (vergroesserung_timer < diff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(30.0f, 1);
            if (!pUnit)
                return;
            DoCastSpellIfCan(pUnit, vergroesserung);
            vergroesserung_timer = random(17000,24000);
        } else vergroesserung_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lethtendris(Creature *_Creature)
{
    return new boss_lethtendrisAI (_Creature);
}

#define Spell_Druckwelle		16046
#define Spell_Feuerball			15228
#define Spell_Flammenpuffer		22713

struct MANGOS_DLL_DECL npc_PimgibAI : public ScriptedAI
{
    npc_PimgibAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

	uint32 Druckwelle_Timer;
	uint32 Feuerball_Timer;
	uint32 Flammenpuffer_Timer;

    void Reset()
    {
		Druckwelle_Timer		= 8000;
		Feuerball_Timer			= 6000;
		Flammenpuffer_Timer		= 4000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
        if (Druckwelle_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Druckwelle);
            Druckwelle_Timer = random(8000,10000);
        } else Druckwelle_Timer -= diff;
		
        if (Feuerball_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Feuerball);
            Feuerball_Timer = random(5000,7000);
        } else Feuerball_Timer -= diff;
		
        if (Flammenpuffer_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Flammenpuffer);
            Flammenpuffer_Timer = random(4000,6000);
        } else Flammenpuffer_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_Pimgib(Creature *_Creature)
{
    return new npc_PimgibAI (_Creature);
}

void AddSC_boss_lethtendris()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lethtendris";
    newscript->GetAI = &GetAI_boss_lethtendris;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name="npc_Pimgib";
    newscript->GetAI = &GetAI_npc_Pimgib;
    newscript->RegisterSelf();
}