#pragma once

#include "dire_maul.h"

#define arkane_explosion		13745
#define blitzschlag				15234
#define feuerball				17290
#define frostnova				15531
#define gedankenschlag			17194
#define heilen					22883
#define kampfrausch				16170
#define kettenblitzschlag		15305
#define machtwort_schild		17139
#define psychischer_schrei		22884
#define totem_des_erdengriffs 	8376
#define welle_der_heilung 		15982

struct MANGOS_DLL_DECL boss_cho_rush_der_beobachterAI : public ScriptedAI
{
    boss_cho_rush_der_beobachterAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		mIsPulledOutside = false;
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
		mAttacker = nullptr;
	}

	ScriptedInstance* pInstance;

    Unit* target;
	Unit* mAttacker;

    uint32 arkane_explosion_timer;
    uint32 blitzschlag_timer;
    uint32 feuerball_timer;
    uint32 frostnova_timer;
    uint32 gedankenschlag_timer;
    uint32 heilen_timer;
    bool kampfrausch_stat;
	bool mIsPulledOutside;
	bool mCombatEnd;
    uint32 kettenblitzschlag_timer;
    uint32 machtwort_schild_timer;
    uint32 psychischer_schrei_timer;
    uint32 totem_des_erdengriffs_timer;
    uint32 welle_der_heilung_timer;

    void Reset()
    {
        arkane_explosion_timer 		= 6000;
        blitzschlag_timer 			= 9000;
        feuerball_timer 			= 7000;
        frostnova_timer 			= 10000;
        gedankenschlag_timer 		= 14000;
        heilen_timer 				= 19000;
        kampfrausch_stat 			= false;
        kettenblitzschlag_timer 	= 14000;
        machtwort_schild_timer 		= 18000;
        psychischer_schrei_timer	= 25000;
        totem_des_erdengriffs_timer = 25000;
        welle_der_heilung_timer 	= 25000;
    }

    void Aggro(Unit *who)
    {
		mAttacker = who;
		Reset();
    }

	void JustReachedHome() {
		mAttacker = nullptr;
		mIsPulledOutside = false;
		Reset();
	}

	void handleGordokDied() {
		mCombatEnd = true;
		
		debug_log("GORDOK_DIED\n");
	}

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
		if(mCombatEnd == true || !m_creature->getVictim() && !m_creature->SelectHostileTarget())
			return;

        if (arkane_explosion_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), arkane_explosion);
            arkane_explosion_timer = random(20000,25000);
        } else arkane_explosion_timer -= diff;

        if (blitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), blitzschlag);
            blitzschlag_timer = random(15000,20000);
        } else blitzschlag_timer -= diff;

        if (feuerball_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), feuerball);
            feuerball_timer = random(5000,10000);
        } else feuerball_timer -= diff;

        if (frostnova_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), frostnova);
            frostnova_timer = random(20000,25000);
        } else frostnova_timer -= diff;

        if (gedankenschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), gedankenschlag);
            gedankenschlag_timer = random(8000,15000);
        } else gedankenschlag_timer -= diff;

        if (heilen_timer < diff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;
            DoCastSpellIfCan(pUnit, heilen);
            heilen_timer = random(8000,14000);
        } else heilen_timer -= diff;

        if (kampfrausch_stat == false && m_creature->GetHealthPercent() < 10.0f)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;
			DoCastSpellIfCan(pUnit, kampfrausch);
            DoCastSpellIfCan(m_creature, kampfrausch);
            kampfrausch_stat = true;
        }

        if (kettenblitzschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), kettenblitzschlag);
            kettenblitzschlag_timer = random(14000,19000);
        } else kettenblitzschlag_timer -= diff;

        if (machtwort_schild_timer < diff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;
            DoCastSpellIfCan(pUnit, machtwort_schild);
            machtwort_schild_timer = random(17000,23000);
        } else machtwort_schild_timer -= diff;

        if (psychischer_schrei_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), psychischer_schrei);
            psychischer_schrei_timer = random(25000,35000);
        } else psychischer_schrei_timer -= diff;
		
        if (totem_des_erdengriffs_timer < diff)
        {
            DoCastSpellIfCan(m_creature, totem_des_erdengriffs);
            totem_des_erdengriffs_timer = random(16000,23000);
        } else totem_des_erdengriffs_timer -= diff;
		
        if (welle_der_heilung_timer < diff)
        {
            Unit* pUnit = DoSelectLowestHpFriendly(60.0f, 1);
            if (!pUnit)
                return;
            DoCastSpellIfCan(pUnit, welle_der_heilung);
            welle_der_heilung_timer = random(14000,20000);
        } else welle_der_heilung_timer -= diff;

        DoMeleeAttackIfReady();
    }
};