/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: ZulGurub
SD%Complete: 100
SDComment:
SDCategory: ZulGurub
EndScriptData */

/* ContentData
at_altar_of_the_blood_god
mob_gurubashi_axe_thrower
mob_gurubashi_blood_drinker
EndContentData */

#include "precompiled.h"
#include "zulgurub.h"

/*######
## at_zulgurub
######*/

bool AreaTrigger_at_zulgurub(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isGameMaster() || pPlayer->isDead())
        return false;

    instance_zulgurub* m_pInstance = (instance_zulgurub*)pPlayer->GetInstanceData();

    if (!m_pInstance || m_pInstance->GetData(TYPE_HAKKAR) == DONE)
        return false;

    m_pInstance->HakkarYell(pAt->id);
    return true;
}

/*######
## mob_gurubashi_axe_thrower
######*/

enum eAxeThrower
{
    SPELL_THROW               = 22887,
    SPELL_AXE_FLURRY          = 24018,
    SPELL_ENRAGE              = 8269,

    SAY_ENRAGE                = -1000003
};

struct MANGOS_DLL_DECL mob_gurubashi_axe_throwerAI : public ScriptedAI
{
    mob_gurubashi_axe_throwerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiThrowTimer;
    uint32 m_uiAxeFlurryTimer;
    bool bEnraged;
    
    void Reset()
    {
        m_uiThrowTimer = 0;
        m_uiAxeFlurryTimer = 5000;
        bEnraged = false;
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Enrage
        if (!bEnraged && m_creature->GetMaxHealth() <= 30)
        {
            bEnraged = true;
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            DoScriptText(SAY_ENRAGE, m_creature);
        }

        /*if (m_creature->getVictim()->IsWithinDist(m_creature, ATTACK_DISTANCE))
        {
            if (m_creature->GetSheath() != SHEATH_STATE_MELEE)
                m_creature->SetSheath(SHEATH_STATE_MELEE);*/

            // Axe Flurry
            if (m_uiAxeFlurryTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_AXE_FLURRY);
                m_uiAxeFlurryTimer = urand(12000, 18000);
            }
            else
                m_uiAxeFlurryTimer -= uiDiff;
        /*}
        else
        {
            m_creature->StopMoving();
            if (m_creature->GetSheath() != SHEATH_STATE_RANGED)
                m_creature->SetSheath(SHEATH_STATE_RANGED);

            // Throw
            if (m_uiThrowTimer <= uiDiff)
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_THROW);
                m_uiThrowTimer = urand(1500, 3000);
            }
            else
                m_uiThrowTimer -= uiDiff;
        }*/
            
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_gurubashi_axe_thrower(Creature* pCreature)
{
    return new mob_gurubashi_axe_throwerAI(pCreature);
}

/*######
## mob_gurubashi_blood_drinker
######*/

enum eBloodDrinker
{
    SPELL_BLOOD_LEECH = 24437,
    SPELL_DRAIN_LIFE  = 24435
};

struct MANGOS_DLL_DECL mob_gurubashi_blood_drinkerAI : public ScriptedAI
{
    mob_gurubashi_blood_drinkerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiBloodLeechTimer;
    uint32 m_uiDrainLifeTimer;

    void Reset()
    {
        m_uiBloodLeechTimer = 0;
        m_uiDrainLifeTimer = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Blood Leech
        if (m_uiBloodLeechTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLOOD_LEECH);
            m_uiBloodLeechTimer = 5000;
        }
        else
            m_uiBloodLeechTimer -= uiDiff;

        // Drain Life
        if (m_uiDrainLifeTimer <= uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectRandomUnfriendlyTarget(0, 20.0f))
                DoCastSpellIfCan(pTarget, SPELL_DRAIN_LIFE);
            m_uiDrainLifeTimer = 8000;
        }
        else
            m_uiDrainLifeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_gurubashi_blood_drinker(Creature* pCreature)
{
    return new mob_gurubashi_blood_drinkerAI(pCreature);
}

bool GOUse_go_kohlepfanne_des_wahnsinns(Player* plr, GameObject* pGo)
{
	if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
	{
		if (pInstance->GetData(TYPE_RND_BOSS) != NOT_STARTED)
			return true;

		// Muss auf DONE gesetzt werden weil es sonst immer wieder möglich ist, den Boss zu spawnen.
		auto rnd = urand(1, 100);
		if (rnd <= 25)
		{
			pInstance->SetData(TYPE_RND_BOSS, DONE);
			plr->SummonCreature(NPC_HAZZARAH, -11893.914063f, -1904.550415f, 65.147491f, 0.839414f, TEMPSUMMON_MANUAL_DESPAWN, 0);
		}
		else if (rnd > 25 && rnd <= 50)
		{
			pInstance->SetData(TYPE_RND_BOSS, DONE);
			plr->SummonCreature(NPC_RENATAKI, -11893.914063f, -1904.550415f, 65.147491f, 0.839414f, TEMPSUMMON_MANUAL_DESPAWN, 0);
		}
		else if (rnd > 50 && rnd <= 75)
		{
			pInstance->SetData(TYPE_RND_BOSS, DONE);
			plr->SummonCreature(NPC_WUSHOOLAY, -11893.914063f, -1904.550415f, 65.147491f, 0.839414f, TEMPSUMMON_MANUAL_DESPAWN, 0);
		}
		else if (rnd > 75 && rnd <= 100)
		{
			pInstance->SetData(TYPE_RND_BOSS, DONE);
			plr->SummonCreature(NPC_GRILEK, -11893.914063f, -1904.550415f, 65.147491f, 0.839414f, TEMPSUMMON_MANUAL_DESPAWN, 0);
		}
	}

	return false;
}

/*######
## AddSC
######*/

void AddSC_zulgurub()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "at_zulgurub";
    pNewScript->pAreaTrigger = &AreaTrigger_at_zulgurub;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_gurubashi_blood_drinker";
    pNewScript->GetAI = GetAI_mob_gurubashi_blood_drinker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_gurubashi_axe_thrower";
    pNewScript->GetAI = GetAI_mob_gurubashi_axe_thrower;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "go_kohlepfanne_des_wahnsinns";
	pNewScript->pGOUse = &GOUse_go_kohlepfanne_des_wahnsinns;
	pNewScript->RegisterSelf();
}
