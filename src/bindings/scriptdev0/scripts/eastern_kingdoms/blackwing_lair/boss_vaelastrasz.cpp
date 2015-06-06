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
SDName: Boss_Vaelastrasz
SD%Complete: 100
SDComment:
- 30% health always
- emote when he got corrupted
- new orientation after speech

SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"
#include "../system/ProgressMonitor.h"

enum
{
	SAY_LINE_1 = -1469026,
	SAY_LINE_2 = -1469027,
	SAY_LINE_3 = -1469028,
	SAY_HALFLIFE = -1469029,
	SAY_KILLTARGET = -1469030,
	SAY_NEFARIUS_CORRUPT_1 = -1469006,                 // When he corrupts Vaelastrasz
	SAY_NEFARIUS_CORRUPT_2 = -1469031,
	SAY_TECHNICIAN_RUN = -1469033,

	SPELL_ESSENCE_OF_THE_RED = 23513,
	SPELL_FLAME_BREATH = 23461,
	SPELL_FIRE_NOVA = 23462,
	SPELL_TAIL_SWEEP = 15847,
	SPELL_BURNING_ADRENALINE = 23620,
	SPELL_CLEAVE = 19983,                    // Chain cleave is most likely named something different and contains a dummy effect
	SPELL_ROOT = 31366,

	SPELL_NEFARIUS_CORRUPTION = 23642,

	GOSSIP_ITEM_VAEL_1 = -3469003,
	GOSSIP_ITEM_VAEL_2 = -3469004,

	GOSSIP_TEXT_VAEL_1 = 384,
	GOSSIP_TEXT_VAEL_2 = 384,

	FACTION_HOSTILE = 14,

	AREATRIGGER_VAEL_INTRO = 3626,
};

std::list<Creature*> technicianArroundVael;
std::list<Creature*> GoblinDespawner;

struct MANGOS_DLL_DECL boss_vaelastraszAI : public ScriptedAI
{
	boss_vaelastraszAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		// Set stand state to dead before the intro event
		m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
		m_creature->setFaction(35);
		m_creature->SetHealthPercent(100.0f);
		//start orientation while he is laying on the ground
		m_creature->SetOrientation(4.9f);
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
		//player can hit at
		m_creature->SetBoundingValue(0, 13);
		//boss can hit at:
		m_creature->SetBoundingValue(1, 11);
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

	ObjectGuid m_nefariusGuid;
	ObjectGuid m_playerGuid;

	uint8 m_uiIntroPhase;
	uint8 m_uiAdrenalineTargetCount;
	uint8 m_uiSpeechNum;

	uint32 m_uiSpeechTimer;
	uint32 m_uiIntroTimer;
	uint32 m_uiCleaveTimer;
	uint32 m_uiFlameBreathTimer;
	uint32 m_uiFireNovaTimer;
	uint32 m_uiBurningAdrenalineCasterTimer;
	uint32 m_uiTailSweepTicker;

	bool m_bHasYelled;
	bool triggerd;
	bool speech;
	bool debuff;

	Creature * pNef;

	void Reset()
	{
		m_playerGuid.Clear();
		m_uiIntroTimer						= 0;
		m_uiIntroPhase						= 0;
		m_uiAdrenalineTargetCount			= 1;
		m_uiSpeechTimer						= 12000;
		m_uiSpeechNum						= 0;
		m_uiCleaveTimer						= 4000;
		m_uiFlameBreathTimer				= 5000;
		m_uiBurningAdrenalineCasterTimer	= 15000;
		m_uiFireNovaTimer					= 3000;
		m_uiTailSweepTicker					= 10000;

		m_bHasYelled						= false;
		triggerd							= false;
		speech								= false;
		debuff								= false;
		pNef								= 0;

		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
	}

	void BeginIntro()
	{
		// Start Intro delayed
		m_uiIntroTimer = 5000;
		m_uiIntroPhase = 0;
		triggerd = true;

		if (m_pInstance)
			m_pInstance->SetData(TYPE_VAELASTRASZ, SPECIAL);
	}

	void BeginSpeech(Player* pTarget)
	{
		speech = true;

		// remove intro flags
		SetCombatMovement(true);
		triggerd = false;

		//remove gossip
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

		// Stand up and begin speach
		m_playerGuid = pTarget->GetObjectGuid();

		// 10 seconds
		DoScriptText(SAY_LINE_1, m_creature);

		// Make boss stand
		m_creature->SetStandState(UNIT_STAND_STATE_STAND);

		//new orientation - dont work
		m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 2.1f);
		m_creature->SetOrientation(2.1f);

		//at this time we can hide the event goblins
		for (std::list<Creature*>::iterator itr = technicianArroundVael.begin(); itr != technicianArroundVael.end(); ++itr)
		{
			if ((*itr))
			{
				(*itr)->SetVisibility(VISIBILITY_OFF);
				(*itr)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE);
				(*itr)->SetDefaultMovementType(IDLE_MOTION_TYPE);
				(*itr)->GetMotionMaster()->Initialize();
			}
		}
	}

	void KilledUnit(Unit* pVictim)
	{
		if (urand(0, 4))
			return;

		DoScriptText(SAY_KILLTARGET, m_creature, pVictim);
	}

	void Aggro(Unit* pWho)
	{
		if (!triggerd)
		{
			if (m_pInstance)
				m_pInstance->SetData(TYPE_VAELASTRASZ, IN_PROGRESS);

			m_creature->SetStandState(UNIT_STAND_STATE_STAND);
		}
	}

	void JustDied(Unit* killer)
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_VAELASTRASZ, DONE);

		using namespace ProgressMonitor;
		if (killer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(killer), Raids::BlackWingLair, Bosses::Vaelstrasz);
	}

	void JustReachedHome()
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_VAELASTRASZ, FAIL);

		m_creature->SetHealthPercent(30);
	}

	bool isPlayerInBack()
	{
		bool doTailSweep = false;

		if (m_pInstance)
		{
			Map::PlayerList const &PlayerList = m_pInstance->instance->GetPlayers();
			for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
			{
				Player* pPlayer = itr->getSource();
				if (pPlayer && !pPlayer->isGameMaster() && pPlayer->isAlive() && m_creature->isInBack(pPlayer, 10.0f, 3.14159265359f))
					doTailSweep = true;
			}
		}

		return doTailSweep;
	}

	Player* DoSelectRandomNonMeleePlayer()
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
					if (pTarget->GetTypeId() != TYPEID_PLAYER)
						continue;

					// We need only a player
					if (!pTarget->IsCharmerOrOwnerPlayerOrPlayerItself())
						continue;

					if (pTarget->getPowerType() == POWER_MANA && !pTarget->HasAura(23620))
						// a player with mana power
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

	void UpdateAI(const uint32 uiDiff)
	{
		if (triggerd)
		{
			if (m_uiIntroTimer)
			{
				if (m_uiIntroTimer <= uiDiff)
				{
					switch (m_uiIntroPhase)
					{
					case 0:
						pNef = m_creature->SummonCreature(NPC_LORD_VICTOR_NEFARIUS, aNefariusSpawnLoc[0], aNefariusSpawnLoc[1], aNefariusSpawnLoc[2], aNefariusSpawnLoc[3], TEMPSUMMON_MANUAL_DESPAWN, 0);
						pNef->SetStandState(UNIT_STAND_STATE_STAND);
						pNef->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						m_uiIntroTimer = 1000;
						break;
					case 1:
						if (pNef)
						{
							SetCombatMovement(false);
							pNef->AI()->AttackStart(m_creature);
							pNef->CastSpell(m_creature, SPELL_NEFARIUS_CORRUPTION, false);
							DoScriptText(SAY_NEFARIUS_CORRUPT_1, pNef);
							m_uiIntroTimer = 35000;
						}
						break;
					case 2:
						if (pNef)
							DoScriptText(SAY_NEFARIUS_CORRUPT_2, pNef);
						m_uiIntroTimer = 5000;
						break;
					case 3:
						if (pNef)
						{
							//teleportspell
							pNef->CastSpell(pNef, 22664, true);
							m_uiIntroTimer = 2000;
						}
						break;
					case 4:
						if (pNef)
						{
							//set vail health and no health reg
							m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
							m_creature->SetHealthPercent(30);
							//despawn nef
							pNef->ForcedDespawn();
							pNef->AddObjectToRemoveList();
							// Set npc flags now
							m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
							m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

							if (m_pInstance)
								m_pInstance->SetData(TYPE_VAELASTRASZ, SPECIAL);

							m_uiIntroTimer = 0;
							return;
						}
						break;
					}
					++m_uiIntroPhase;
				}
				else
					m_uiIntroTimer -= uiDiff;
			}
		}
		else
		{
			// Speech
			if (speech)
			{
				if (m_uiSpeechTimer)
				{
					if (m_uiSpeechTimer <= uiDiff)
					{
						switch (m_uiSpeechNum)
						{
						case 0:
							// 16 seconds till next line
							DoScriptText(SAY_LINE_2, m_creature);
							m_uiSpeechTimer = 16000;
							++m_uiSpeechNum;
							break;
						case 1:
							// This one is actually 16 seconds but we only go to 10 seconds because he starts attacking after he says "I must fight this!"
							DoScriptText(SAY_LINE_3, m_creature);
							m_uiSpeechTimer = 10000;
							++m_uiSpeechNum;
							break;
						case 2:
							//dont work - we have to fix that
							m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
							m_creature->MonsterTextEmote(TEXTEMOTE_ROAR, m_creature);

							m_uiSpeechTimer = 1500;
							++m_uiSpeechNum;
						case 3:
							m_creature->setFaction(FACTION_HOSTILE);

							if (m_playerGuid)
							{
								if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
									AttackStart(pPlayer);
							}
							speech = false;
							m_uiSpeechTimer = 0;
							break;
						}
					}
					else
						m_uiSpeechTimer -= uiDiff;
				}
			}
			else
			{
				// Return since we have no target
				if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
					return;

				if (!debuff)
				{
					// Buff players on aggro, void aggro wont work because of the event!
					m_creature->CastSpell(m_creature, SPELL_ESSENCE_OF_THE_RED, true);
					m_creature->SetHealthPercent(30);
					debuff = true;
				}

				// Yell if hp lower than 15%
				if (m_creature->GetHealthPercent() <= 15.0f && !m_bHasYelled)
				{
					DoScriptText(SAY_HALFLIFE, m_creature);
					m_bHasYelled = true;
				}

				// Cleave Timer
				if (m_uiCleaveTimer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature, SPELL_CLEAVE) == CAST_OK)
						m_uiCleaveTimer = urand(8000, 12000);
				}
				else
					m_uiCleaveTimer -= uiDiff;

				// Fire Nova Timer
				if (m_uiFireNovaTimer < uiDiff)
				{
					m_creature->CastSpell(m_creature, SPELL_FIRE_NOVA, true);
					m_uiFireNovaTimer = 3000;
				}
				else
					m_uiFireNovaTimer -= uiDiff;

				// Burning Adrenaline Caster Timer
				if (m_uiBurningAdrenalineCasterTimer < uiDiff)
				{
					switch (m_uiAdrenalineTargetCount % 3)
					{
						case 0:
						{
							auto target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0);

							if (target)
								m_creature->CastSpell(target, SPELL_BURNING_ADRENALINE, true);

							m_uiBurningAdrenalineCasterTimer = 15000;
						} break;
						default:
						{
							auto target = DoSelectRandomNonMeleePlayer();

							if (target)
								m_creature->CastSpell(target, SPELL_BURNING_ADRENALINE, true);

						} break;
					}

					m_uiBurningAdrenalineCasterTimer = 15000;
					++m_uiAdrenalineTargetCount;
				}
				else
					m_uiBurningAdrenalineCasterTimer -= uiDiff;

				// Tail Sweep Timer
				if (m_uiTailSweepTicker < uiDiff)
				{
					if (isPlayerInBack())
					{
						if (DoCastSpellIfCan(m_creature, SPELL_TAIL_SWEEP) == CAST_OK)
							m_uiTailSweepTicker = 6000;
					}
					else
						m_uiTailSweepTicker = 3000;
				}
				else
					m_uiTailSweepTicker -= uiDiff;

				// Flame Breath Timer
				if (m_uiFlameBreathTimer < uiDiff)
				{
					if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_BREATH) == CAST_OK)
						m_uiFlameBreathTimer = 4000;
				}
				else
					m_uiFlameBreathTimer -= uiDiff;

				DoMeleeAttackIfReady();
			}
		}
	}
};

bool GossipSelect_boss_vaelastrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VAEL_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_VAEL_2, pCreature->GetObjectGuid());
		break;
	case GOSSIP_ACTION_INFO_DEF + 2:
		pPlayer->CLOSE_GOSSIP_MENU();
		if (boss_vaelastraszAI* pVaelAI = dynamic_cast<boss_vaelastraszAI*>(pCreature->AI()))
			pVaelAI->BeginSpeech(pPlayer);
		break;
	}

	return true;
}

bool GossipHello_boss_vaelastrasz(Player* pPlayer, Creature* pCreature)
{
	if (pCreature->isQuestGiver())
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

	pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VAEL_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_VAEL_1, pCreature->GetObjectGuid());

	return true;
}

bool AreaTrigger_at_vaelastrasz(Player* pPlayer, AreaTriggerEntry const* pAt)
{
	instance_blackwing_lair* pInstance = (instance_blackwing_lair*)pPlayer->GetInstanceData();

	if (pAt->id == AREATRIGGER_VAEL_INTRO && pInstance->GetData(TYPE_RAZORGORE) == DONE && pInstance->GetData(TYPE_VAELASTRASZ) != SPECIAL)
	{
		if (pPlayer->isGameMaster() || pPlayer->isDead())
			return false;

		if (pInstance->GetData(TYPE_VAELASTRASZ) == SPECIAL)
		{
			if (Creature* pVaelastrasz = pInstance->GetSingleCreatureFromStorage(NPC_VAELASTRASZ))
			{
				pVaelastrasz->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
				pVaelastrasz->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
			}
		}

		if (instance_blackwing_lair* pInstance = (instance_blackwing_lair*)pPlayer->GetInstanceData())
		{
			// Handle intro event
			if (pInstance->GetData(TYPE_VAELASTRASZ) == NOT_STARTED)
			{
				if (Creature* pVaelastrasz = pInstance->GetSingleCreatureFromStorage(NPC_VAELASTRASZ))
				{
					if (boss_vaelastraszAI* pVaelAI = dynamic_cast<boss_vaelastraszAI*>(pVaelastrasz->AI()))
						pVaelAI->BeginIntro();

					GetCreatureListWithEntryInGrid(technicianArroundVael, pVaelastrasz, NPC_BLACKWING_TECHNICIAN, 18.0f);
					for(std::list<Creature*>::iterator itr = technicianArroundVael.begin(); itr != technicianArroundVael.end(); ++itr)
					{
						if ((*itr)->isAlive())
						{
							(*itr)->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
							(*itr)->SetSpeedRate(MOVE_RUN, 1.4f);
							(*itr)->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
							(*itr)->GetMotionMaster()->Initialize();

							(*itr)->ForcedDespawn(120000);
						}
					}
				}
			}
		}
	}

	return false;
}

CreatureAI* GetAI_boss_vaelastrasz(Creature* pCreature)
{
	return new boss_vaelastraszAI(pCreature);
}

void AddSC_boss_vaelastrasz()
{
	Script* pNewScript;

	pNewScript = new Script;
	pNewScript->Name = "boss_vaelastrasz";
	pNewScript->GetAI = &GetAI_boss_vaelastrasz;
	pNewScript->pGossipHello = &GossipHello_boss_vaelastrasz;
	pNewScript->pGossipSelect = &GossipSelect_boss_vaelastrasz;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "at_vaelastrasz";
	pNewScript->pAreaTrigger = &AreaTrigger_at_vaelastrasz;
	pNewScript->RegisterSelf();
}