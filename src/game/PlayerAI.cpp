
#include "Player.h"
#include "Log.h"
#include "SpellMgr.h"
#include "World.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "Group.h"
#include "Pet.h"
#include "Util.h"
#include "Spell.h"
#include "ScriptMgr.h"
#include "PlayerAI.h"

#include <memory>

SpellFamily SpellFamilies[12] =
{
	SPELLFAMILY_GENERIC,
	SPELLFAMILY_WARRIOR,
	SPELLFAMILY_PALADIN,
	SPELLFAMILY_HUNTER,
	SPELLFAMILY_ROGUE,
	SPELLFAMILY_PRIEST,
	SPELLFAMILY_GENERIC,
	SPELLFAMILY_SHAMAN,
	SPELLFAMILY_MAGE,
	SPELLFAMILY_WARLOCK,
	SPELLFAMILY_GENERIC,
	SPELLFAMILY_DRUID
};

bool IsBlacklistedSpell(SpellEntry const * entry)
{
    switch (entry->Id)
    {
        case 783:
        case 1066:
        case 1953:
        case 5118:
        case 13161:
        case 13159:
        case 14274:
        case 15629:
        case 15630:
        case 15631:
        case 15632:
        case 20736:
            return true;
        default:
            return false;
    }
}

PlayerAI::PlayerAI(Player* player, uint32 spellId) :
	mPlayer(player),
	mSpellId(spellId),
	rnd_generator(spellId),
	dist(1000, 3000),
	mTimer(2000),
    mTarget()
{
	auto spellMap = player->GetSpellMap();
	mSpells.reserve(spellMap.size());

	for(auto& spellEntry : spellMap)
	{
		if(spellEntry.second.active && spellEntry.second.dependent == false)
        {
            SpellEntry const * entry = sSpellStore.LookupEntry(spellEntry.first);

            if (!entry || IsBlacklistedSpell(entry))
                continue;

            if (entry && !entry->HasAttribute(SPELL_ATTR_PASSIVE) && !entry->HasAttribute(SPELL_ATTR_CANT_USED_IN_COMBAT))
            {
                bool higherRank = false;
                for (std::vector<uint32>::iterator itr = mSpells.begin(); itr != mSpells.end(); ++itr)
                {
                    if (sSpellMgr.GetFirstSpellInChain(spellEntry.first) == sSpellMgr.GetFirstSpellInChain(*itr))
                    {
                        SpellEntry const * lower = sSpellStore.LookupEntry(*itr);
                        if (lower->Rank < entry->Rank)
                            mSpells.erase(itr);
                        else
                            higherRank = true;
                        break;
                    }
                }
			
                if (higherRank)
                    continue;

				auto sameFamily = static_cast<SpellFamily>(entry->SpellFamilyName) == SpellFamilies[player->getClass()];
				if (!sameFamily)
					continue;

				if (entry->RecoveryTime >= 60)
					continue;

				bool costsReagent = false;
				for (int i = 0; i < MAX_SPELL_REAGENTS && !costsReagent; ++i)
					costsReagent = entry->Reagent[i] != 0;

				if (!costsReagent)
					mSpells.push_back(entry->Id);
			}
		}
	}

	Chase();
}


PlayerAI::~PlayerAI()
{
}

Unit* PlayerAI::SelectTarget()
{
    Unit* target = mPlayer->GetMap()->GetUnit(mTarget);

    if (target && target->isTargetableForAttack())
        return target;

    Unit* charmer = mPlayer->GetCharmer();
    if (!charmer)
        return nullptr;

    if (charmer->GetTypeId() != TYPEID_UNIT)
        return charmer->getVictim();

    std::vector<ObjectGuid> targets;
    ((Creature*)charmer)->FillGuidsListFromThreatList(targets);

    std::random_shuffle(targets.begin(), targets.end());

    while (targets.size())
    {
        if (targets.back() != mPlayer->GetObjectGuid())
        {
            target = mPlayer->GetMap()->GetUnit(targets.back());
            if (target && target->isTargetableForAttack())
            {
                mTarget = targets.back();
                return target;
            }
        }
        targets.pop_back();
    }

    return charmer->getVictim();
}

void PlayerAI::UpdateAI(uint32 timeDelta)
{
    if (!CheckCharmer())
        return;

    if (mPlayer->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)
        Chase();

	if(mTimer <= timeDelta && !mPlayer->IsNonMeleeSpellCasted(false))
	{
		std::random_shuffle(mSpells.begin(), mSpells.end());

        for (unsigned i = 0; i < mSpells.size(); i++)
        {
            if (CastSpell(mSpells[i]))
                break;
        }
		mTimer = dist(rnd_generator);
	}
	else
		mTimer -= timeDelta;
}


bool PlayerAI::CastSpell(uint32 spellId, bool triggered/* = false*/)
{
	Unit* target = nullptr;
	Unit* charmer = mPlayer->GetCharmer();
	if(!charmer)
		return true;

	if(IsPositiveSpell(spellId) && spellId != 5)
        target = charmer;
	else
        target = SelectTarget();

    // e. g. Mindcontrol
    if (!target && charmer->GetOwner())
        target = charmer->GetOwner()->getVictim();

	if (!mPlayer->CanCast(target, spellId, triggered))
		return false;
	
    SpellEntry const * entry = sSpellStore.LookupEntry(spellId);

    if (!entry)
        return false;

    auto spell = std::make_shared<Spell>(mPlayer, entry, triggered);
    SpellCastTargets targets;
    targets.setUnitTarget(target);
    spell->m_targets = targets;

    if (!target || !target->IsInWorld())
        return false;

    SpellCastResult result = spell->CheckCast(true);

    if (result != SPELL_CAST_OK)
        return false;

    result = spell->CheckRange(true);

    if (result != SPELL_CAST_OK)
        return false;

    spell->prepare(&targets);

    mPlayer->GetMotionMaster()->Clear();
    mPlayer->MonsterMoveWithSpeed(mPlayer->GetPositionX(), mPlayer->GetPositionY(), mPlayer->GetPositionZ(), mPlayer->GetSpeed(MOVE_RUN));
    mPlayer->StopMoving(true);

    return true;
}


void PlayerAI::Chase()
{
	Unit* charmer = mPlayer->GetCharmerOrOwner();
	if(!charmer || !charmer->getVictim() || mPlayer->IsNonMeleeSpellCasted(false))
		return;

    Unit* target = SelectTarget();

	if(!target)
		return;

    mPlayer->Attack(target, true);
	mPlayer->GetMotionMaster()->MoveChase(target, 0.f);
}


bool PlayerAI::CheckCharmer()
{
	Unit* charmer = mPlayer->GetCharmerOrOwner();
	if(!charmer || charmer->isDead())
    {
		UnCharm();
        return false;
    }
    return true;
}


void PlayerAI::UnCharm()
{
	for(int i = 0; i < MAX_EFFECT_INDEX; ++i)
		mPlayer->RemoveAura(mSpellId, static_cast<SpellEffectIndex>(i));

    mPlayer->StopMoving(true);
    mPlayer->GetMotionMaster()->MovementExpired(false);
	mPlayer->SetCharmed(false, 0, 0);
}


void PlayerAI::MovementInform()
{
	
}