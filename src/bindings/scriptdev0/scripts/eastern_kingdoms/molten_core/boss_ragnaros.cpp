/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Ragnaros
SD%Complete: 75
SDComment: Intro Dialog and event NYI
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "TemporarySummon.h"

#include "boss_ragnaros.h"
#include "../system/ProgressMonitor.h"


#define ADD_1X 848.740356f
#define ADD_1Y -816.103455f
#define ADD_1Z -229.74327f
#define ADD_1O 2.615287f

#define ADD_2X 852.560791f
#define ADD_2Y -849.861511f
#define ADD_2Z -228.560974f
#define ADD_2O 2.836073f

#define ADD_3X 808.710632f
#define ADD_3Y -852.845764f
#define ADD_3Z -227.914963f
#define ADD_3O 0.964207f

#define ADD_4X 786.597107f
#define ADD_4Y -821.132874f
#define ADD_4Z -226.350128f
#define ADD_4O 0.949377f

#define ADD_5X 796.219116f
#define ADD_5Y -800.948059f
#define ADD_5Z -226.010361f
#define ADD_5O 0.560603f

#define ADD_6X 821.602539f
#define ADD_6Y -782.744109f
#define ADD_6Z -226.023575f
#define ADD_6O 6.157440f

#define ADD_7X 844.924744f
#define ADD_7Y -769.453735f
#define ADD_7Z -225.521698f
#define ADD_7O 4.4539958f

#define ADD_8X 839.823364f
#define ADD_8Y -810.869385f
#define ADD_8Z -229.683182f
#define ADD_8O 4.693108f

enum eRanaros
{
    SAY_SUMMON_FIRST                = -1409013,
    SAY_SUMMON                      = -1409014,
    SAY_MIGHT                       = -1409015,
    SAY_WRATH                       = -1409016,
    SAY_KILL                        = -1409017,
    SAY_MAGMA_BLAST                 = -1409018,

    SPELL_HAND_OF_RAGNAROS          = 19780,    // Fire Damage, knocking back and stun
    SPELL_ELEMENTAL_FIRE            = 20564,    // DoT 4800 dmg/8sec
    SPELL_MIGHT_OF_RAGNAROS         = 21154,    // Summons gameobject for trigger cast
    SPELL_MAGMA_BLAST               = 20565,    // Only casted then noone in melee range
    SPELL_MELT_WEAPON               = 21387,    // Dura Lost for weapons
    SPELL_WRATH_OF_RAGNAROS         = 20566,    // Melee Knockback
    SPELL_RAGNAROS_EMERGE           = 20568,    // Emerge animation
    SPELL_RAGNAROS_SUBMERGE_FADE    = 21107,	// Apply: mod_stealth
    SPELL_RAGNAROS_SUBMERGE_VISUAL  = 20567,	// Dummy effect
    SPELL_RAGNAROS_SUBMERGE_ROOT    = 23973,
    SPELL_RAGNAROS_SUBMERGE_EFFECT  = 21859,  
    SPELL_LAVA_BURST_DUMMY          = 21908,
    SPELL_SONS_OF_FLAMES_DUMMY      = 21108,	// Summon sons of flame

	NPC_SON_OF_FLAME				= 12143,
    NPC_FLAME_OF_RAGNAROS           = 13148
};

boss_ragnarosAI::boss_ragnarosAI(Creature* pCreature) : ScriptedAI(pCreature)
{
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    SetCombatMovement(false);
    m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FIRE, true);
    Reset();
}

void boss_ragnarosAI::Reset()
{
	m_eventPhase				= false;
	m_eventDone					= false;
	m_eventStage				= 0;
	m_eventTimer				= 2500;

    m_bSubmerged				= false;
    m_bSubmergedOnce			= false;
    m_bSonsDead					= true;
    m_uiSummonCount				= 0;
    m_uiElementalFireTimer		= 3000;
    m_uiEmergeTimer				= 0;
    m_uiMeltWeaponTimer			= 10000;
    m_uiMightOfRagnarosTimer	= 20000;
    m_uiMagmaBlastTimer			= 2000;
    m_uiSubmergeTimer           = 3 * MINUTE * IN_MILLISECONDS; 
    m_uiWrathOfRagnarosTimer	= 30000;
    m_uiLavaBurstTimer			= urand(1000, 10000);
    m_uiPhase					= 0;

    m_creature->RemoveAurasDueToSpell(SPELL_RAGNAROS_SUBMERGE_FADE);
    m_creature->RemoveAurasDueToSpell(SPELL_RAGNAROS_SUBMERGE_EFFECT);
    m_creature->RemoveAurasDueToSpell(SPELL_MELT_WEAPON);

	m_creature->SetMeleeDamageSchool(SPELL_SCHOOL_FIRE);

	m_creature->addUnitState(UNIT_STAT_IGNORE_PATHFINDING);

    Trigger = 0;

    if (pInstance)
        pInstance->SetData(TYPE_RAGNAROS, NOT_STARTED);
}

void boss_ragnarosAI::JustReachedHome()
{
    if (pInstance)
	{
        pInstance->SetData(TYPE_RAGNAROS, NOT_STARTED);
		static_cast<instance_molten_core*>(pInstance)->RespawnDomo();
	}

	static_cast<TemporarySummon*>(m_creature)->UnSummon();
	//m_creature->RemoveFromWorld();
}

void boss_ragnarosAI::JustSummoned(Creature* pSummoned) 
{
    if (pSummoned->GetEntry() == NPC_FLAME_OF_RAGNAROS)
    {
        pSummoned->SetTargetGuid(ObjectGuid(HIGHGUID_PLAYER, m_uiMightOfRagnarosTargetGuid));
        //if (Player *pTarget = m_creature->GetMap()->GetPlayer())
    }
}

void boss_ragnarosAI::StartEvent(bool instant)
{
	if(instant)
		m_eventDone = true;
	else
		m_eventPhase = true;
}

void boss_ragnarosAI::JustDied(Unit* killer)
{
    if (pInstance)
        pInstance->SetData(TYPE_RAGNAROS, DONE);

	using namespace ProgressMonitor;
	if (killer->GetTypeId() == TYPEID_PLAYER)
		Track(static_cast<Player*>(killer), Raids::MoltenCore, Bosses::Ragnaros);
}

void boss_ragnarosAI::KilledUnit(Unit* pVictim)
{
	if(pVictim->GetEntry() != NPC_MAJORDOMO)
		DoScriptText(SAY_KILL, m_creature);
}

void boss_ragnarosAI::Aggro(Unit* pAttacker)
{
    m_creature->SetInCombatWithZone();
    m_creature->CastSpell(m_creature, SPELL_MELT_WEAPON, false);
	m_creature->SetMeleeDamageSchool(SPELL_SCHOOL_FIRE);

    if (pInstance)
        pInstance->SetData(TYPE_RAGNAROS, IN_PROGRESS);
}

Player* boss_ragnarosAI::DoSelectRandomNonMeleePlayer()
{
    if (!m_creature->CanHaveThreatList())
        return 0;

    std::vector<ObjectGuid> vGuids;
    m_creature->FillGuidsListFromThreatList(vGuids);
    std::vector<Player*> vManaPlayers;

    if (!vGuids.empty())
    {
        for (std::vector<ObjectGuid>::const_iterator itr = vGuids.begin(); itr != vGuids.end(); ++itr)
        {
            if (Unit* pTarget = m_creature->GetMap()->GetUnit(*itr))
            {
                if (!pTarget->IsCharmerOrOwnerPlayerOrPlayerItself())
                    continue;

                if (!m_creature->CanReachWithMeleeAttack(pTarget) && m_creature->getPowerType() == POWER_MANA)
                    vManaPlayers.push_back((Player*)pTarget);
            }
        }
    }

    if (!vManaPlayers.empty())
    {
        std::vector<Player*>::iterator i = vManaPlayers.begin();
        advance(i, (rand() % vManaPlayers.size()));
        return (*i);
    }
        
    return 0;
}

void boss_ragnarosAI::UpdateAI(const uint32 uiDiff)
{
	if(m_eventPhase)
	{
		if(m_eventTimer < uiDiff)
		{
			switch(m_eventStage)
			{
			case 0:
				DoScriptText(-1409012, m_creature);
				if (Player* pPlayer = pInstance->GetPlayerInMap(true, false))
					m_creature->SetFacingToObject(pPlayer);
				++m_eventStage;
                m_eventTimer = 0; // 12000 ToDo: When was this changed? 
				break;
			case 1:
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				if (Player* pPlayer = pInstance->GetPlayerInMap(true, false))
					AttackStart(pPlayer);
				m_eventDone = true;
				m_eventPhase = false;
				break;
			}
		}
		else
			m_eventTimer -= uiDiff;

		return;
	}
	
	if(!m_eventDone || !m_creature->SelectHostileTarget() || !m_creature->getVictim())
		return;

    // Lava Burst
    if (!m_bSubmerged && m_uiSubmergeTimer > m_uiLavaBurstTimer && m_uiPhase >= 4)
    {
        m_uiLavaBurstTimer = m_uiSubmergeTimer + 500;
    }

    if (m_uiLavaBurstTimer <= uiDiff)
    {
        if (DoCastSpellIfCan(m_creature, SPELL_LAVA_BURST_DUMMY) == CAST_OK)
            m_uiLavaBurstTimer = urand(1000, 10000);
    }
    else
        m_uiLavaBurstTimer -= uiDiff;

    if (m_bSubmerged)
    {
        m_bSonsDead = true;
        for(std::list<Creature*>::iterator i = pSons.begin(); i != pSons.end(); ++i)
        {
            if ((*i)->isAlive() && !(*i)->IsImmunedToDamage(SPELL_SCHOOL_MASK_NORMAL))
                m_bSonsDead = false;
        }

        // Emerge
        if (m_uiEmergeTimer <= uiDiff || m_bSonsDead)
        {
            if (++m_uiPhase == 1)
            {
                // Emerge animation
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveAurasDueToSpell(SPELL_RAGNAROS_SUBMERGE_FADE);
                DoCastSpellIfCan(m_creature, SPELL_RAGNAROS_EMERGE);
                m_uiEmergeTimer = 3000; // 2900 as cast time
            }
            else
            {
                // Ragnaros is fully emerged
                m_creature->RemoveAurasDueToSpell(SPELL_RAGNAROS_SUBMERGE_EFFECT);
                m_bSubmerged = false;
                m_uiPhase = 0;
                m_uiSubmergeTimer = 180000;
            }
        }
        else
            m_uiEmergeTimer -= uiDiff;
    }
    else
    {
        // Submerge Timer
        if (m_uiSubmergeTimer <= uiDiff)
        {
            // Submerge haben wir auch mehrere Phasen
            switch (m_uiPhase)
            {
                case 0:
                {
                    m_creature->AttackStop();

                    if (m_creature->IsNonMeleeSpellCasted(false))
                        m_creature->InterruptNonMeleeSpells(false);
                
                    if (DoCastSpellIfCan(m_creature, SPELL_RAGNAROS_SUBMERGE_EFFECT, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                        m_uiPhase++;

                    return;
                }
                case 1:
                {
					for (int i = 0; i < 8; ++i)
					{
						if (pInstance)
						{
							if (Player* pPlayer = pInstance->GetPlayerInMap(true, false))
							{
								switch(i)
								{
									case 0:
										if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_1X, ADD_1Y, ADD_1Z, ADD_1O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
											pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 1:
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_2X, ADD_2Y, ADD_2Z, ADD_2O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
											pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 2:
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_3X, ADD_3Y, ADD_3Z, ADD_3O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
											pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 3 :
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_4X, ADD_4Y, ADD_4Z, ADD_4O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
											pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 4:
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_5X, ADD_5Y, ADD_5Z, ADD_5O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
										pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 5:
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_6X, ADD_6Y, ADD_6Z, ADD_6O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
										pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 6:
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_7X, ADD_7Y, ADD_7Z, ADD_7O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
										pSummoned->AI()->AttackStart(pPlayer);
									break;
									case 7:
									if (Creature* pSummoned = m_creature->SummonCreature(12143,ADD_8X, ADD_8Y, ADD_8Z, ADD_8O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
										pSummoned->AI()->AttackStart(pPlayer);
									break;
								}
							}
						}
					}
					m_uiPhase++;
                    return;
                }
                case 2:
                {
                    pSons.clear();
			        // Create list of Sons, 8 in total
			        GetCreatureListWithEntryInGrid(pSons, m_creature, NPC_SON_OF_FLAME, 300.0f);
			        for(std::list<Creature*>::iterator i = pSons.begin(); i != pSons.end(); ++i)
			        {
                        if ((*i)->isAlive())
				        {
                            (*i)->SetInCombatWithZone();
                            (*i)->SetMeleeDamageSchool(SPELL_SCHOOL_FIRE);
				        }
                    }

                    if (!pSons.empty())
					{
                        m_uiPhase++;
						m_uiSubmergeTimer = 1000;
					}
                        
                    return;
                }
                case 3:
                {
                    if (!m_bSubmergedOnce)
                    {
                        DoScriptText(SAY_SUMMON_FIRST, m_creature);
                        m_bSubmergedOnce = true;
                    }
                    else
                        DoScriptText(SAY_SUMMON, m_creature);

                    m_uiPhase++;
                    return;
                }
                case 4:
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_RAGNAROS_SUBMERGE_FADE) == CAST_OK)
                        m_uiPhase++;

                    return;
                }
                case 5:
                {
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_bSubmerged = true;
                    m_uiEmergeTimer = 90000;
                    m_uiPhase = 0;
                    return;
                }
            }
        }
        else
            m_uiSubmergeTimer -= uiDiff;

        if (m_uiPhase > 0)
            return;

        // Elemental Fire
        if (m_uiElementalFireTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ELEMENTAL_FIRE) == CAST_OK)
                m_uiElementalFireTimer = urand(10000, 14000);
        }
        else
            m_uiElementalFireTimer -= uiDiff;

        // Melt Weapon Recast
        if (m_uiMeltWeaponTimer <= uiDiff)
        {               
            m_uiMeltWeaponTimer = 10000;
            m_creature->RemoveAurasDueToSpell(SPELL_MELT_WEAPON);
            m_creature->CastSpell(m_creature, SPELL_MELT_WEAPON, false);
        }
        else
            m_uiMeltWeaponTimer -= uiDiff;

        if (m_uiMightOfRagnarosTimer <= uiDiff)
        {
            if (Player* pManaPlayer = DoSelectRandomNonMeleePlayer())
            {
                if (DoCastSpellIfCan(pManaPlayer, SPELL_MIGHT_OF_RAGNAROS) == CAST_OK)
                {
                    DoScriptText(SAY_MIGHT, m_creature);
                    m_uiMightOfRagnarosTimer = 11000;
                    m_uiMightOfRagnarosTargetGuid = pManaPlayer->GetGUIDLow();
                }
            }
        }
        else
            m_uiMightOfRagnarosTimer -= uiDiff;

        // Wrath Of Ragnaros
        if (m_uiWrathOfRagnarosTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WRATH_OF_RAGNAROS);
            DoScriptText(SAY_WRATH, m_creature);
            m_uiWrathOfRagnarosTimer = 30000;
        }
        else
            m_uiWrathOfRagnarosTimer -= uiDiff;

        // If we are within range melee the target
        if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            // Make sure our attack is ready and we arn't currently casting
            if (m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
                //m_creature->CastSpell(m_creature, SPELL_MELT_WEAPON, false);
            }
        }
        else
        {
            // Look-up for a new closest target in melee range
            std::vector<ObjectGuid> vGuids;
            m_creature->FillGuidsListFromThreatList(vGuids);
            if (!vGuids.empty())
            {
                for (std::vector<ObjectGuid>::const_iterator itr = vGuids.begin(); itr != vGuids.end(); ++itr)
                {
                    if (Unit* pTarget = m_creature->GetMap()->GetUnit(*itr))
                    {
                        if (!m_creature->CanReachWithMeleeAttack(pTarget))
                            m_creature->getThreatManager().modifyThreatPercent(pTarget,-75);
                        else
                            m_creature->getThreatManager().addThreat(pTarget, 500.0f);
                    }
                }

                if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
                    return;
            }

            // Magma Blast
            if (m_uiMagmaBlastTimer <= uiDiff)
            {   
                if (Unit* bTarget = DoSelectRandomNonMeleePlayer())
                {
                    if (DoCastSpellIfCan(bTarget, SPELL_MAGMA_BLAST) == CAST_OK)
                    {
                        if(urand(1,4) == 1) DoScriptText(SAY_MAGMA_BLAST, m_creature);
                        m_uiMagmaBlastTimer = 2000;
                    }
                }
            }
            else
                m_uiMagmaBlastTimer -= uiDiff;
        }
    }
}

void boss_ragnarosAI::SummonedCreatureJustDied(Creature* pSummoned)
{
    pSons.remove(pSummoned);
}

CreatureAI* GetAI_boss_ragnaros(Creature* pCreature)
{
    return new boss_ragnarosAI(pCreature);
}

enum eFlameOfRagnaros
{
	SPELL_MANA_BURN_AURA	= 21857
};

struct MANGOS_DLL_DECL flame_of_ragnarosAI : public ScriptedAI
{
    flame_of_ragnarosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

	uint32 m_uiManaBurnTimer;
	uint32 m_uiCheckAttackTimer;

    void Reset()
    {
		m_uiManaBurnTimer		= 1000;
		m_uiCheckAttackTimer	= 5000;

		m_creature->addUnitState(UNIT_STAT_IGNORE_PATHFINDING);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiManaBurnTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_MANA_BURN_AURA);
            m_uiManaBurnTimer = 30000;
        } else m_uiManaBurnTimer -= uiDiff;

        if (m_uiCheckAttackTimer < uiDiff && !m_creature->isInCombat())
        {
			if (Player* pPlayer = pInstance->GetPlayerInMap(true, false))
			{
				m_creature->AI()->AttackStart(pPlayer);
				return;
			}
			m_uiCheckAttackTimer = 5000;
            
        } else m_uiCheckAttackTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_flame_of_ragnaros(Creature* pCreature)
{
    return new flame_of_ragnarosAI(pCreature);
}


struct MANGOS_DLL_DECL knockbackDummyAI : public ScriptedAI
{
	bool done;
    uint32 despawnTimer;

    knockbackDummyAI(Creature* pCreature) : ScriptedAI(pCreature), done(false), despawnTimer(200)
	{
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
	}

	void Reset() { }

	void UpdateAI(uint32 timeDelta)
	{
		if(!done)
        {
            Player* pPlayer = nullptr;
            if (auto map = m_creature->GetMap())
                pPlayer = map->GetPlayer(m_creature->GetTargetGuid());

            if (pPlayer)
                pPlayer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            m_creature->CastSpell(m_creature, 21155, true);

            if (pPlayer)
                pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            done = true;
		}
		else
		{
			if(despawnTimer <= timeDelta)
				m_creature->RemoveFromWorld();
			else
				despawnTimer -= timeDelta;
		}
	}
};

void AddSC_boss_ragnaros()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ragnaros";
    newscript->GetAI = &GetAI_boss_ragnaros;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "flame_of_ragnaros";
    newscript->GetAI = &GetAI_flame_of_ragnaros;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "ragnaros_knockback_dummy";
    newscript->GetAI = [](Creature* creature) -> CreatureAI*
	{
		return new knockbackDummyAI(creature);
	};
    newscript->RegisterSelf();
}
