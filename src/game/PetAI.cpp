/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos-zero>
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

#include "PetAI.h"
#include "Errors.h"
#include "Pet.h"
#include "Player.h"
#include "DBCStores.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"
#include "Creature.h"
#include "World.h"
#include "Util.h"
#include "Unit.h"
#include "Movement\MoveSpline.h"

int PetAI::Permissible(const Creature *creature)
{
    if( creature->IsPet())
        return PERMIT_BASE_SPECIAL;

    return PERMIT_BASE_NO;
}

PetAI::PetAI(Creature *c) : CreatureAI(c), i_tracker(TIME_INTERVAL_LOOK), inCombat(false), moveToOwner(false)
{
    m_AllySet.clear();
    UpdateAllies();
}

void PetAI::MoveInLineOfSight(Unit *u)
{
    if (m_creature->getVictim())
        return;

    if (m_creature->IsPet() && ((Pet*)m_creature)->GetModeFlags() & PET_MODE_DISABLE_ACTIONS)
        return;

    if (!m_creature->GetCharmInfo() || !m_creature->GetCharmInfo()->HasReactState(REACT_AGGRESSIVE) || moveToOwner)
        return;

    if (u->isTargetableForAttack() && m_creature->IsHostileTo( u ) &&
        u->isInAccessablePlaceFor(m_creature))
    {
        float attackRadius = m_creature->GetAttackDistance(u);
        if(m_creature->IsWithinDistInMap(u, attackRadius) && m_creature->GetDistanceZ(u) <= CREATURE_Z_ATTACK_RANGE)
        {
			if (m_creature->IsWithinLOSInMap(u))
			{
				if (IsVisible(u))
				{
					AttackStart(u);
					u->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
				}
			}
        }
    }
}

void PetAI::AttackStart(Unit *u)
{
    if(!u || (m_creature->IsPet() && ((Pet*)m_creature)->getPetType() == MINI_PET))
        return;

    if(m_creature->Attack(u,true))
    {
        // TMGs call CreatureRelocation which via MoveInLineOfSight can call this function
        // thus with the following clear the original TMG gets invalidated and crash, doh
        // hope it doesn't start to leak memory without this :-/
        //i_pet->Clear();

		HandleMovementOnAttackStart(u);
        inCombat = true;
        moveToOwner = false;
    }
}

void PetAI::EnterEvadeMode()
{
}

bool PetAI::IsVisible(Unit *pl) const
{
    return _isVisible(pl);
}

bool PetAI::_needToStop() const
{
    // This is needed for charmed creatures, as once their target was reset other effects can trigger threat
    if(m_creature->isCharmed() && m_creature->getVictim() == m_creature->GetCharmer())
        return true;

	return !m_creature->getVictim()->isTargetableForAttack() && IsVisible(m_creature->getVictim());
}

void PetAI::_stopAttack()
{
    inCombat = false;

    Unit* owner = m_creature->GetCharmerOrOwner();

    if(owner && m_creature->GetCharmInfo() && m_creature->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW))
    {
        moveToOwner = true;
        m_creature->GetMotionMaster()->MoveFollow(owner,PET_FOLLOW_DIST,PET_FOLLOW_ANGLE);
    }
    else
    {
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MoveIdle();
    }
    m_creature->AttackStop();
}

void PetAI::UpdateAI(const uint32 diff)
{
    if (!m_creature->isAlive())
        return;

    Unit* owner = m_creature->GetCharmerOrOwner();

    if (m_updateAlliesTimer <= diff)
        // UpdateAllies self set update timer
        UpdateAllies();
    else
        m_updateAlliesTimer -= diff;

	auto* victim = m_creature->getVictim();
	auto isPet = m_creature->IsPet();
	auto disable = ((Pet*)m_creature)->GetModeFlags() & PET_MODE_DISABLE_ACTIONS;

    if (inCombat && (!victim || (isPet && disable)))
        _stopAttack();

    if (moveToOwner && (!owner || m_creature->GetDistance2d(owner) <= 2 * PET_FOLLOW_DIST || !m_creature->hasUnitState(UNIT_STAT_FOLLOW)))
    {
        if (!IsCombatMovement() && !m_creature->GetCharmInfo()->HasReactState(REACT_PASSIVE))
        {
            SetCombatMovement(true);
            HandleMovementOnAttackStart(m_creature->getVictim());
        }
        if (victim)
            inCombat = true;
        moveToOwner = false;
    }

    // i_pet.getVictim() can't be used for check in case stop fighting, i_pet.getVictim() clear at Unit death etc.
    if (m_creature->getVictim())
    {
        if (_needToStop())
        {
            DEBUG_FILTER_LOG(LOG_FILTER_AI_AND_MOVEGENSS, "PetAI (guid = %u) is stopping attack.", m_creature->GetGUIDLow());
            _stopAttack();
            return;
        }

        bool meleeReach = m_creature->CanReachWithMeleeAttack(m_creature->getVictim());
		const uint32 imp = 416;
        const uint32 succubus = 1863;

        if (m_creature->GetEntry() == imp)
        {
            const float imp_combat_dist = 30.f;

            bool shouldChase = m_creature->GetDistance2d(m_creature->getVictim()) > imp_combat_dist;
            if (IsCombatMovement() != shouldChase)
                SetCombatMovement(shouldChase, !moveToOwner);
        }
        if (m_creature->IsStopped() || meleeReach)
        {
            // Hack for Succubs-Seduction
            if (m_creature->GetEntry() == succubus && m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL)
                && m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->m_spellInfo->Id == 6358 
                && m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->IsChannelActive()
                && m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->m_targets.getUnitTarget()
                && !m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->m_targets.getUnitTarget()->HasAura(6358))
                m_creature->InterruptNonMeleeSpells(false);

            // required to be stopped cases
            if (m_creature->IsStopped() && m_creature->IsNonMeleeSpellCasted(false))
            {
                if (m_creature->hasUnitState(UNIT_STAT_FOLLOW_MOVE))
                    m_creature->InterruptNonMeleeSpells(false);
                else
                    return;
            }
            // not required to be stopped case
            else if (m_creature->GetEntry() != imp && DoMeleeAttackIfReady())
            {
                if (!m_creature->getVictim())
                    return;

                if (owner && owner->GetTypeId() == TYPEID_PLAYER && m_creature->getVictim()->IsPvP())
                    ((Player*)owner)->UpdatePvP(true);

                //if pet misses its target, it will also be the first in threat list
                m_creature->getVictim()->AddThreat(m_creature);

                if (_needToStop())
                    _stopAttack();
            }
        }
    }
    else if (owner && m_creature->GetCharmInfo())
    {
        if (owner->isInCombat() && !(m_creature->GetCharmInfo()->HasReactState(REACT_PASSIVE) || m_creature->GetCharmInfo()->HasCommandState(COMMAND_STAY)) && !moveToOwner)
        {
            AttackStart(owner->getAttackerForHelper());
        }
        else if(m_creature->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW))
        {
            if (!m_creature->hasUnitState(UNIT_STAT_FOLLOW) )
            {
                m_creature->GetMotionMaster()->MoveFollow(owner,PET_FOLLOW_DIST,PET_FOLLOW_ANGLE);
            }
        }
    }

    // Autocast (casted only in combat or persistent spells in any state)
    if (!m_creature->IsNonMeleeSpellCasted(false) && !moveToOwner)
    {
        typedef std::vector<std::pair<Unit*, std::shared_ptr<Spell>>> TargetSpellList;
        TargetSpellList targetSpellStore;

        for (uint8 i = 0; i < m_creature->GetPetAutoSpellSize(); ++i)
        {
            uint32 spellID = m_creature->GetPetAutoSpellOnPos(i);
            if (!spellID)
                continue;

            SpellEntry const *spellInfo = sSpellStore.LookupEntry(spellID);
            if (!spellInfo)
                continue;

            if (m_creature->GetCharmInfo() && m_creature->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                continue;

            if ((m_creature->IsPet() && static_cast<Pet*>(m_creature)->HasSpellCooldown(spellID))
                || m_creature->HasSpellCooldown(spellID))
				continue;

            // ignore some combinations of combat state and combat/noncombat spells
            if (!inCombat)
            {
                if ((spellID == 24450 && !m_creature->HasAura(24450)) || (spellID == 24452 && !m_creature->HasAura(24452)) || (spellID == 24453 && !m_creature->HasAura(24453)))
					((Pet*)m_creature)->ToggleAutocast(spellID, true);

                // ignore attacking spells, and allow only self/around spells
                if (!IsPositiveSpell(spellInfo->Id))
                    continue;

                // non combat spells allowed
                // only pet spells have IsNonCombatSpell and not fit this reqs:
                // Consume Shadows, Lesser Invisibility, so ignore checks for its
                if (!IsNonCombatSpell(spellInfo))
                {
                    // allow only spell without spell cost or with spell cost but not duration limit
                    int32 duration = GetSpellDuration(spellInfo);
                    if ((spellInfo->manaCost || spellInfo->ManaCostPercentage || spellInfo->manaPerSecond) && duration > 0)
                        continue;

                    // allow only spell without cooldown > duration
                    int32 cooldown = GetSpellRecoveryTime(spellInfo);
                    if (cooldown >= 0 && duration >= 0 && cooldown > duration)
                        continue;

                    // not allow instant kill autocasts as full health cost
                    if (IsSpellHaveEffect(spellInfo, SPELL_EFFECT_INSTAKILL))
                        continue;
                }
                // prevent Consume Shadows with almost full hp
                else if (spellInfo->SpellIconID == 207 && m_creature->GetHealthPercent() > 80.f)
                    continue;
            }
            else
            {
                // just ignore non-combat spells
                if (IsNonCombatSpell(spellInfo))
                    continue;
            }

			auto spell = std::make_shared<Spell>(m_creature, spellInfo, false);

            if (inCombat && !m_creature->hasUnitState(UNIT_STAT_FOLLOW) && m_creature->getVictim() && spell->CanAutoCast(m_creature->getVictim()))
            {
                if (CanCastSpell(m_creature->getVictim(), spellInfo, false) == CAST_OK)
                    targetSpellStore.push_back(TargetSpellList::value_type(m_creature->getVictim(), spell));
                continue;
            }
            else
            {
                bool spellUsed = false;
                for (AllySet::const_iterator tar = m_AllySet.begin(); tar != m_AllySet.end(); ++tar)
                {
                    Unit* Target = m_creature->GetMap()->GetUnit(*tar);

                    //only buff targets that are in combat, unless the spell can only be cast while out of combat
                    if (!Target)
                        continue;

                    if (spell->CanAutoCast(Target) && CanCastSpell(Target, spellInfo, false) == CAST_OK)
                    {
                        targetSpellStore.push_back(TargetSpellList::value_type(Target, spell));
                        spellUsed = true;
                        break;
                    }
                }
                //if (!spellUsed)
                  //  delete spell;
            }
        }

        //found units to cast on to
        if (!targetSpellStore.empty())
        {
            uint32 index = urand(0, targetSpellStore.size() - 1);

            auto spell  = targetSpellStore[index].second;
            Unit*  target = targetSpellStore[index].first;

            targetSpellStore.erase(targetSpellStore.begin() + index);

            SpellCastTargets targets;
            targets.setUnitTarget( target );

            if (!m_creature->HasInArc(M_PI_F, target))
            {
                m_creature->SetInFront(target);
                if (target->GetTypeId() == TYPEID_PLAYER)
                    m_creature->SendCreateUpdateToPlayer((Player*)target);

                if (owner && owner->GetTypeId() == TYPEID_PLAYER)
                    m_creature->SendCreateUpdateToPlayer( (Player*)owner );
            }

            if (owner && owner->GetTypeId() == TYPEID_PLAYER && owner != target && target->IsPvP())
                ((Player*)owner)->UpdatePvP(true);

            m_creature->AddCreatureSpellCooldown(spell->m_spellInfo->Id);
            if (m_creature->IsPet())
                ((Pet*)m_creature)->CheckLearning(spell->m_spellInfo->Id);

            spell->prepare(&targets);
        }

        // deleted cached Spell objects
        //for(TargetSpellList::const_iterator itr = targetSpellStore.begin(); itr != targetSpellStore.end(); ++itr)
        //    delete itr->second;
    }
}

bool PetAI::_isVisible(Unit *u) const
{
    return m_creature->IsWithinDist(u,sWorld.getConfig(CONFIG_FLOAT_SIGHT_GUARDER))
        && u->isVisibleForOrDetect(m_creature,m_creature,true);
}

void PetAI::UpdateAllies()
{
    Unit* owner = m_creature->GetCharmerOrOwner();
    Group *pGroup = NULL;

    m_updateAlliesTimer = 10*IN_MILLISECONDS;                //update friendly targets every 10 seconds, lesser checks increase performance

    if (!owner)
        return;
    else if (owner->GetTypeId() == TYPEID_PLAYER)
        pGroup = ((Player*)owner)->GetGroup();

    //only pet and owner/not in group->ok
    if (m_AllySet.size() == 2 && !pGroup)
        return;
    //owner is in group; group members filled in already (no raid -> subgroupcount = whole count)
    if (pGroup && !pGroup->isRaidGroup() && m_AllySet.size() == (pGroup->GetMembersCount() + 2))
        return;

    m_AllySet.clear();
    m_AllySet.insert(m_creature->GetObjectGuid());
    if (pGroup)                                             //add group
    {
        for (GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* target = itr->getSource();
            if (!target || !pGroup->SameSubGroup((Player*)owner, target))
                continue;

            if (target->GetObjectGuid() == owner->GetObjectGuid())
                continue;

            m_AllySet.insert(target->GetObjectGuid());
        }
    }
    else                                                    //remove group
        m_AllySet.insert(owner->GetObjectGuid());
}

void PetAI::AttackedBy(Unit *attacker)
{
    //when attacked, fight back in case 1)no victim already AND 2)not set to passive AND 3)not set to stay, unless can it can reach attacker with melee attack anyway
    if(!m_creature->getVictim() && m_creature->GetCharmInfo() && !m_creature->GetCharmInfo()->HasReactState(REACT_PASSIVE) &&
        (!m_creature->GetCharmInfo()->HasCommandState(COMMAND_STAY) || m_creature->CanReachWithMeleeAttack(attacker)) && !moveToOwner)
        AttackStart(attacker);
}
