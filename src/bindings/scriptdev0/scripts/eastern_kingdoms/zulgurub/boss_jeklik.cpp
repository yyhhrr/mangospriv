/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
 *
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
SDName: Boss_Jeklik
SD%Complete: 95
SDComment: Evade case is looking weird (flying troll). Change her to bat for the time she's flying. Get the right bomb throwing bats ID.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"
#include "../system/ProgressMonitor.h"

enum
{
    SAY_AGGRO                   = -1309002,
    SAY_RAIN_FIRE               = -1309003,
    SAY_DEATH                   = -1309004,

    SPELL_SCREECH               = 6605,
    SPELL_PIERCE_ARMOR          = 12097,
    SPELL_CHANNELING_VISUAL     = 13540,
    SPELL_CURSE_OF_BLOOD        = 16098,
    SPELL_BLOOD_LEECH           = 22644,
    SPELL_PSYCHIC_SCREAM        = 22884,
    SPELL_CHARGE                = 22911,
    SPELL_SONICBURST            = 23918,
    SPELL_SWOOP                 = 23919,
    SPELL_SHADOW_WORD_PAIN      = 23952,
    SPELL_MIND_FLAY             = 23953,
    SPELL_GREATERHEAL           = 23954,
    SPELL_BAT_FORM              = 23966,
    SPELL_CHAIN_MIND_FLAY       = 26044,                    // Right ID unknown. So disabled

    // Batriders Spell
    SPELL_DEMORALIZING_SHOUT	= 23511,
    SPELL_BATTLE_COMMAND		= 5115,
    SPELL_INFECTED_BITE			= 16128,
    SPELL_THRASH				= 3391,
    SPELL_UNSTABLE_CONCOTION	= 24024,
    SPELL_THROW_LIQUID_FIRE		= 23968,

    SPELL_BOMB                  = 23970,
	SPELL_SUMMON_LIQUID_FIRE	= 23971,
	SPELL_BOMB_FIRE_DAMAGE		= 23972,
};

struct MANGOS_DLL_DECL boss_jeklikAI : public ScriptedAI
{
    boss_jeklikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_creature->CastSpell(m_creature, SPELL_CHANNELING_VISUAL, false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiChargeTimer;
    uint32 m_uiSonicBurstTimer;
    uint32 m_uiScreechTimer;
    uint32 m_uiSpawnBatsTimer;
    uint32 m_uiShadowWordPainTimer;
    uint32 m_uiMindFlayTimer;
    uint32 m_uiChainMindFlayTimer;
    uint32 m_uiGreaterHealTimer;
    uint32 m_uiSpawnFlyingBatsTimer;
    uint32 m_uiPsychicScreamTimer;
    uint32 m_uiBloodLeechTimer;
    uint32 m_uiPierceArmorTimer;
    uint32 m_uiSwoopTimer;
    uint32 m_uiCurseOfBloodTimer;
    uint32 m_uiFlyDownTimer;

    bool m_bIsPhaseOne;
	bool m_flightPhase;

    void Reset()
    {
        m_uiChargeTimer = 20000;
        m_uiSonicBurstTimer = urand(25000, 40000);
        m_uiScreechTimer = 13000;
        m_uiSpawnBatsTimer = 60000;
        m_uiShadowWordPainTimer = 10000;
        m_uiMindFlayTimer = urand(45000,50000);
        m_uiChainMindFlayTimer = 26000;
        m_uiGreaterHealTimer = urand(20000,25000);
        m_uiSpawnFlyingBatsTimer = 10000;
        m_uiPsychicScreamTimer = urand(45000,50000);
        m_uiBloodLeechTimer = urand(25000,30000);
        m_uiPierceArmorTimer = 35000;
        m_uiSwoopTimer = urand(15000,20000);
        m_uiCurseOfBloodTimer = urand(10000,15000);
		m_uiFlyDownTimer = 10000;

        m_bIsPhaseOne = m_flightPhase = true;
    }
	
	bool PreEnterEvadeMode() override
	{
		m_creature->RemoveAllAuras();

		m_creature->DeleteThreatList();
		m_creature->CombatStop(true);
		m_creature->LoadCreatureAddon(false);
		m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_FLYING);
		m_creature->SetLootRecipient(NULL);
		m_creature->EvadeStart();

		if (m_creature->GetMotionMaster())
			m_creature->GetMotionMaster()->MovePoint(1338, -12291.90f, -1380.07f, 144.90f, false);

		m_creature->SetDisplayId(15191);

		return false;
	}

	void MovementInform(uint32 type, uint32 id) override
	{
		if (type != POINT_MOTION_TYPE)
			return;

		if(id == 1338) //bottom -> top
			JustReachedHome();
		else if(id == 1337) //top -> bottom
		{
			m_creature->SetCanChase(true);

			if (!m_creature->SelectHostileTarget())
				return;

			if (auto* target = m_creature->getVictim())
				m_creature->GetMotionMaster()->MoveChase(target);

			m_flightPhase = false;
			m_creature->EvadeStop();
		}
	}

    void EnterCombat(Unit* /*pWho*/)
    {
		m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_FLYING);
		DoScriptText(SAY_AGGRO, m_creature);
		m_creature->SetDisplayId(15191);
        m_creature->RemoveAurasDueToSpell(SPELL_CHANNELING_VISUAL);
		m_creature->GetMotionMaster()->MovePoint(1337, -12278.32f, -1401.88f, 130.70f, false);
		m_creature->SetCanChase(false);
		m_creature->EvadeStart();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_JEKLIK, IN_PROGRESS);
    }

    void JustReachedHome()
	{
		m_creature->EvadeStop();
		m_creature->DeMorph();
		m_creature->RemoveAurasDueToSpell(SPELL_BAT_FORM);
		m_creature->m_movementInfo.RemoveMovementFlag(MOVEFLAG_FLYING);
		m_creature->CastSpell(m_creature, SPELL_CHANNELING_VISUAL, false);
		Reset();

		if (m_pInstance)
			m_pInstance->SetData(TYPE_JEKLIK, FAIL);
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
			m_pInstance->SetData(TYPE_JEKLIK, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::ZulGurub, Bosses::Jeklik);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_flightPhase)
			return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsPhaseOne)
        {
            // Phase Switch at 50%
            if (m_creature->GetHealthPercent() <= 50.0f)
            {
				DoScriptText(SAY_RAIN_FIRE, m_creature);
                m_creature->RemoveAurasDueToSpell(SPELL_BAT_FORM);
				m_creature->DeMorph();
                m_creature->m_movementInfo.RemoveMovementFlag(MOVEFLAG_FLYING);
                DoResetThreat();
                m_bIsPhaseOne = false;
                return;
            }

            if (m_uiChargeTimer < uiDiff)
            {
                Unit* pTarget = m_creature->SelectRandomUnfriendlyTarget(nullptr, 30.f);
                DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_CHARGE);

                m_uiSonicBurstTimer = 1000;
                m_uiChargeTimer = urand(15000, 30000);      // 10000, 20000
            }
            else
                m_uiChargeTimer -= uiDiff;

            if (m_uiSonicBurstTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SONICBURST) == CAST_OK)
                    m_uiSonicBurstTimer = urand(7000, 12000);
            }
            else
                m_uiSonicBurstTimer -= uiDiff;

            if (m_uiScreechTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SCREECH) == CAST_OK)
                    m_uiScreechTimer = urand(18000, 26000);
            }
            else
                m_uiScreechTimer -= uiDiff;

            if (m_uiSpawnBatsTimer < uiDiff)
            {
                // TODO There are some bats in the cave behind the boss, perhaps they should be called
                float fX, fY, fZ, fO, fNewX, fNewY, fNewZ;
                m_creature->GetRespawnCoord(fX, fY, fZ, &fO);
				Unit* pTarget = m_creature->SelectRandomUnfriendlyTarget(nullptr, 30.f);
                for (uint8 i = 0; i < 6; ++i)
                {
                    // Get a point a little bit behind Jeklik respawn pos
                    m_creature->GetRandomPoint(fX - 5.0f, fY + 5.0f, fZ, 5.0f, fNewX, fNewY, fNewZ);
                    if (Creature* pBat = m_creature->SummonCreature(NPC_BLOODSEEKER_BAT, fNewX, fNewY, fNewZ, fO, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
					{
						pBat->m_movementInfo.AddMovementFlag(MOVEFLAG_FLYING);
						pBat->addUnitState(UNIT_STAT_IGNORE_PATHFINDING);
						pBat->AI()->AttackStart(pTarget ? pTarget : m_creature->getVictim());
					}
                }

                m_uiSpawnBatsTimer = 60000;
            }
            else
                m_uiSpawnBatsTimer -= uiDiff;

            if (m_uiBloodLeechTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BLOOD_LEECH) == CAST_OK)
                    m_uiBloodLeechTimer = urand(10000, 20000);
            }
            else
                m_uiBloodLeechTimer -= uiDiff;

            if (m_uiSwoopTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SWOOP) == CAST_OK)
                    m_uiSwoopTimer = urand(8000, 13000);
            }
            else
                m_uiSwoopTimer -= uiDiff;

            if (m_uiPierceArmorTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PIERCE_ARMOR) == CAST_OK)
                    m_uiPierceArmorTimer = urand(25000, 40000);
            }
            else
                m_uiPierceArmorTimer -= uiDiff;
        }
        else                                                // Phase Two
        {
            if (m_uiShadowWordPainTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_WORD_PAIN) == CAST_OK)
                        m_uiShadowWordPainTimer = urand(13000, 25000);
                }
            }
            else
                m_uiShadowWordPainTimer -= uiDiff;

            if (m_uiMindFlayTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIND_FLAY) == CAST_OK)
                    m_uiMindFlayTimer = urand(15000, 20000);
            }
            else
                m_uiMindFlayTimer -= uiDiff;

            if (m_uiChainMindFlayTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAIN_MIND_FLAY) == CAST_OK)
                    m_uiChainMindFlayTimer = urand(25000, 45000);
            }
            else
                m_uiChainMindFlayTimer -= uiDiff;

            if (m_uiGreaterHealTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_GREATERHEAL, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                    m_uiGreaterHealTimer = urand(20000, 25000);
            }
            else
                m_uiGreaterHealTimer -= uiDiff;

            if (m_uiSpawnFlyingBatsTimer < uiDiff)
            {
                float fX, fY, fZ, fO, fNewX, fNewY, fNewZ;
                m_creature->GetRespawnCoord(fX, fY, fZ, &fO);
                // Get a point a little bit behind Jeklik respawn pos
                m_creature->GetRandomPoint(fX - 5.0f, fY + 5.0f, fZ, 15.0f, fNewX, fNewY, fNewZ);
				
				for(uint8 i = 0; i < 3; ++i)
					m_creature->SummonCreature(NPC_FRENZIED_BAT, fNewX, fNewY, fNewZ, fO, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
				
				m_uiSpawnFlyingBatsTimer = urand(15000, 20000);
            }
            else
                m_uiSpawnFlyingBatsTimer -= uiDiff;

            if (m_uiPsychicScreamTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_PSYCHIC_SCREAM) == CAST_OK)
                    m_uiPsychicScreamTimer = urand(30000, 45000);
            }
            else
                m_uiPsychicScreamTimer -= uiDiff;

            if (m_uiCurseOfBloodTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_CURSE_OF_BLOOD) == CAST_OK)
                        m_uiCurseOfBloodTimer = urand(10000, 15000);
                }
            }
            else
                m_uiCurseOfBloodTimer -= uiDiff;

        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jeklik(Creature* pCreature)
{
    return new boss_jeklikAI(pCreature);
}

// Flying Bat
struct MANGOS_DLL_DECL mob_batriderAI : public ScriptedAI
{
    mob_batriderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_fHeight = 150;	//m_creature->GetPositionZ();

        m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_FLYING);
        SetCombatMovement(false);

        if (m_pInstance)	// Just to avoid combat in non instance maps (who knows)
            m_creature->SetInCombatWithZone();
        else
            return;	// Nothing to do then

        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            m_creature->GetMotionMaster()->MovePoint(0, pTarget->GetPositionX(), pTarget->GetPositionY(), m_fHeight, false);

        Reset();
    }

    ScriptedInstance* m_pInstance;

	bool bInfectedBiteCasted;
	bool bJekliksEncounter;
	bool bExploded;

    uint32 m_uiBattleCommandTimer;
    uint32 m_uiInfectedBiteTimer;
    uint32 m_uiThrashTimer;
    uint32 m_uiThrowLiquidFireTimer;

    uint32 m_uiBombTimer;
    uint32 m_uiCheckTimer;
	uint32 m_uiCheckJekliksEncounterTimer;

    float m_fHeight; // We keep same height

    void Reset()
    {
		bInfectedBiteCasted = false;
        bExploded = false;
        bJekliksEncounter = false;

        m_uiBattleCommandTimer = 8000;
        m_uiInfectedBiteTimer = 6500;
        m_uiThrashTimer = 6000;
        m_uiThrowLiquidFireTimer = 2000;

        m_uiBombTimer = 15000;
        m_uiCheckTimer = 1000;
		m_uiCheckJekliksEncounterTimer = 0;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pVictim)
    {
        if (!bJekliksEncounter)
            DoCastSpellIfCan(pVictim, SPELL_DEMORALIZING_SHOUT);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        // If new health is 50 percent or lesser, cast explode
        if (!bJekliksEncounter && !bExploded && ( (uiDamage >= m_creature->GetHealth() ) || (m_creature->GetHealthPercent() <= 50.0f) ))
        {
            bExploded = true;
            uiDamage = 0;
            //DoCastSpellIfCan(m_creature, SPELL_THROW_LIQUID_FIRE__);
            DoCastSpellIfCan(m_creature, SPELL_UNSTABLE_CONCOTION, CAST_FORCE_CAST);
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
		if (pSpell->Id == SPELL_BOMB)
		{
			auto PositionX = pTarget->GetPositionX();
			auto PositionY = pTarget->GetPositionY();
			auto PositionZ = pTarget->GetPositionZ();

			pTarget->CastSpell(pTarget, SPELL_SUMMON_LIQUID_FIRE, true);
			m_creature->SummonCreature(NPC_JEKLIK_FIRE_DUMMY, PositionX, PositionY, PositionZ + 1.0f, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 18000);
		}
    }

    void MovementInform(uint32 /*uiMoveType*/, uint32 /*uiPointId*/)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
        {
            float fX, fY, fZ; // fZ not used
            m_creature->GetRandomPoint(pTarget->GetPositionX(), pTarget->GetPositionY(), 0.0f, 20.0f, fX, fY, fZ);
            m_creature->GetMotionMaster()->MovePoint(0, fX, fY, m_fHeight, false);
        }
    }

    void UpdateAI (const uint32 uiDiff)
    {
        // If we are NOT_SELECTABLE, it's point that we are summoned by Jeklik
        if (m_uiCheckJekliksEncounterTimer <= uiDiff)
        {
            bJekliksEncounter = m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE) ? true : false;
            m_uiCheckJekliksEncounterTimer = 1000;
        }
        else
            m_uiCheckJekliksEncounterTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!bJekliksEncounter)
        {
            // Battle Command
            if (m_uiBattleCommandTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_BATTLE_COMMAND);
                m_uiBattleCommandTimer = 25000;
            }
            else
                m_uiBattleCommandTimer -= uiDiff;

            // Infected Bite
            if (!bInfectedBiteCasted && m_uiInfectedBiteTimer <= uiDiff)
            {
                bInfectedBiteCasted = true;
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_INFECTED_BITE);
            }
            else
                m_uiInfectedBiteTimer -= uiDiff;

			// Thrash
			if (m_uiThrashTimer <= uiDiff)
			{
				Unit* pTarget = m_creature->getVictim();
				if (pTarget && pTarget->isAlive())
					m_creature->AttackerStateUpdate(pTarget, BASE_ATTACK, true);

				m_uiThrashTimer = urand(4000, 6000);
			}
			else
				m_uiThrashTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        else
        {
            // Throw Liquid Fire
            if (m_uiThrowLiquidFireTimer <= uiDiff)
            {
				Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_BOMB);
                m_uiThrowLiquidFireTimer = urand(10000, 15000);
            }
            else
                m_uiThrowLiquidFireTimer -= uiDiff;
        }

        // Check Timer
        if (m_uiCheckTimer < uiDiff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_JEKLIK) == DONE)
                {
                    m_creature->SetDeathState(JUST_DIED);
                    m_creature->RemoveCorpse();
                    return;
                }
            }
            m_uiCheckTimer = 1000;
        }
        else
            m_uiCheckTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_batrider(Creature* pCreature)
{
    return new mob_batriderAI(pCreature);
}

struct MANGOS_DLL_DECL mob_fire_bombAI : public ScriptedAI
{
	mob_fire_bombAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}
	ScriptedInstance* m_pInstance;

	uint32 FireBomb_Timer;

	void Reset()
	{
		FireBomb_Timer = 1000;
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		SetCombatMovement(false);
	}

	void Aggro(Unit* pWho)
	{

	}

	void UpdateAI(const uint32 diff)
	{
		if (FireBomb_Timer <= diff)
		{
			if (DoCastSpellIfCan(m_creature, SPELL_BOMB_FIRE_DAMAGE) == CAST_OK)
				FireBomb_Timer = 1500;
		}
		else
			FireBomb_Timer -= diff;
	}
};

CreatureAI* GetAI_mob_fire_bomb(Creature* pCreature)
{
	return new mob_fire_bombAI(pCreature);
}

void AddSC_boss_jeklik()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_jeklik";
    pNewScript->GetAI = &GetAI_boss_jeklik;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_batrider";
    pNewScript->GetAI = &GetAI_mob_batrider;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_fire_bomb";
	pNewScript->GetAI = &GetAI_mob_fire_bomb;
	pNewScript->RegisterSelf();
}
