/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


//////////////////////////////////////////////////////////////////////////
// ** boss_mr_smite.cpp
//  Handles AI of boss Mr. Smite in the deadmines.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Header files
#include "precompiled.h"
#include "boss_mr_smite.h"
#include "deadmines.h"


//////////////////////////////////////////////////////////////////////////
// Local Constants
enum
{
	SAY_PHASE_2                     = -1036002,
	SAY_PHASE_3                     = -1036003,

	EQUIP_ID_SWORD					= 2179,                 // default equipment, not used in code
	EQUIP_ID_AXE                    = 2183,
	EQUIP_ID_HAMMER                 = 10756,

	SPELL_NIBLE_REFLEXES            = 6433,                 // removed after phase 1
	SPELL_SMITE_SLAM                = 6435,                 // only casted in phase 3
	SPELL_SMITE_STOMP               = 6432,
	SPELL_SMITE_HAMMER              = 6436,                 // unclear, not casted in sniff
	SPELL_THRASH                    = 12787,                // only casted in phase 2; unclear, 3391 directly casted in sniff
	
	SPELL_SMITE_SLAM_COOLDOWN		= 11000,

	PHASE_1                         = 1,
	PHASE_2                         = 2,
	PHASE_3                         = 3,
	PHASE_EQUIP_NULL                = 4,
	PHASE_EQUIP_START               = 5,
	PHASE_EQUIP_PROCESS             = 6,
	PHASE_EQUIP_END                 = 7,
};


//////////////////////////////////////////////////////////////////////////
// Implementation: struct boss_mr_smiteAI
boss_mr_smiteAI::boss_mr_smiteAI(Creature* pCreature)
	: ScriptedAI(pCreature) 
{
	Reset();
}

boss_mr_smiteAI::~boss_mr_smiteAI()
{
}

void boss_mr_smiteAI::Reset()
{
	// Reset phase variable and timers
    m_uiPhase = PHASE_1;
    m_uiEquipTimer = 0;
    m_uiSlamTimer = 9000;

	// What does this do? ...
    DoCastSpellIfCan(m_creature, SPELL_NIBLE_REFLEXES, CAST_TRIGGERED);

    // It's easier for me to just equip him with his item than to look
	// at the database entries...
    SetEquipmentSlots(false, EQUIP_ID_SWORD, 0, 0);
}

void boss_mr_smiteAI::AttackedBy(Unit* pAttacker)
{
	// Error: no target
    if (m_creature->getVictim())
        return;

	// Boss is dead...
    if (m_uiPhase > PHASE_3)
        return;

	// Start attacking this unit
    AttackStart(pAttacker);
}

void boss_mr_smiteAI::AttackStart(Unit* pWho)
{
	// The boss is dead...
    if (m_uiPhase > PHASE_3)
        return;

	// Start auto attack
    if (m_creature->Attack(pWho, true))
    {
		// Successfully started auto attack, now add initial threat and set combat states
        m_creature->AddThreat(pWho);
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);

		// Chase the attacker!
        m_creature->GetMotionMaster()->MoveChase(pWho);
    }
}

void boss_mr_smiteAI::MovementInform(uint32 uiMotionType, uint32 /*uiPointId*/)
{
	// Nothing else than waypoint movement is handled here
    if (uiMotionType != POINT_MOTION_TYPE)
        return;

	// Sheath weapon and kneel down
    m_creature->SetSheath(SHEATH_STATE_UNARMED);
    m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);

	// Equipment process will take 3 seconds, enter equipment phase
    m_uiEquipTimer = 3000;
    m_uiPhase = PHASE_EQUIP_PROCESS;
}

void boss_mr_smiteAI::PhaseEquipStart()
{
	// Try to get scripted instance data
    ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
    if (!pInstance)
        return;

	// Get chest object if available
    GameObject* pChest = pInstance->GetSingleGameObjectFromStorage(GO_SMITE_CHEST);
    if (!pChest)
        return;

	// Enter equipment phase (0: Walk to chest object)
    m_uiPhase = PHASE_EQUIP_NULL;

	// Get contact point of chest object
    float fX, fY, fZ;
    pChest->GetContactPoint(m_creature, fX, fY, fZ, CONTACT_DISTANCE);

	// Erase all previous movements and walk towards the clostest point available near 
	// that chest object
    m_creature->GetMotionMaster()->Clear();
    m_creature->SetFacingToObject(pChest);
    m_creature->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
}

void boss_mr_smiteAI::PhaseEquipProcess()
{
	// Last phase: Equip hammer!
    if (m_creature->GetHealthPercent() < 33.0f)
    {
		// This is a little hack: We need to unsheath the weapons and immediatly send a forced update
		// because if not, the sheathed 1h weapons (axes) will remain there and mr. smite will
		// wear his hammer AND the axes!
		m_creature->SetSheath(SHEATH_STATE_MELEE);
		m_creature->SendForcedObjectUpdate();

		// Now equip the hammer
		SetEquipmentSlots(false, EQUIP_ID_HAMMER, EQUIP_UNEQUIP);

		// This is a little enrage effect and increases Mr. Smites attack speed by 50%!
        DoCastSpellIfCan(m_creature, SPELL_SMITE_HAMMER);
    }
    else
	{
		// Has to be the first stun, so use double axe
        SetEquipmentSlots(false, EQUIP_ID_AXE, EQUIP_ID_AXE);
	}
	
	// Now that he is properly equipped, stand up!
    m_creature->SetStandState(UNIT_STAND_STATE_STAND);

	// Continue to the last phase
    m_uiPhase = PHASE_EQUIP_END;
    m_uiEquipTimer = 1000;
}

void boss_mr_smiteAI::PhaseEquipEnd()
{
    // We don't have a victim, so select from threat list
    Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0);
    if (!pVictim)
    {
        EnterEvadeMode();
        return;
    }

	// Unsheath weapon
    m_creature->SetSheath(SHEATH_STATE_MELEE);

	// Continue with combat phase based on Mr. Smites health
    m_uiPhase = m_creature->GetHealthPercent() < 33.0f ? PHASE_3 : PHASE_2;

    if (m_uiPhase == PHASE_2)
	{
		// This will enable double-strike attacks for Mr. Smite with his dual axes
        DoCastSpellIfCan(m_creature, SPELL_THRASH, CAST_TRIGGERED);
		//m_creature->AttackerStateUpdate(m_creature->getVictim(), BASE_ATTACK, true);
	}

	// Attack that poor little player
    AttackStart(pVictim);
}

void boss_mr_smiteAI::UpdateAI(const uint32 uiDiff)
{
	// Equipment phase (Somehow hacky I think, since this is determined by his target...)
    if (!m_creature->SelectHostileTarget() ||
		!m_creature->getVictim())
    {
		// decrease the cooldown in between equipment change phases
        if (m_uiEquipTimer)
        {
            if (m_uiEquipTimer > uiDiff)
            {
                m_uiEquipTimer -= uiDiff;
                return;		// STOP here since the timer has not yet expired
            }
            else
                m_uiEquipTimer = 0;
        }

		// Choose which phase needs to be updated
        switch (m_uiPhase)
        {
            case PHASE_EQUIP_START:
                PhaseEquipStart();
                break;

            case PHASE_EQUIP_PROCESS:
                PhaseEquipProcess();
                break;

            case PHASE_EQUIP_END:
                PhaseEquipEnd();
                break;
        }

        return;
    }

    // Normal combat phases
    switch (m_uiPhase)
    {
    case PHASE_1:
		{
			// On 66% life or below, go on to equipment phase (Double Axe)
			if (m_creature->GetHealthPercent() < 66.0f)
			{
				// This will stun all players around!
				if (DoCastSpellIfCan(m_creature, SPELL_SMITE_STOMP) == CAST_OK)
				{
					// Show Mr. Smite texts (This code allows skipping the double axe if Mr. Smite's health 
					// sinks too quickly - was it like that in classic?)
					DoScriptText(m_creature->GetHealthPercent() < 33.0f ? SAY_PHASE_3 : SAY_PHASE_2, m_creature);
					
					// Go on with equip phase (2.5 Seconds duration)
					m_uiPhase = PHASE_EQUIP_START;
					m_uiEquipTimer = 2500;

					// We need to clear victim and stop combat
					m_creature->AttackStop(true);
					m_creature->RemoveAurasDueToSpell(SPELL_NIBLE_REFLEXES);
				}

				return;
			}
			break;
		}

    case PHASE_2:
		{
			// On 33% life or below, go on to equipment phase again!
			
			if (m_creature->GetHealthPercent() < 33.0f)
			{
				// This will stun all players around!
				if (DoCastSpellIfCan(m_creature, SPELL_SMITE_STOMP) == CAST_OK)
				{
					// Let him yell his text
					DoScriptText(SAY_PHASE_3, m_creature);
					
					// Go on with equip phase (2.5 Seconds duration)
					m_uiPhase = PHASE_EQUIP_START;
					m_uiEquipTimer = 2500;

					// We need to clear victim and stop combat
					m_creature->AttackStop(true);
					m_creature->RemoveAurasDueToSpell(SPELL_THRASH);
				}

				return;
			}
			break;
		}

	case PHASE_3:
		{
			// Use "Slam" every 11 Seconds
			if (m_uiSlamTimer < uiDiff)
			{
				if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SMITE_SLAM) == CAST_OK)
					m_uiSlamTimer = SPELL_SMITE_SLAM_COOLDOWN;
			}
			else
			{
				m_uiSlamTimer -= uiDiff;
			}

			break;
		}
    }

	// Do melee attack on cooldown (Not reached during equipment phases)
    DoMeleeAttackIfReady();
}


//////////////////////////////////////////////////////////////////////////
// Global functions: Script registration
static CreatureAI* GetAI_boss_mr_smite(Creature* pCreature)
{
    return new boss_mr_smiteAI(pCreature);
}

void AddSC_boss_mr_smite()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_mr_smite";
    pNewScript->GetAI = &GetAI_boss_mr_smite;
    pNewScript->RegisterSelf();
}

//////////////////////////////////////////////////////////////////////////
