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

#include "precompiled.h"
#include "blackwing_lair.h"
#include "TemporarySummon.h"
#include "../system/ProgressMonitor.h"
#include "TemporarySummon.h"

struct DrakonIdInfo
{
    DrakonIdInfo(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z) {}

    float x, y, z;
};

void populateBoneConstructs(const std::vector<DrakonIdInfo>& dragons, CreatureAI* ai);

  ///////////////////////
 /////  PHASE ONE  /////
///////////////////////

/* ScriptData
SD%Complete: 75
SDComment:		Victor Nefarius:	spawns Nef in wrong place, need more flying points
                Nefarian :			Some issues with class calls effecting more than one class (core problems)
EndScriptData */

enum
{
    SAY_GAMESBEGIN_1                = -1469004,
    SAY_GAMESBEGIN_2                = -1469005,
    SAY_BEGIN_PHASE2                   = -1469007,

    MAX_DRAKES                      = 5,
    MAX_DRAKE_SUMMONS               = 42,

    SPELL_NEFARIUS_BARRIER          = 22663,                // immunity in phase 1
    SPELL_SHADOWBOLT                = 22677,
    SPELL_FEAR                      = 26678,                
    SPELL_SHADOWBOLT_VOLLEY			= 22665,
    SPELL_SILENCE					= 22666,
    SPELL_SHADOW_COMMAND			= 22667,
    SPELL_SHADOWBLINK				= 22664,				// or 22681 -> teleport around the room, possibly random
    SPELL_BONE_CONTRUST				= 23363,
	SPELL_ROOT						= 31366,

    FACTION_BLACK_DRAGON            = 103,
    FACTION_HOSTILE					= 14,
};

struct SpawnLocation
{
    float m_fX, m_fY, m_fZ;
};

static const SpawnLocation aNefarianLocs[5] =
{
    {-7599.32f, -1191.72f, 475.545f},                       // opening where red/blue/black darknid spawner appear (ori 3.05433)
    {-7526.27f, -1135.04f, 473.445f},                       // same as above, closest to door (ori 5.75959)
    {-7498.177f, -1273.277f, 481.649f},                     // nefarian spawn location (ori 1.798)
    {-7592.0f, -1264.0f, 481.0f},                           // hide pos (useless; remove this)
    {-7502.002f, -1256.503f, 476.758f},                     // nefarian fly to this position
};

#define GOSSIP_ITEM_NEFARIUS_1           "Ich habe keine Fehler gemacht."
#define GOSSIP_ITEM_NEFARIUS_2           "Ihr habt den Verstand verloren, Nefarius! Ihr sprecht in R\303\244tseln."
#define GOSSIP_ITEM_NEFARIUS_3           "Bitte tut es."

static const uint32 aPossibleDrake[MAX_DRAKES] = {NPC_BRONZE_DRAKANOID, NPC_BLUE_DRAKANOID, NPC_RED_DRAKANOID, NPC_GREEN_DRAKANOID, NPC_BLACK_DRAKANOID};


struct MANGOS_DLL_DECL boss_victor_nefariusAI : public ScriptedAI
{
    boss_victor_nefariusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        uint8 uiPos1 = urand(0, MAX_DRAKES - 1);
        uint8 uiPos2 = (uiPos1 + urand(1, MAX_DRAKES - 1)) % MAX_DRAKES;

        m_uiDrakeTypeOne = aPossibleDrake[uiPos1];
        m_uiDrakeTypeTwo = aPossibleDrake[uiPos2];

        Reset();
    }

	ScriptedInstance* m_pInstance;
	std::vector<DrakonIdInfo> NefarianDragons;

    uint32 m_uiSpawnedAddDeadCounter;
    uint32 m_uiAddSpawnTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiShadowBoltVolleyTimer;
    uint32 m_uiSilenceTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiShadowBlinkTimer;
    uint32 m_uiShadowCommandTimer;

    uint32 m_uiDrakeTypeOne;
    uint32 m_uiDrakeTypeTwo;

	uint32 m_uiPhase2Timer;

    bool summonedDragonNefarian;

    Unit* ShadowBoltPlayer;

    void Reset()
    {
        m_uiSpawnedAddDeadCounter	= 0;
        m_uiAddSpawnTimer			= 5000;
        m_uiShadowBoltTimer			= 0;
        m_uiShadowBoltVolleyTimer	= urand(8000, 15000);
        m_uiFearTimer				= urand(4000, 8000);
        m_uiSilenceTimer			= urand(5000, 12000);
        m_uiShadowCommandTimer		= urand(8000, 15000);
        m_uiShadowBlinkTimer		= 1000;
        summonedDragonNefarian		= false;
		m_uiPhase2Timer = 10000;

        if (m_creature->GetVisibility() != VISIBILITY_ON)
            m_creature->SetVisibility(VISIBILITY_ON);

        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->setFaction(FACTION_FRIENDLY);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		m_creature->SetStandState(UNIT_STAND_STATE_SIT_HIGH_CHAIR);
		SetCombatMovement(true);
    }

    void Aggro(Unit* pWho)
    {
        if (isEndbossFight())
        {
            if (m_pInstance)
                m_pInstance->SetData(TYPE_NEFARIAN, IN_PROGRESS);
        } else
		{
		    if (m_pInstance)
				m_pInstance->SetData(TYPE_NEFARIAN, SPECIAL);
		}
		SetCombatMovement(false);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NEFARIAN, FAIL);

		NefarianDragons.clear();

		m_creature->CastStop(false);
		Reset();
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_NEFARIAN)
        {
			//pSummoned->SetInCombatWithZone();
			pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

            pSummoned->SetRespawnDelay(7*DAY);

            pSummoned->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
            // see boss_onyxia (also note the removal of this in boss_nefarian)
            pSummoned->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND/* | UNIT_BYTE1_FLAG_UNK_2*/);
            pSummoned->m_movementInfo.AddMovementFlag(MOVEFLAG_FLYING);

            // Let Nefarian fly towards combat area
            pSummoned->GetMotionMaster()->MovePoint(1, aNefarianLocs[4].m_fX, aNefarianLocs[4].m_fY, aNefarianLocs[4].m_fZ);
			populateBoneConstructs(NefarianDragons, pSummoned->AI());
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        // If Nefarian has reached combat area, let him attack
        if (pSummoned->GetEntry() == NPC_NEFARIAN && uiMotionType == POINT_MOTION_TYPE && uiPointId == 1)
        {
			pSummoned->HandleEmote(EMOTE_ONESHOT_LAND);

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        // Despawn self when Nefarian is killed
        if (pSummoned->GetEntry() == NPC_NEFARIAN)
            m_creature->ForcedDespawn();
        else
        {
            m_uiSpawnedAddDeadCounter++;
            pSummoned->CastSpell(pSummoned, SPELL_BONE_CONTRUST, true);

            float fX, fY, fZ;
            pSummoned->GetPosition(fX, fY, fZ);
            NefarianDragons.push_back(DrakonIdInfo(fX, fY, fZ));
        }
    }

    void SpawnDragonWave()
    {
		uint32 uiCreatureId = 0;
		// 1 in 3 chance it will be a chromatic
		uiCreatureId = urand(0, 2) ? m_uiDrakeTypeOne : uint32(NPC_CHROMATIC_DRAKANOID);
		Creature* cDragon = m_creature->SummonCreature(uiCreatureId, aNefarianLocs[0].m_fX, aNefarianLocs[0].m_fY, aNefarianLocs[0].m_fZ, 5.000f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);

		// 1 in 3 chance it will be a chromatic
		uiCreatureId = urand(0, 2) ? m_uiDrakeTypeTwo : uint32(NPC_CHROMATIC_DRAKANOID);
		Creature* c1Dragon = m_creature->SummonCreature(uiCreatureId, aNefarianLocs[1].m_fX, aNefarianLocs[1].m_fY, aNefarianLocs[1].m_fZ, 5.000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);

		if (cDragon || c1Dragon)
		{
			//source hordeguides
			cDragon->setFaction(FACTION_HOSTILE);
			c1Dragon->setFaction(FACTION_HOSTILE);

			//dragon immunity
			switch(m_uiDrakeTypeOne)
			{
				case NPC_BRONZE_DRAKANOID :
					cDragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ARCANE, true);
					break;
				case NPC_BLUE_DRAKANOID :
					cDragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FROST, true);
					cDragon->SetResistance(SPELL_SCHOOL_ARCANE, 300);
					break;
				case NPC_RED_DRAKANOID :
					cDragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FIRE, true);
					break;
				case NPC_GREEN_DRAKANOID :
					cDragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NATURE, true);
					break;
				case NPC_BLACK_DRAKANOID :
					cDragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FIRE, true);
					cDragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, true);
					break;
			}

			//dragon immunity
			switch(m_uiDrakeTypeTwo)
			{
				case NPC_BRONZE_DRAKANOID :
					c1Dragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ARCANE, true);
					break;
				case NPC_BLUE_DRAKANOID :
					c1Dragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FROST, true);
					c1Dragon->SetResistance(SPELL_SCHOOL_ARCANE, 300);
					break;
				case NPC_RED_DRAKANOID :
					c1Dragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FIRE, true);
					break;
				case NPC_GREEN_DRAKANOID :
					c1Dragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NATURE, true);
					break;
				case NPC_BLACK_DRAKANOID :
					c1Dragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FIRE, true);
					c1Dragon->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, true);
					break;
			}

			cDragon->SetInCombatWithZone();
			c1Dragon->SetInCombatWithZone();

			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				cDragon->AI()->AttackStart(pTarget);

			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				c1Dragon->AI()->AttackStart(pTarget);
		}
    }

    bool isEndbossFight()
    {
        bool endbossFight = false;
        if (m_creature->HasAura(SPELL_NEFARIUS_BARRIER) && m_creature->getFaction() == FACTION_BLACK_DRAGON)
            endbossFight = true;

        return endbossFight;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
		{
			if(m_creature->isInCombat() == false)
				return;
		}

        if (isEndbossFight())
		{
            // Only do this if we haven't spawned nef yet
            if (m_uiSpawnedAddDeadCounter < MAX_DRAKE_SUMMONS)
            {
                // Shadowbolt Volley Timer
                if (m_uiShadowBoltVolleyTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SHADOWBOLT_VOLLEY) == CAST_OK)
                        m_uiShadowBoltVolleyTimer = urand(13000, 16000);
                }
                else
                    m_uiShadowBoltVolleyTimer -= uiDiff;

                // Silence Timer
                if (m_uiSilenceTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (pTarget->getPowerType() == POWER_MANA && pTarget->GetTypeId() == TYPEID_PLAYER)
                        {
                            m_creature->CastSpell(pTarget, SPELL_SILENCE, true);
                            m_uiSilenceTimer = urand(8000, 15000);
                        }
                    }
                }
                else
                    m_uiSilenceTimer -= uiDiff;

                 // Shadow Command Timer
                if (m_uiShadowCommandTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_COMMAND) == CAST_OK)
                            m_uiShadowCommandTimer = urand(15000, 25000);
                    }
                }
                else
                    m_uiShadowCommandTimer -= uiDiff;

                // Fear Timer
                if (m_uiFearTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_FEAR) == CAST_OK)
                            m_uiFearTimer = urand(15000, 20000);
                    }
                }
                else
                    m_uiFearTimer -= uiDiff;

                // Shadow Blink Timer
                if (m_uiShadowBlinkTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        m_creature->NearTeleportTo(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());
                        m_creature->CastSpell(m_creature, SPELL_SHADOWBLINK, true);
                        m_uiShadowBlinkTimer = urand(30000, 45000);
                        //get new shadowbolt target at new position
                        m_uiShadowBoltTimer = 0;
                    }
                }
                else
                    m_uiShadowBlinkTimer -= uiDiff;


                // spawning mechanism
                if (m_uiAddSpawnTimer < uiDiff)
                {
                    SpawnDragonWave();

                    m_uiAddSpawnTimer = 7000;
                }
                else
                    m_uiAddSpawnTimer -= uiDiff;

                //  cast Shadowbolt if nothing is to do
                if (m_uiShadowBoltTimer != 0)
                {
                    if (ShadowBoltPlayer)
                    {
                        if (DoCastSpellIfCan(ShadowBoltPlayer, SPELL_SHADOWBOLT) == CAST_OK)
                            m_uiShadowBoltTimer--;
                    }
                    else
                        m_uiShadowBoltTimer--;
                }
                else
                {
                    ShadowBoltPlayer = GetPlayerAtMinimumRange(30.0);
                    m_uiShadowBoltTimer = urand(1,3);
                }
			} else
			{
				if (!summonedDragonNefarian)
				{
					//Begin phase 2 by spawning Nefarian
					if (m_uiSpawnedAddDeadCounter >= MAX_DRAKE_SUMMONS)
					{
						if (m_uiPhase2Timer == 10000)
						{
							//Inturrupt any spell casting
							DoScriptText(SAY_BEGIN_PHASE2, m_creature);
							m_creature->InterruptNonMeleeSpells(false);

							//Make super invis
							if (m_creature->GetVisibility() != VISIBILITY_OFF)
								m_creature->SetVisibility(VISIBILITY_OFF);

							m_creature->NearTeleportTo(-7588.27f, -1261.92f, 482.03f, 0.610865f);

							m_creature->RemoveAura(SPELL_NEFARIUS_BARRIER, EFFECT_INDEX_0);
						}
					}
				}
			}
        } else
		{
			if (!summonedDragonNefarian)
			{
				//Begin phase 2 by spawning Nefarian
				if (m_uiSpawnedAddDeadCounter >= MAX_DRAKE_SUMMONS)
				{
					if (m_uiPhase2Timer <= uiDiff)
					{
						// Spawn Nefarian
						// Summon as active, to be able to work proper!
						m_creature->SummonCreature(NPC_NEFARIAN, aNefarianLocs[2].m_fX, aNefarianLocs[2].m_fY, aNefarianLocs[2].m_fZ, 0, TEMPSUMMON_MANUAL_DESPAWN, 0, true);
						summonedDragonNefarian = true;
					} else
						m_uiPhase2Timer -= uiDiff;
				}
			}
		}
    }
};

bool GossipHello_boss_victor_nefarius(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEFARIUS_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
	if(pPlayer->GetSession()->GetSecurity() > SEC_PLAYER)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<GameMaster> Phase 2", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
    pPlayer->SEND_GOSSIP_MENU(7134, pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_boss_victor_nefarius(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEFARIUS_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(7198, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEFARIUS_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(7199, pCreature->GetObjectGuid());
            DoScriptText(SAY_GAMESBEGIN_1, pCreature);
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            DoScriptText(SAY_GAMESBEGIN_2, pCreature);

            // remove gossip, set hostile, remove StandState, set rooted and attack
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            pCreature->setFaction(FACTION_BLACK_DRAGON);
            pCreature->CastSpell(pCreature, SPELL_NEFARIUS_BARRIER, false);
			pCreature->SetStandState(UNIT_STAND_STATE_STAND);
			pCreature->CastSpell(pCreature, SPELL_ROOT, true);

            pCreature->AI()->AttackStart(pPlayer);
            pCreature->SetInCombatWithZone();
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
			pCreature->SummonCreature(NPC_NEFARIAN, aNefarianLocs[2].m_fX, aNefarianLocs[2].m_fY, aNefarianLocs[2].m_fZ, 0, TEMPSUMMON_DEAD_DESPAWN, 0, true);
			break;
    }
    return true;
}

  ///////////////////////
 /////  PHASE TWO  /////
///////////////////////

enum
{
    SAY_XHEALTH                 = -1469008,             // at 5% hp
    SAY_SHADOWFLAME             = -1469009,
    SAY_RAISE_SKELETONS         = -1469010,
    SAY_SLAY                    = -1469011,
    SAY_DEATH                   = -1469012,

    SAY_MAGE                    = -1469013,
    SAY_WARRIOR                 = -1469014,
    SAY_DRUID                   = -1469015,
    SAY_PRIEST                  = -1469016,
    SAY_PALADIN                 = -1469017,
    SAY_SHAMAN                  = -1469018,
    SAY_WARLOCK                 = -1469019,
    SAY_HUNTER                  = -1469020,
    SAY_ROGUE                   = -1469021,

    SPELL_SHADOWFLAME_INITIAL   = 22986,
    SPELL_SHADOWFLAME           = 22539,
    SPELL_BELLOWING_ROAR        = 22686,
    SPELL_VEIL_OF_SHADOW        = 22687,                // old spell id 7068 -> wrong
    SPELL_CLEAVE                = 20691,             
    SPELL_TAILSWEEP	            = 23364,

    SPELL_MAGE                  = 23410,                // wild magic
    SPELL_WARRIOR               = 23397,                // beserk
    SPELL_DRUID                 = 23398,                // cat form
    SPELL_PRIEST                = 23401,                // corrupted healing
    SPELL_PALADIN               = 23418,                // syphon blessing
    SPELL_SHAMAN                = 23425,                // totems
    SPELL_WARLOCK               = 23427,                // infernals    -> should trigger 23426
    SPELL_HUNTER                = 23436,                // bow broke
    SPELL_ROGUE                 = 23414,                // Paralise

    NPC_BONE_CONSTRUCT			= 14605,
};

struct MANGOS_DLL_DECL boss_nefarianAI : public ScriptedAI
{
    uint32 team;

    boss_nefarianAI(Creature* pCreature) : ScriptedAI(pCreature), team(0)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_FIRE, true);

		std::vector<int> pair(2, 0);

		pair[0] = SPELL_MAGE; pair[1] = SAY_MAGE;
		Class.push_back(pair);

		pair[0] = SPELL_WARRIOR; pair[1] = SAY_WARRIOR;
		Class.push_back(pair);

		pair[0] = SPELL_DRUID; pair[1] = SAY_DRUID;
		Class.push_back(pair);

		pair[0] = SPELL_PRIEST; pair[1] = SAY_PRIEST;
		Class.push_back(pair);

		pair[0] = SPELL_WARLOCK; pair[1] = SAY_WARLOCK;
		Class.push_back(pair);

		pair[0] = SPELL_HUNTER; pair[1] = SAY_HUNTER;
		Class.push_back(pair);

		pair[0] = SPELL_ROGUE; pair[1] = SAY_ROGUE;
		Class.push_back(pair);

		pair[0] = SPELL_PALADIN; pair[1] = SAY_PALADIN;
		Class.push_back(pair);

		pair[0] = SPELL_SHAMAN; pair[1] = SAY_SHAMAN;
		Class.push_back(pair);

        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowFlameTimer;
    uint32 m_uiBellowingRoarTimer;
    uint32 m_uiVeilOfShadowTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiClassCallTimer;
	uint32 m_uiTailSweepTicker;

	std::vector<DrakonIdInfo> NefarianDragons;
	std::vector<Creature*> NefarianBoneConstructs;

    bool m_bPhase3;
    bool m_bHasEndYell;

	std::vector<std::vector<int32>> Class;

    void Reset()
    {
        m_uiShadowFlameTimer    = urand(10000, 15000);
        m_uiBellowingRoarTimer  = urand(28000, 32000);
        m_uiVeilOfShadowTimer   = urand(12000, 18000);
        m_uiCleaveTimer         = urand(5000, 9000);
        m_uiClassCallTimer      = urand(25000, 35000);
		m_uiTailSweepTicker		= 10000;
        m_bPhase3               = false;
        m_bHasEndYell           = false;

        DespawnBoneConstructs();
		m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_FLYING);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 4) == 0)
            return;

        DoScriptText(SAY_SLAY, m_creature, pVictim);
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
			m_pInstance->SetData(TYPE_NEFARIAN, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::BlackWingLair, Bosses::Nefarian);
    }

    void JustReachedHome()
	{
		NefarianDragons.clear();
		NefarianBoneConstructs.clear();
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_NEFARIAN, FAIL);

            // Cleanup encounter
            if (m_creature->IsTemporarySummon())
            {
                TemporarySummon* pTemporary = (TemporarySummon*)m_creature;

                if (Creature* pNefarius = m_creature->GetMap()->GetCreature(pTemporary->GetSummonerGuid()))
                    pNefarius->AI()->EnterEvadeMode();
            }
            m_creature->ForcedDespawn();
        }
    }

    void Aggro(Unit* pWho)
    {
		DoCastSpellIfCan(m_creature, SPELL_SHADOWFLAME_INITIAL, CAST_TRIGGERED);

		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
		m_creature->m_movementInfo.RemoveMovementFlag(MOVEFLAG_FLYING);
		m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_WALK_MODE);
    }

    void SpawnBoneConstructs()
    {
		for (auto& info : NefarianDragons)
        {
            Creature* cBoneConstruct = m_creature->SummonCreature(NPC_BONE_CONSTRUCT, info.x, info.y, info.z, 0, TEMPSUMMON_MANUAL_DESPAWN, 0);
            if (cBoneConstruct)
            {
                //source hordeguides classic
                cBoneConstruct->SetLevel(60);
                cBoneConstruct->SetMaxHealth(6104);
                cBoneConstruct->setFaction(FACTION_HOSTILE);

                cBoneConstruct->SetInCombatWithZone();
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    cBoneConstruct->AI()->AttackStart(pTarget);

                NefarianBoneConstructs.push_back(cBoneConstruct);
            }
        }

        NefarianDragons.clear();
    }

    void DespawnBoneConstructs()
    {
        for(std::vector<Creature*>::const_iterator it = NefarianBoneConstructs.begin(); it != NefarianBoneConstructs.end(); ++it)
        {
            if (*it)
            {
                (*it)->ForcedDespawn();
                (*it)->AddObjectToRemoveList();
            }
        }
        NefarianBoneConstructs.clear();
    }

    bool isPlayerInBack()
    {
        bool doTailSweep = false;

        if (m_pInstance)
        {
            Map::PlayerList const &PlayerList = m_pInstance->instance->GetPlayers();
            for(Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            {
                Player* pPlayer = itr->getSource();
                if (pPlayer && !pPlayer->isGameMaster() && pPlayer->isAlive() && m_creature->isInBack(pPlayer, 8.0f, 3.14159265359f))
                    doTailSweep = true;
            }
        }

        return doTailSweep;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //get team
        if (!team)
        {
            Player* tPlayer = GetPlayerAtMinimumRange(50.0);
            if (tPlayer)
            {
                if (tPlayer->GetTeam() == ALLIANCE)
                    team = TEAM_ALLIANCE;
                else
                {
                    team = TEAM_HORDE;
                    Class[7][0] = Class[8][0];
                    Class[7][1] = Class[8][1];
                }
            }
        }

        // ShadowFlame_Timer
        if (m_uiShadowFlameTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWFLAME) == CAST_OK)
            {
                m_uiShadowFlameTimer = urand(12000, 18000);

                if (urand(0, 2) == 0)
                    DoScriptText(SAY_SHADOWFLAME, m_creature);
            }
        }
        else
            m_uiShadowFlameTimer -= uiDiff;

        // BellowingRoar_Timer
        if (m_uiBellowingRoarTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BELLOWING_ROAR) == CAST_OK)
                m_uiBellowingRoarTimer = urand(28000, 32000);
        }
        else
            m_uiBellowingRoarTimer -= uiDiff;

        // VeilOfShadow_Timer
        if (m_uiVeilOfShadowTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO,0);
            if (pTarget)
            {
                if (DoCastSpellIfCan(pTarget, SPELL_VEIL_OF_SHADOW) == CAST_OK)
                    m_uiVeilOfShadowTimer = urand(12000, 18000);
            }
        }
        else
            m_uiVeilOfShadowTimer -= uiDiff;

        // Cleave_Timer
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(5000, 9000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Tail Sweep Timer
        if (m_uiTailSweepTicker < uiDiff)
        {
            if (isPlayerInBack())
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TAILSWEEP) == CAST_OK)
                    m_uiTailSweepTicker = 6000;
            }
            else
                m_uiTailSweepTicker = 3000;
        }
        else
            m_uiTailSweepTicker -= uiDiff;

        // ClassCall_Timer
        if (m_uiClassCallTimer < uiDiff)
        {
            //Cast a random class call
            //On official it is based on what classes are currently on the hostil list
            //but we can't do that yet so just randomly call one
             uint32 i = urand(0,7);
             if (DoCastSpellIfCan(m_creature, Class[i][0]) == CAST_OK)
             {
                 //telport rogues in front of nefarian
                 if ( i == 6)
                 {
                    Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO,0);
                    Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                    {
                        Player* pPlayer = itr->getSource();
                        if (pTarget && pPlayer && pPlayer->getClass() == CLASS_ROGUE)
                            DoTeleportPlayer(pPlayer, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());
                    }
                 }

                DoScriptText(Class[i][1], m_creature);
                m_uiClassCallTimer = urand(20000, 25000);
             }
        }
        else
            m_uiClassCallTimer -= uiDiff;

          ///////////////////////
         ///// PHASE THREE /////
        ///////////////////////
        if (!m_bPhase3 && m_creature->GetHealthPercent() <= 20.0f)
        {
            SpawnBoneConstructs();

            m_bPhase3 = true;
            DoScriptText(SAY_RAISE_SKELETONS, m_creature);
        }

        // 5% hp yell
        if (!m_bHasEndYell && m_creature->GetHealthPercent() < 5.0f)
        {
            m_bHasEndYell = true;
            DoScriptText(SAY_XHEALTH, m_creature);
        }

        DoMeleeAttackIfReady();
    }
};

void populateBoneConstructs(const std::vector<DrakonIdInfo>& dragons, CreatureAI* ai)
{
	auto nefAI = static_cast<boss_nefarianAI*>(ai);
	nefAI->NefarianDragons = dragons;
}

enum AddSpells
{
	SPELL_BROOD_POWER_BRONZE	= 22642,
	SPELL_BROOD_POWER_BLUE		= 22559,
	SPELL_BROOD_POWER_RED		= 22558,
	SPELL_BROOD_POWER_GREEN		= 22561,
	SPELL_BROOD_POWER_BLACK		= 22560,
	SPELL_TELEPORT				= 20477
};

struct MANGOS_DLL_DECL mob_chromatic_drakanoidAI : public ScriptedAI
{
    mob_chromatic_drakanoidAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

	uint32 m_uiBroodPowerTimer;

    void Reset()
    {
		m_uiBroodPowerTimer	= urand(6000, 10000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBroodPowerTimer < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROOD_POWER_BLACK) == CAST_OK)
			    m_uiBroodPowerTimer = urand(6000, 12000);
		}
		else
			m_uiBroodPowerTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_black_drakanoidAI : public ScriptedAI
{
    mob_black_drakanoidAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

	uint32 m_uiBroodPowerBlackTimer;
	uint32 m_uiTeleportToCaster;

    void Reset()
    {
		m_uiBroodPowerBlackTimer	= urand(4000, 6000);
		m_uiTeleportToCaster		= urand(10000, 15000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBroodPowerBlackTimer < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROOD_POWER_BLACK) == CAST_OK)
			    m_uiBroodPowerBlackTimer = urand(6000, 10000);
		}
		else
			m_uiBroodPowerBlackTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_blue_drakanoidAI : public ScriptedAI
{
    mob_blue_drakanoidAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

	uint32 m_uiBroodPowerTimerBlue;
	uint32 m_uiTeleportToCaster;

    void Reset()
    {
		m_uiBroodPowerTimerBlue	= urand(4000, 6000);
		m_uiTeleportToCaster	= urand(10000, 15000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBroodPowerTimerBlue < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROOD_POWER_BLUE) == CAST_OK)
			    m_uiBroodPowerTimerBlue = urand(6000, 10000);
		}
		else
			m_uiBroodPowerTimerBlue -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_red_drakanoidAI : public ScriptedAI
{
    mob_red_drakanoidAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

	uint32 m_uiBroodPowerTimerRed;
	uint32 m_uiTeleportToCaster;

    void Reset()
    {
		m_uiBroodPowerTimerRed	= urand(4000, 6000);
		m_uiTeleportToCaster	= urand(10000, 15000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBroodPowerTimerRed < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROOD_POWER_RED) == CAST_OK)
			    m_uiBroodPowerTimerRed = urand(6000, 10000);
		}
		else
			m_uiBroodPowerTimerRed -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_green_drakanoidAI : public ScriptedAI
{
    mob_green_drakanoidAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

	uint32 m_uiBroodPowerTimerGreen;
	uint32 m_uiTeleportToCaster;

    void Reset()
    {
		m_uiBroodPowerTimerGreen	= urand(4000, 6000);
		m_uiTeleportToCaster		= urand(10000, 15000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBroodPowerTimerGreen < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROOD_POWER_GREEN) == CAST_OK)
			    m_uiBroodPowerTimerGreen = urand(6000, 10000);
		}
		else
			m_uiBroodPowerTimerGreen -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_bronze_drakanoidAI : public ScriptedAI
{
    mob_bronze_drakanoidAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

	uint32 m_uiBroodPowerTimerBronze;

    void Reset()
    {
		m_uiBroodPowerTimerBronze	= urand(4000, 6000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBroodPowerTimerBronze < diff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BROOD_POWER_BRONZE) == CAST_OK)
			    m_uiBroodPowerTimerBronze = urand(6000, 10000);
		}
		else
			m_uiBroodPowerTimerBronze -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_victor_nefarius(Creature* pCreature)
{
    return new boss_victor_nefariusAI(pCreature);
}

CreatureAI* GetAI_boss_nefarian(Creature* pCreature)
{
    return new boss_nefarianAI(pCreature);
}

CreatureAI* GetAI_mob_chromatic_drakanoid(Creature* pCreature)
{
    return new mob_chromatic_drakanoidAI(pCreature);
}

CreatureAI* GetAI_mob_bronze_drakanoid(Creature* pCreature)
{
    return new mob_bronze_drakanoidAI(pCreature);
}

CreatureAI* GetAI_mob_black_drakanoid(Creature* pCreature)
{
    return new mob_black_drakanoidAI(pCreature);
}

CreatureAI* GetAI_mob_blue_drakanoid(Creature* pCreature)
{
    return new mob_blue_drakanoidAI(pCreature);
}

CreatureAI* GetAI_mob_red_drakanoid(Creature* pCreature)
{
    return new mob_red_drakanoidAI(pCreature);
}

CreatureAI* GetAI_mob_green_drakanoid(Creature* pCreature)
{
    return new mob_green_drakanoidAI(pCreature);
}

struct MANGOS_DLL_DECL totemAI: public ScriptedAI
{
	totemAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		switch(pCreature->GetEntry())
		{
		case 14662: //corrupted fire nova totem
			mCastTimer = 5000;
			mRecastTimer = -1;
			mDespawnTimer = 5500;
			mSpell = m_creature->m_spells[0];
			break;
		case 14663: //corrupted stoneskin totem
		case 14664: //corrupted healing stream totem
			mCastTimer = 0;
			mRecastTimer = -1;
			mDespawnTimer = 60000;
			mSpell = m_creature->m_spells[0];
			break;
		case 14666: //corrupted windfury totem
			mCastTimer = 0;
			mRecastTimer = -1;
			mDespawnTimer = 60000;
			mSpell = m_creature->m_spells[1];
		}

		m_creature->SetCanChase(false);
		m_creature->SetCanAttackPlayer(false);
	}

	uint32 mCastTimer = 0;
	uint32 mRecastTimer = 0;
	uint32 mDespawnTimer = 0;
	uint32 mSpell = 0;

	void Reset()
	{
	}

	void EnterEvadeMode()
	{
		static_cast<TemporarySummon*>(m_creature)->UnSummon();
	}

	void UpdateAI(const uint32 diff)
	{
		m_creature->SelectHostileTarget();
		m_creature->SetCanChase(false);
		m_creature->SetCanAttackPlayer(false);

		if(mCastTimer <= diff)
		{
			if (mSpell != 0)
				m_creature->CastSpell(m_creature, mSpell, true);
			mCastTimer = mRecastTimer;
		} else
			mCastTimer -= diff;

		if (mDespawnTimer <= diff)
			static_cast<TemporarySummon*>(m_creature)->UnSummon();
		else
			mDespawnTimer -= diff;
	}
};

void AddSC_boss_nefarian()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_victor_nefarius";
    pNewScript->GetAI = &GetAI_boss_victor_nefarius;
    pNewScript->pGossipHello = &GossipHello_boss_victor_nefarius;
    pNewScript->pGossipSelect = &GossipSelect_boss_victor_nefarius;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_nefarian";
    pNewScript->GetAI = &GetAI_boss_nefarian;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_chromatic_drakanoid";
    pNewScript->GetAI = &GetAI_mob_chromatic_drakanoid;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_black_drakanoid";
    pNewScript->GetAI = &GetAI_mob_black_drakanoid;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_blue_drakanoid";
    pNewScript->GetAI = &GetAI_mob_blue_drakanoid;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_red_drakanoid";
    pNewScript->GetAI = &GetAI_mob_red_drakanoid;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_green_drakanoid";
    pNewScript->GetAI = &GetAI_mob_green_drakanoid;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "mob_bronze_drakanoid";
	pNewScript->GetAI = &GetAI_mob_bronze_drakanoid;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "Corrupted_Totem_AI";
	pNewScript->GetAI = [](Creature* c) -> CreatureAI* { return new totemAI(c); };
	pNewScript->RegisterSelf();
}
