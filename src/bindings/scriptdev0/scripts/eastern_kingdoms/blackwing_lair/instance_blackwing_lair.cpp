/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Instance_Blackwing_Lair
SD%Complete: 90
SDComment:
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"

instance_blackwing_lair::instance_blackwing_lair(Map* pMap) : ScriptedInstance(pMap),
	m_uiRazorgoreSummonTimer(TIMER_START_SPAWMING_ADDS),
    m_uiDragonkinSummoned(0),
    m_uiOrcSummoned(0),
	m_uiChromaggusPullTimer(0),

	m_uiRazorgorManagerTimer(1),
	m_uiVealManagerTimer(1),
	m_uiLashLayerManagerTimer(1),
	m_uiChromaggusManagerTimer(1),
	m_uiGuardsmanManagerTimer(1),

	m_uiGuardsmanManager(true),
	m_uiGrethokManager(true),
	m_uiRazorgorManager(true),
	m_uiVealManager(true),
	m_uiLashLayerManager(true),
	m_uiChromaggusManager(true)
{
    Initialize();
}

void instance_blackwing_lair::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

    m_lBlackwingGuardsman.clear();
    m_lDragonEggsGuids.clear();
    m_lDragonTrio.clear();
}
/*
enum {
	SLOW_CHAMBER_SPAWN1 = 12468, 
	SLOW_CHAMBER_SPAWN2 = 14022, 
	SLOW_CHAMBER_SPAWN3 = 14023, 
	SLOW_CHAMBER_SPAWN4 = 14024, 
	SLOW_CHAMBER_SPAWN5 = 14025, 
	SLOW_CHAMBER_SPAWN6 = 12458
};

void instance_blackwing_lair::OnCreatureRespawn(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
		case SLOW_CHAMBER_SPAWN1:
		case SLOW_CHAMBER_SPAWN2:
		case SLOW_CHAMBER_SPAWN3:
		case SLOW_CHAMBER_SPAWN4:
		case SLOW_CHAMBER_SPAWN5:
		case SLOW_CHAMBER_SPAWN6:
			if(GetData(TYPE_LASHLAYER) == DONE)
			{
				pCreature->SetRespawnTime(-1);
				pCreature->ForcedDespawn();
			}
			break;
	}
}
*/
void instance_blackwing_lair::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_BLACKWING_GUARDSMAN:
			if (GetData(TYPE_RAZORGORE) == DONE)
				pCreature->ForcedDespawn();
            m_lBlackwingGuardsman.push_back(pCreature->GetObjectGuid());
            return;
		case 12457: //Blackwing Spellbinder
			pCreature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_MAGIC, true);
        case NPC_BLACKWING_TECHNICIAN:
			// Um das abfarmen der Goblins zu verhindern. Mage durch das Tor blinzeln und pullen.
			if (GetData(TYPE_RAZORGORE) != DONE)
			{
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
			}
            // hide event vael goblins if we enter instance and vael is already defeated
            if (pCreature->IsWithinDist2d(aNefariusSpawnLoc[0], aNefariusSpawnLoc[1], 50.0f) && GetData(TYPE_VAELASTRASZ) == DONE)
            {
                pCreature->SetVisibility(VISIBILITY_OFF);
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE);
            }
			m_lTechnicianAndOther.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_BLACKWING_LEGIONNAIRE:
        case NPC_BLACKWING_MAGE:
            m_lDefendersGuids.push_back(pCreature->GetObjectGuid());
			break;
        case NPC_DEATH_TALON_DRAGONSPAWN:
            m_lDefendersGuids.push_back(pCreature->GetObjectGuid());
            break;
        case NPC_CHROMAGGUS:
            // VALUE_BREATH1, VALUE_BREATH2 and so forth are the ID-specific spells
            if (GetData(VALUE_BREATH1) == NOT_STARTED && GetData(VALUE_BREATH2) == NOT_STARTED)
                SetData(VALUE_BREATH1, DONE);
            break;
        case NPC_BLUE_DRAGON:
		case NPC_BRONZE_DRAGON:
		case NPC_RED_DRAGON:
		case NPC_GREEN_DRAGON:
			if (GetData(TYPE_LASHLAYER) == DONE)
			{
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
			}
			m_lLashDragons.push_back(pCreature->GetObjectGuid());
			break;
		case NPC_DRAGONKIN_LASH:
		case NPC_PRIEST_LASH:
			if (GetData(TYPE_LASHLAYER) == DONE)
			{
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
			}
			m_lLashDragons.push_back(pCreature->GetObjectGuid());
			break;
		case NPC_SUPPRESSION_TRIGGER:
			if (GetData(TYPE_LASHLAYER) == DONE)
			{
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
			}
            m_lLashDragons.push_back(pCreature->GetObjectGuid());
            break;
		case NPC_BLACKWING_AUFSEHER:
		case NPC_FLAMMENSCHUPPE:
		case NPC_SCHNAUBER:
		case NPC_WYRMKIN:
		case NPC_HAUPTMANN:
			if (GetData(TYPE_RAZORGORE) != DONE)
			{
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
			}
			m_lTechnicianAndOther.push_back(pCreature->GetObjectGuid());
			return;
		case NPC_GRETHOK_THE_CONTROLLER:
		case NPC_NEFARIANS_TROOPS:
		case NPC_BLACKWING_ORB_TRIGGER:
        case NPC_RAZORGORE:
			if (GetData(TYPE_RAZORGORE) == DONE)
				DespawnEggs();
			break;
        case NPC_VAELASTRASZ:
			break;
		case NPC_LASHLAYER:
			if (GetData(TYPE_LASHLAYER) == DONE)
				DespawnLashlayerRoom();
			break;
		case NPC_NEFARIAN:
            break;
		case NPC_BRONZE_DRAKANOID:
		case NPC_BLUE_DRAKANOID:
		case NPC_RED_DRAKANOID:
		case NPC_GREEN_DRAKANOID:
		case NPC_BLACK_DRAKANOID:
			m_lNefDragonsP1Guids.push_back(pCreature->GetObjectGuid());
			break;
    }
	m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_blackwing_lair::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_BLACK_DRAGON_EGG:
            m_lDragonEggsGuids.push_back(pGo->GetObjectGuid());
            return;
        case GO_PORTCULLIS_ENTRANCE:
            break;
        case GO_DOOR_NEFARIAN:
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORTCULLIS_RAZORGORE:
            if (m_auiEncounter[TYPE_RAZORGORE] == DONE)
				pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORTCULLIS_NEFARIAN:
		case GO_PORTCULLIS_CHROMAGGUS:
            if (m_auiEncounter[TYPE_CHROMAGGUS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORTCULLIS_VAELASTRASZ:
            if (m_auiEncounter[TYPE_VAELASTRASZ] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PORTCULLIS_BROODLORD:
            if (m_auiEncounter[TYPE_LASHLAYER] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DRAKONID_BONES:
            m_lDrakonidBonesGuids.push_back(pGo->GetObjectGuid());
            return;
		case GO_SUPPRESSION_DEVICE:
			m_lSuppressionDeviceGuids.push_back(pGo->GetObjectGuid());
			return;
        case GO_CHROMAGGUS_LEVER:
            pGo->SetObjectScale(3.0f);
            if (m_auiEncounter[TYPE_CHROMAGGUS] == DONE)
                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        case GO_ORB_OF_DOMINATION:
			pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
            break;
        default:
            return;
    }

    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_blackwing_lair::SetData64(uint32 uiData, uint64 uiGuid)
{
    if (uiData == DATA_DRAGON_EGG)
    {
        if (GameObject* pEgg = instance->GetGameObject(ObjectGuid(uiGuid)))
            m_lUsedEggsGuids.push_back(pEgg->GetObjectGuid());

        // If all eggs are destroyed, then allow Razorgore to be attacked
        if (m_lUsedEggsGuids.size() == m_lDragonEggsGuids.size())
        {
				SetData(TYPE_RAZORGORE, SPECIAL);

                if (GameObject* Orb = GetSingleGameObjectFromStorage(GO_ORB_OF_DOMINATION))
                    Orb->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);

				if (Creature* pTrigger = GetSingleCreatureFromStorage(NPC_NEFARIANS_TROOPS))
				{
					DoScriptText(EMOTE_ORB_SHUT_OFF, pTrigger);
					DoScriptText(EMOTE_TROOPS_FLEE, pTrigger);
				}

				for (GuidList::const_iterator itr = m_lDefendersGuids.begin(); itr != m_lDefendersGuids.end(); ++itr)
				{
					if (Creature* pDefender = instance->GetCreature(*itr))
					{
						pDefender->SetFlag(UNIT_NPC_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
						pDefender->setFaction(35);
						pDefender->DeleteThreatList();
						pDefender->CombatStop();
						pDefender->SetCanAttackPlayer(false);
						pDefender->AI()->EnterEvadeMode();
						pDefender->ForcedDespawn(15000);
					}
				}

                if (Creature* pRazorgore = GetSingleCreatureFromStorage(NPC_RAZORGORE))
                {
                    pRazorgore->SetHealthPercent(100.0f);

                    if (Unit* pController = pRazorgore->GetCharmerOrOwner())
                    {
						pController->CastStop();
						pController->RemoveAurasDueToSpell(SPELL_MIND_EXHAUSTION);
                        pController->RemoveAurasDueToSpell(SPELL_USE_DRAGON_ORB);
                        pRazorgore->RemoveAurasDueToSpell(SPELL_USE_DRAGON_ORB);
                        pRazorgore->AI()->AttackStart(pController);
                        pRazorgore->TauntApply(pController);
						pRazorgore->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.20f);
                    }
                }

				m_lDefendersGuids.clear();
		}
            else
                debug_log("BWL: Black Dragon Eggs left to destroy: %u", m_lDragonEggsGuids.size());
    }
}

void instance_blackwing_lair::OnObjectUse(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CHROMAGGUS_LEVER:
            if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_PORTCULLIS_CHROMAGGUS))
            {
                pGo->SetGoState(GO_STATE_ACTIVE);
                m_uiChromaggusPullTimer = 2000;
            }
            break;
        default:
            break;
    }
}

void instance_blackwing_lair::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_RAZORGORE:
            m_auiEncounter[uiType] = uiData;
            if (uiData != SPECIAL)
                HandleGameObject(GO_PORTCULLIS_ENTRANCE, true);
            if (uiData == IN_PROGRESS)
                HandleGameObject(GO_PORTCULLIS_ENTRANCE, false);
            if (uiData == DONE)
			{
				HandleGameObject(GO_PORTCULLIS_ENTRANCE, true);
				HandleGameObject(GO_PORTCULLIS_RAZORGORE, true);

				DespawnEggs();

				if (Creature* pGrethok = GetSingleCreatureFromStorage(NPC_GRETHOK_THE_CONTROLLER))
					pGrethok->ForcedDespawn();

				for(auto& creature : m_lBlackwingGuardsman)
				{
					if (Creature* pGuardsman = instance->GetCreature(creature))
						pGuardsman->ForcedDespawn();
				}

				for (GuidList::const_iterator itr = m_lTechnicianAndOther.begin(); itr != m_lTechnicianAndOther.end(); ++itr)
				{
					if (Creature* pTechnicianAndOther = instance->GetCreature(*itr))
					{
						pTechnicianAndOther->SetVisibility(VISIBILITY_ON);
						pTechnicianAndOther->setFaction(103);
					}
				}
			}
            if (uiData == FAIL)
            {
				HandleGameObject(GO_PORTCULLIS_ENTRANCE, true);

				for (GuidList::const_iterator itr = m_lDragonEggsGuids.begin(); itr != m_lDragonEggsGuids.end(); ++itr)
				{
					if (GameObject* pEgg = instance->GetGameObject(*itr))
					{
						if (!pEgg->isSpawned())
							pEgg->Respawn();
					}
				}

                for (GuidList::const_iterator itr = m_lDefendersGuids.begin(); itr != m_lDefendersGuids.end(); ++itr)
                {
                    if (Creature* pDefender = instance->GetCreature(*itr))
                        pDefender->ForcedDespawn();
                }

				for(auto& creature : m_lBlackwingGuardsman)
				{
					if (Creature* guardsman = instance->GetCreature(creature))
						guardsman->Respawn();
				}

				if (Creature* pGrethok = GetSingleCreatureFromStorage(NPC_GRETHOK_THE_CONTROLLER))
				{
					if (pGrethok->isAlive())
						pGrethok->ForcedDespawn();

					if(pGrethok->isDead())
						pGrethok->Respawn();
				}

				if (GameObject* Orb = GetSingleGameObjectFromStorage(GO_ORB_OF_DOMINATION))
					Orb->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);

				// Respawn Razorgore
				if (Creature* pRazorgore = GetSingleCreatureFromStorage(NPC_RAZORGORE))
					pRazorgore->Respawn();

                for (GuidList::const_iterator itr = m_lLashDragons.begin(); itr != m_lLashDragons.end(); ++itr)
                {
                    if (Creature* pLashDragons = instance->GetCreature(*itr))
                        pLashDragons->Respawn();
                }

				for (GuidList::const_iterator itr = m_lTechnicianAndOther.begin(); itr != m_lTechnicianAndOther.end(); ++itr)
				{
					if (Creature* pTechnicianAndOther = instance->GetCreature(*itr))
						pTechnicianAndOther->Respawn();
				}

				m_uiRazorgoreSummonTimer = TIMER_START_SPAWMING_ADDS;
				m_uiDragonkinSummoned = 0;
				m_uiOrcSummoned = 0;

				m_lUsedEggsGuids.clear();
                m_lDefendersGuids.clear();
            }
            break;
        case TYPE_VAELASTRASZ:
            m_auiEncounter[uiType] = uiData;
            // Prevent the players from running back to the first room; use if the encounter is not special
            if (uiData != SPECIAL)
                HandleGameObject(GO_PORTCULLIS_RAZORGORE, true);
			else
			{
				int i = 0;
				for(auto& guid : m_lTechnicianGuids)
				{
					if(auto* technician = instance->GetCreature(guid))
					{
						technician->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
						technician->GetMotionMaster()->MovePoint(0, -7518.99f, -974.00f, 423.42f);
					}
				}
			}

            if (uiData == DONE)
			{
                HandleGameObject(GO_PORTCULLIS_RAZORGORE, true);
                HandleGameObject(GO_PORTCULLIS_VAELASTRASZ, true);
			}
            break;
        case TYPE_LASHLAYER:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
			{
				HandleGameObject(GO_PORTCULLIS_BROODLORD, true);

				for (GuidList::const_iterator itr = m_lLashDragons.begin(); itr != m_lLashDragons.end(); ++itr)
				{
					if (Creature* pDragonsLashSpawn = instance->GetCreature(*itr))
					{
						if (pDragonsLashSpawn)
						{
							pDragonsLashSpawn->setFaction(35);
							pDragonsLashSpawn->DeleteThreatList();
							pDragonsLashSpawn->SetVisibility(VISIBILITY_OFF);
							pDragonsLashSpawn->setAttackEnabled(false);
						}
					}
				}
			}
            break;
        case TYPE_FIREMAW:
        case TYPE_EBONROC:
        case TYPE_FLAMEGOR:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                if (Creature* pChromaggus = GetSingleCreatureFromStorage(NPC_CHROMAGGUS))
                    pChromaggus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
            break;
        case TYPE_CHROMAGGUS:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                HandleGameObject(GO_PORTCULLIS_NEFARIAN, true);
                HandleGameObject(GO_DOOR_NEFARIAN, true);
            }
			else if(uiData == SPECIAL)
			{
				DoUseDoorOrButton(GO_PORTCULLIS_CHROMAGGUS);
				Creature* chromaggus = GetSingleCreatureFromStorage(NPC_CHROMAGGUS);
				if(chromaggus == nullptr)
					break;
				chromaggus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				chromaggus->SetInCombatWithZone();
			}
            break;
        case VALUE_BREATH1:
            //genrate chromaggus breath and nefarian dragon
            for (uint8 i = 0; i < 2; i++)
            {
                uint8 uiPos1 = urand(0, 4);
                uint8 uiPos2 = 0;
                do { uiPos2 = urand(0, 4); } while (uiPos1 == uiPos2);
                m_auiEncounter[VALUE_BREATH1 + (i * 2)] = uiPos1;
                m_auiEncounter[VALUE_BREATH1+1 + (i * 2)] = uiPos2;
            }
            break;
        case TYPE_NEFARIAN:
            m_auiEncounter[uiType] = uiData;
            if (uiData == SPECIAL)
            {
				HandleGameObject(GO_DOOR_NEFARIAN, false);
                // handle missing spell 23362
                Creature* pNefarius = GetSingleCreatureFromStorage(NPC_LORD_VICTOR_NEFARIUS);
                if (!pNefarius)
                    break;

                for (GuidList::const_iterator itr = m_lDrakonidBonesGuids.begin(); itr != m_lDrakonidBonesGuids.end(); ++itr)
                {
                    // The Go script will handle the missing spell 23361
                    if (GameObject* pGo = instance->GetGameObject(*itr))
                        pGo->Use(pNefarius);
                }

                for (GuidList::const_iterator itr = m_lNefDragonsP1Guids.begin(); itr != m_lNefDragonsP1Guids.end(); ++itr)
                {
                    if (Creature* pDragonSpawn = instance->GetCreature(*itr))
                        pDragonSpawn->ForcedDespawn();
                }

				m_lNefDragonsP1Guids.clear();
                // Don't store special data
                break;
            }
			if (uiData == DONE)
			{
				HandleGameObject(GO_DOOR_NEFARIAN, true);

                for (GuidList::const_iterator itr = m_lDrakonidBonesGuids.begin(); itr != m_lDrakonidBonesGuids.end(); ++itr)
                {
                    if (GameObject* pGo = instance->GetGameObject(*itr))
					{
                        pGo->SetLootState(GO_JUST_DEACTIVATED);
						pGo->RemoveFromWorld();
					}
                }
				m_lDrakonidBonesGuids.clear();
			}
            if (uiData == FAIL)
            {
				HandleGameObject(GO_DOOR_NEFARIAN, true);

				if (Creature* pVictor = GetSingleCreatureFromStorage(NPC_LORD_VICTOR_NEFARIUS))
				{
					pVictor->ForcedDespawn();
					pVictor->Respawn();
				}

                for (GuidList::const_iterator itr = m_lDrakonidBonesGuids.begin(); itr != m_lDrakonidBonesGuids.end(); ++itr)
                {
                    if (GameObject* pGo = instance->GetGameObject(*itr))
					{
                        pGo->SetLootState(GO_JUST_DEACTIVATED);
						pGo->RemoveFromWorld();
					}
                }

                for (GuidList::const_iterator itr = m_lNefDragonsP1Guids.begin(); itr != m_lNefDragonsP1Guids.end(); ++itr)
                {
                    if (Creature* pDragonSpawn = instance->GetCreature(*itr))
                        pDragonSpawn->ForcedDespawn();
                }

				m_lDrakonidBonesGuids.clear();
				m_lNefDragonsP1Guids.clear();
            }
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " " 
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " "
            << m_auiEncounter[8] << " " << m_auiEncounter[9];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_blackwing_lair::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8]
        >> m_auiEncounter[9];

    // Dont set the spell values to NOT_STARTED (MAX_ENCOUNTER -4)
    for(uint8 i = 0; i < MAX_ENCOUNTER - 4; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_blackwing_lair::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_blackwing_lair::OnCreatureEnterCombat(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_GRETHOK_THE_CONTROLLER)
    {
		 if (Creature* pRazorgore = GetSingleCreatureFromStorage(NPC_RAZORGORE))
		 {
			 if(pCreature->getVictim() != nullptr)
				pRazorgore->SetInCombatWith(pCreature->getVictim());
		 }

        SetData(TYPE_RAZORGORE, IN_PROGRESS);
        m_uiRazorgoreSummonTimer = TIMER_START_SPAWMING_ADDS;
    }
}

void instance_blackwing_lair::OnCreatureDeath(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_GRETHOK_THE_CONTROLLER)
    {
        if (GameObject* Orb = GetSingleGameObjectFromStorage(GO_ORB_OF_DOMINATION))
            Orb->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);

		m_uiGrethokManager = true;
    }

    switch (pCreature->GetEntry())
    {
        case NPC_BLACKWING_LEGIONNAIRE:
        case NPC_BLACKWING_MAGE:
			--m_uiOrcSummoned;
		break;
        case NPC_DEATH_TALON_DRAGONSPAWN:
			--m_uiDragonkinSummoned;
		break;
		case NPC_BLUE_DRAGON:
		case NPC_BRONZE_DRAGON:
		case NPC_RED_DRAGON:
		case NPC_GREEN_DRAGON:
			pCreature->SetRespawnTime(20);
			pCreature->SaveRespawnTime();
		break;
		case NPC_DRAGONKIN_LASH:
		case NPC_PRIEST_LASH:
			pCreature->SetRespawnTime(1200);
			pCreature->SaveRespawnTime();
		break;
		case NPC_BLACKWING_GUARDSMAN:
			m_uiGuardsmanManager = true;
		break;
	}
}

void instance_blackwing_lair::OnCreatureEvade(Creature* pCreature) 
{
    switch(pCreature->GetEntry())
    {
		case NPC_GRETHOK_THE_CONTROLLER:
        case NPC_RAZORGORE:
        {
			SetData(TYPE_RAZORGORE, FAIL);
            break;
        }
        default:
            break;
    }
}

void instance_blackwing_lair::Update(uint32 uiDiff)
{
	//IN_PROGRESS = Razorgore Phase 1
    if (GetData(TYPE_RAZORGORE) == IN_PROGRESS)
    {
        if (m_uiRazorgoreSummonTimer <= uiDiff)
        {
            if (Creature* pRazorgore = GetSingleCreatureFromStorage(NPC_RAZORGORE))
            {
                for (uint32 i = 0; i < 8; i++)
                {
                    /* SPAWNS BY VALUE
                     * ===========
                     * 0         = nothing
                     * 1 - 3     = orc
                     * 4         = dragon
                     */
                     
                    uint8 orcCount = 0;
                    if (m_uiOrcSummoned < MAX_BLACKWING_ORC)
                        orcCount = 3;

                    uint8 dragonCount = 0;
                    if (m_uiDragonkinSummoned < MAX_BLACKWING_DRAGONKIN)
                        dragonCount = 1;

                    if (orcCount || dragonCount)
                    {
                        uint32 spawnType = urand(1, orcCount + dragonCount);
                        if (!orcCount)
                            spawnType = 4;
                   
                         Creature* spawnedOrcs	 = 0;
						 Creature* spawnedDragon = 0;

						if (spawnType < 4)
                        {
                            ++m_uiOrcSummoned;
							spawnedOrcs = pRazorgore->SummonCreature(urand(0, 1) ? NPC_BLACKWING_LEGIONNAIRE : NPC_BLACKWING_MAGE, Corner[i].x,
                                    Corner[i].y, Corner[i].z, Corner[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);                 
                        }
						else
                        {
							++m_uiDragonkinSummoned;
							spawnedDragon = pRazorgore->SummonCreature(NPC_DEATH_TALON_DRAGONSPAWN, Corner[i].x, Corner[i].y, Corner[i].z, Corner[i].o,
								TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                        }

                        //All adds have initial aggro on the Razorgore controller. The mage types will run to the center of the room and start to attack the controller with their ranged fireballs
                        Unit* pController = pRazorgore->GetCharmerOrOwner();
						if (spawnedOrcs)
                        {
							razorgoreAdds.push_back(spawnedOrcs);

                            if (pController && pController->isAlive())
								spawnedOrcs->AI()->AttackStart(pController);
							else
							{
								if (Unit* pTarget = spawnedOrcs->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
									spawnedOrcs->AI()->AttackStart(pTarget);
							}
                        }
						if (spawnedDragon)
						{
							razorgoreAdds.push_back(spawnedDragon);

							if (pRazorgore->isAlive())
							{
								spawnedDragon->AI()->AttackStart(pRazorgore);
								spawnedDragon->AddThreat(pRazorgore, 100.0f);
							}
							else
							{
								if (Unit* pTarget = spawnedDragon->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
									spawnedDragon->AI()->AttackStart(pTarget);
							}
						}
                    }
                }
            }

            m_uiRazorgoreSummonTimer = TIMER_NEXT_ADD_SPAWN;
        }
        else
            m_uiRazorgoreSummonTimer -= uiDiff;
    }

	/*if (GetData(TYPE_RAZORGORE) == DONE)
	{
		HandleGameObject(GO_PORTCULLIS_RAZORGORE, true);

		for(auto& creature : m_lBlackwingGuardsman)
		{
			if (Creature* guardsman = instance->GetCreature(creature))
				guardsman->ForcedDespawn();
		}

		if (Creature* grethok = GetSingleCreatureFromStorage(NPC_GRETHOK_THE_CONTROLLER))
			grethok->ForcedDespawn();	
	}*/

    if (m_uiChromaggusPullTimer)
    {
        if (m_uiChromaggusPullTimer <= uiDiff)
        {
            if (Creature* pChromaggus = GetSingleCreatureFromStorage(NPC_CHROMAGGUS))
            {
                pChromaggus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pChromaggus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pChromaggus->SetInCombatWithZone();
            }
            m_uiChromaggusPullTimer = 0;
        }
        else
            m_uiChromaggusPullTimer -= uiDiff;
    }

	if (m_uiGuardsmanManager && !m_uiGuardsmanManager)
	{
		if (m_uiGuardsmanManagerTimer <= uiDiff)
		{
			for (auto& creature : m_lBlackwingGuardsman)
			{
				if (Creature* pGuardsmans = instance->GetCreature(creature))
				{
					if (pGuardsmans->isDead() && GetData(TYPE_RAZORGORE) != DONE && GetData(TYPE_RAZORGORE) != SPECIAL && GetData(TYPE_RAZORGORE) != IN_PROGRESS && GetData(TYPE_RAZORGORE) != FAIL)
						pGuardsmans->Respawn();

					m_uiGuardsmanManager = false;
				}
			}
			m_uiGuardsmanManagerTimer = 30000;
		}
		else
			m_uiGuardsmanManagerTimer -= uiDiff;
	}

	if (m_uiGrethokManager && !m_uiGrethokManager)
	{
		if (m_uiGrethokManagerTimer <= uiDiff)
		{
			if (Creature* pGrethok = GetSingleCreatureFromStorage(NPC_GRETHOK_THE_CONTROLLER))
			{
				if (pGrethok->isDead() && GetData(TYPE_RAZORGORE) != DONE && GetData(TYPE_RAZORGORE) != SPECIAL && GetData(TYPE_RAZORGORE) != IN_PROGRESS && GetData(TYPE_RAZORGORE) != FAIL)
					pGrethok->Respawn();

				m_uiGrethokManager = false;
			}
			m_uiGrethokManagerTimer = 30000;
		}
		else
			m_uiGrethokManagerTimer -= uiDiff;
	}

	if (m_uiRazorgorManager)
	{
		if (m_uiRazorgorManagerTimer <= uiDiff)
		{
			if (Creature* pRazorgore = GetSingleCreatureFromStorage(NPC_RAZORGORE))
			{
				if (pRazorgore->isDead() && GetData(TYPE_RAZORGORE) != DONE)
					SetData(TYPE_RAZORGORE, DONE);

				m_uiRazorgorManager = false;
			}
			m_uiRazorgorManagerTimer = 30000;
		}
		else
			m_uiRazorgorManagerTimer -= uiDiff;
	}

	if (m_uiVealManager)
	{
		if (m_uiVealManagerTimer <= uiDiff)
		{
			if (Creature* pVeal = GetSingleCreatureFromStorage(NPC_VAELASTRASZ))
			{
				if (pVeal->isDead() && GetData(TYPE_VAELASTRASZ) != DONE)
					SetData(TYPE_VAELASTRASZ, DONE);

				m_uiVealManager = false;
			}
			m_uiVealManagerTimer = 30000;
		}
		else
			m_uiVealManagerTimer -= uiDiff;
	}

	/*if (m_uiLashLayerManager)
	{
		if (m_uiLashLayerManagerTimer <= uiDiff)
		{
			if (Creature* pLash = GetSingleCreatureFromStorage(NPC_LASHLAYER))
			{
				if (pLash->isDead() && GetData(TYPE_LASHLAYER) != DONE)
					SetData(TYPE_LASHLAYER, DONE);

				m_uiLashLayerManager = false;
			}
			m_uiLashLayerManagerTimer = 30000;
		}
		else
			m_uiLashLayerManagerTimer -= uiDiff;
	}*/

	if (m_uiChromaggusManager)
	{
		if (m_uiChromaggusManagerTimer <= uiDiff)
		{
			if (Creature* pChromaggus = GetSingleCreatureFromStorage(NPC_CHROMAGGUS))
			{
				if (pChromaggus->isDead() && GetData(TYPE_CHROMAGGUS) != DONE)
					SetData(TYPE_CHROMAGGUS, DONE);

				m_uiChromaggusManager = false;
			}
			m_uiChromaggusManagerTimer = 30000;
		}
		else
			m_uiChromaggusManagerTimer -= uiDiff;
	}
}

void instance_blackwing_lair::KillAllPlayersInRazorgoreRoom()
{
	Map::PlayerList const &PlayerList = instance->GetPlayers();
	for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
	{
		if (Creature* pRazorgore = GetSingleCreatureFromStorage(NPC_RAZORGORE))
		{
			Player* pPlayer = itr->getSource();
			if (pPlayer && pPlayer->isAlive() && pRazorgore->IsWithinDistInMap(pPlayer, 120))
				pRazorgore->DealDamage(pPlayer, 50000, NULL, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
		}
	}
}

void instance_blackwing_lair::DespawnEggs()
{
	for (GuidList::const_iterator itr = m_lDragonEggsGuids.begin(); itr != m_lDragonEggsGuids.end(); ++itr)
	{
		if (GameObject* pEgg = instance->GetGameObject(*itr))
		{
			if (pEgg->isSpawned())
				pEgg->RemoveFromWorld();
		}
	}
}

void instance_blackwing_lair::DespawnLashlayerRoom()
{
	for (GuidList::const_iterator itr = m_lLashDragons.begin(); itr != m_lLashDragons.end(); ++itr)
	{
		if (Creature* pDragonsLashSpawn = instance->GetCreature(*itr))
		{
			if (pDragonsLashSpawn)
			{
				pDragonsLashSpawn->setFaction(35);
				pDragonsLashSpawn->DeleteThreatList();
				pDragonsLashSpawn->SetVisibility(VISIBILITY_OFF);
				pDragonsLashSpawn->setAttackEnabled(false);
			}
		}
	}
}

void instance_blackwing_lair::DespawnTechnicians()
{				
	for(auto& guid : m_lTechnicianGuids)
	{
		if(auto* technician = instance->GetCreature(guid))
			technician->ForcedDespawn();
	}
}

void instance_blackwing_lair::SetPlayerController(ObjectGuid guid)
{
	m_playerController = guid;
}

InstanceData* GetInstanceData_instance_blackwing_lair(Map* pMap)
{
    return new instance_blackwing_lair(pMap);
}

void AddSC_instance_blackwing_lair()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_blackwing_lair";
    pNewScript->GetInstanceData = &GetInstanceData_instance_blackwing_lair;
    pNewScript->RegisterSelf();
}
