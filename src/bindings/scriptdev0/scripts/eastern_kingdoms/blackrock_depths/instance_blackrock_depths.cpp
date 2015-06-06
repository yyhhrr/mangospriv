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
SDName: Instance_Blackrock_Depths
SD%Complete: 80
SDComment:
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"
#include "blackrock_depths.h"
#include "the_grim_guzzler.h"

instance_blackrock_depths::instance_blackrock_depths(Map* pMap) : ScriptedInstance(pMap),
    m_uiBarAleCount(0),
    m_uiCofferDoorsOpened(0),

    m_fArenaCenterX(0.0f),
    m_fArenaCenterY(0.0f),
    m_fArenaCenterZ(0.0f),
	mFireguardTimer(0),
	m_darkKeeperSpawned(false),
	mBlackbreathSlaied(false),
	mFireguardSpanwed(false),
	mPlugger(nullptr),
	mPhalanx(nullptr),
	mNumKilledPatrons(0),
	numKlickedKegs(0),
	numFoodLooted(0)

{
    Initialize();
}

void instance_blackrock_depths::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    SetOpenedDoor(GO_JAIL_DOOR_DUGHAL, false);
    SetOpenedDoor(GO_JAIL_DOOR_TOBIAS, false);
    SetOpenedDoor(GO_JAIL_DOOR_JAZ,    false);
    SetOpenedDoor(GO_JAIL_DOOR_CREST,  false);
    SetOpenedDoor(GO_JAIL_DOOR_SHILL,  false);
    SetOpenedDoor(GO_JAIL_DOOR_SUPPLY, false);
	m_darkKeeperSpawned = false;
}

void instance_blackrock_depths::SetOpenedDoor(uint64 m_uiGoEntry, bool opened)
{
    switch(m_uiGoEntry)
    {
        case GO_JAIL_DOOR_DUGHAL: m_bDoorDughalOpened = opened; break;
        case GO_JAIL_DOOR_TOBIAS: m_bDoorTobiasOpened = opened; break;
        case GO_JAIL_DOOR_CREST:  m_bDoorCrestOpened  = opened; break;
        case GO_JAIL_DOOR_JAZ:    m_bDoorJazOpened    = opened; break;
        case GO_JAIL_DOOR_SHILL:  m_bDoorShillOpened  = opened; break;
        case GO_JAIL_DOOR_SUPPLY: m_bDoorSupplyOpened = opened; break;
    }
}

bool instance_blackrock_depths::GetOpenedDoor(uint64 m_uiGoEntry)
{
    switch(m_uiGoEntry)
    {
        case GO_JAIL_DOOR_DUGHAL: return m_bDoorDughalOpened;
        case GO_JAIL_DOOR_TOBIAS: return m_bDoorTobiasOpened;
        case GO_JAIL_DOOR_CREST:  return m_bDoorCrestOpened;
        case GO_JAIL_DOOR_JAZ:    return m_bDoorJazOpened;
        case GO_JAIL_DOOR_SHILL:  return m_bDoorShillOpened;
        case GO_JAIL_DOOR_SUPPLY: return m_bDoorSupplyOpened;
    }
    return false;
}

void instance_blackrock_depths::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_EMPEROR:
        case NPC_PRINCESS:
        case NPC_HATEREL:
        case NPC_ANGERREL:
        case NPC_VILEREL:
        case NPC_GLOOMREL:
        case NPC_SEETHREL:
        case NPC_DOOMREL:
        case NPC_DOPEREL:
        case NPC_OGRABISI:
        case NPC_SHILL:
        case NPC_CREST:
        case NPC_JAZ:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;

        case NPC_WARBRINGER_CONST:
            // sort them so we can get only the ones in the vault room
            if (std::abs(pCreature->GetPositionZ() - aVaultPositions[0][2]) < 1)
            {
                // we need to set the aura here and not in creature_addon because if the party wipes during the event then the mobs shouldn't reset the aura
                pCreature->CastSpell(pCreature, SPELL_STONED, true);

                m_suiVaultNpcGUIDs.insert(pCreature->GetObjectGuid());
            }
            break;

		case NPC_GUZZLING_PATRON:
		case NPC_GRIM_PATRON:
		case NPC_HAMMERED_PATRON:
			m_grimGuzzlerPatrons.push_back(pCreature->GetObjectGuid());
			break;

		case NPC_PHALANX:
			mPhalanx = pCreature;
			break;

		case NPC_PLUGGER_SPAZZRING:
			mPlugger = pCreature;
			break;

		case NPC_RIBBLY_CRONY:
			m_ribblyCronies.push_back(pCreature->GetObjectGuid());
			break;

        default:
            return;
    }
}

void instance_blackrock_depths::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ARENA_1:
        case GO_ARENA_2:
        case GO_ARENA_3:
        case GO_ARENA_4:
        case GO_JAIL_DOOR_SUPPLY:
        case GO_JAIL_SUPPLY_CRATE:
        case GO_SHADOW_LOCK:
        case GO_SHADOW_MECHANISM:
        case GO_SHADOW_GIANT_DOOR:
        case GO_SHADOW_DUMMY:
        case GO_BAR_KEG_SHOT:
        case GO_BAR_KEG_TRAP:
        case GO_TOMB_ENTER:
        case GO_TOMB_EXIT:
        case GO_LYCEUM:
        case GO_GOLEM_ROOM_N:
        case GO_GOLEM_ROOM_S:
        case GO_THRONE_ROOM:
        case GO_SPECTRAL_CHALICE:
        case GO_CHEST_SEVEN:
        case GO_ARENA_SPOILS:
        case GO_SECRET_DOOR:
		case GO_SECRET_DARK_COFFER:
		case GO_DARK_COFFER:
            break;
        case GO_BAR_DOOR:
			mBarDoor = pGo;
			break;
		case GO_RELIC_COFFER:
			m_relicCoffers.push_back(pGo);
			break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_blackrock_depths::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_RING_OF_LAW:
            // If finished the arena event after theldren fight
            if (uiData == DONE && m_auiEncounter[0] == SPECIAL)
                DoRespawnGameObject(GO_ARENA_SPOILS, HOUR*IN_MILLISECONDS);
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_VAULT:
            if (uiData == SPECIAL)
            {
                ++m_uiCofferDoorsOpened;

                if (m_uiCofferDoorsOpened == MAX_RELIC_DOORS)
                {
                    SetData(TYPE_VAULT, IN_PROGRESS);

                    // activate vault constructs
                    for (GUIDSet::const_iterator itr = m_suiVaultNpcGUIDs.begin(); itr != m_suiVaultNpcGUIDs.end(); ++itr)
                    {
                        if (Creature* pConstruct = instance->GetCreature(*itr))
                            pConstruct->RemoveAurasDueToSpell(SPELL_STONED);
                    }
					
					for(auto go : m_relicCoffers)
					{
						if(go != nullptr)
							go->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
					}

                    Player* pPlayer = GetPlayerInMap();
                    if (!pPlayer)
                        return;

                    // summon doomgrip and add him to the set
                    if (Creature* pDoomgrip = pPlayer->SummonCreature(NPC_WATCHER_DOOMGRIP, aVaultPositions[0][0], aVaultPositions[0][1], aVaultPositions[0][2], aVaultPositions[0][3], TEMPSUMMON_DEAD_DESPAWN, 0))
                        m_suiVaultNpcGUIDs.insert(pDoomgrip->GetObjectGuid());
                }
            }
            // don't set the special case
            if (uiData != SPECIAL)
                m_auiEncounter[1] = uiData;
            break;
        case TYPE_BAR:
            if (uiData == SPECIAL)
                ++m_uiBarAleCount;
            else
                m_auiEncounter[2] = uiData;

			if(uiData == DONE)
				OnPatronAttacked(); //Everyone inside the bar becomes hostile
            break;
        case TYPE_TOMB_OF_SEVEN:
            switch(uiData)
            {
                case IN_PROGRESS:
                    DoUseDoorOrButton(GO_TOMB_ENTER);
                    break;
                case FAIL:
                    if (m_auiEncounter[3] == IN_PROGRESS)//prevent use more than one time
                        DoUseDoorOrButton(GO_TOMB_ENTER);
                    break;
                case DONE:
                    DoRespawnGameObject(GO_CHEST_SEVEN, HOUR*IN_MILLISECONDS);
                    DoUseDoorOrButton(GO_TOMB_EXIT);
                    DoUseDoorOrButton(GO_TOMB_ENTER);
                    break;
            }
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_LYCEUM:
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_GOLEM_ROOM_N);
                DoUseDoorOrButton(GO_GOLEM_ROOM_S);
            }
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_IRON_HALL:
            switch(uiData)
            {
                case IN_PROGRESS:
                    DoUseDoorOrButton(GO_GOLEM_ROOM_N);
                    DoUseDoorOrButton(GO_GOLEM_ROOM_S);
                    break;
                case FAIL:
                    DoUseDoorOrButton(GO_GOLEM_ROOM_N);
                    DoUseDoorOrButton(GO_GOLEM_ROOM_S);
                    break;
                case DONE:
                    DoUseDoorOrButton(GO_GOLEM_ROOM_N);
                    DoUseDoorOrButton(GO_GOLEM_ROOM_S);
                    DoUseDoorOrButton(GO_THRONE_ROOM);
                    break;
            }
            m_auiEncounter[5] = uiData;
            break;
        case TYPE_QUEST_JAIL_BREAK:
            if (uiData == FAIL)
            {
                Map::PlayerList const &PlayerList = instance->GetPlayers();

                for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                {
                    Player* pPlayer = itr->getSource();
                    if (pPlayer && pPlayer->GetQuestStatus(QUEST_JAIL_BREAK) == QUEST_STATUS_INCOMPLETE)
                        pPlayer->SendQuestFailed(QUEST_JAIL_BREAK);
                }
            }
            m_auiEncounter[6] = uiData;
            break;
        case TYPE_JAIL_DUGHAL:
            m_auiEncounter[7] = uiData;
            break;
        case TYPE_JAIL_SUPPLY_ROOM:
            m_auiEncounter[8] = uiData;
            break;
        case TYPE_JAIL_TOBIAS:
            m_auiEncounter[9] = uiData;
            break;
		case TYPE_DOOMGRIP:
            m_auiEncounter[10] = uiData;
			DoUseDoorOrButton(GO_SECRET_DOOR);
			GetSingleGameObjectFromStorage(GO_SECRET_DARK_COFFER)->RemoveFlag(GAMEOBJECT_FLAGS, 127); //Remove all Flags, because for some reason this thing has weird flags
			break;
		case TYPE_BLACKBREATH:
			mBlackbreathSlaied = uiData == DONE;
			break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_blackrock_depths::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_RING_OF_LAW:
            return m_auiEncounter[0];
        case TYPE_VAULT:
            return m_auiEncounter[1];
        case TYPE_BAR:
            if (m_auiEncounter[2] == IN_PROGRESS && m_uiBarAleCount == 3)
                return SPECIAL;
            else
                return m_auiEncounter[2];
        case TYPE_TOMB_OF_SEVEN:
            return m_auiEncounter[3];
        case TYPE_LYCEUM:
            return m_auiEncounter[4];
        case TYPE_IRON_HALL:
            return m_auiEncounter[5];
        case TYPE_QUEST_JAIL_BREAK:
            return m_auiEncounter[6];
        case TYPE_JAIL_DUGHAL:
            return m_auiEncounter[7];
        case TYPE_JAIL_SUPPLY_ROOM:
            return m_auiEncounter[8];
        case TYPE_JAIL_TOBIAS:
            return m_auiEncounter[9];
		case TYPE_DOOMGRIP:
            return m_auiEncounter[10];
		case TYPE_BLACKBREATH:
			return mBlackbreathSlaied;
        default:
            return 0;
    }
}

void instance_blackrock_depths::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_blackrock_depths::OnCreatureEvade(Creature* pCreature)
{
    if (GetData(TYPE_RING_OF_LAW) == IN_PROGRESS || GetData(TYPE_RING_OF_LAW) == SPECIAL)
    {
        for (uint8 i = 0; i < sizeof(aArenaNPCs)/sizeof(uint32); ++i)
        {
            if (pCreature->GetEntry() == aArenaNPCs[i])
            {
                 SetData(TYPE_RING_OF_LAW, FAIL);
                 return;
             }
        }
    }
}

void instance_blackrock_depths::OnCreatureDeath(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_WARBRINGER_CONST:
        case NPC_WATCHER_DOOMGRIP:
            if (GetData(TYPE_VAULT) == IN_PROGRESS)
            {
                if (m_suiVaultNpcGUIDs.find(pCreature->GetObjectGuid()) != m_suiVaultNpcGUIDs.end())
                    m_suiVaultNpcGUIDs.erase(pCreature->GetObjectGuid());

                // if all event npcs dead then set event to done
                if (m_suiVaultNpcGUIDs.empty())
                    SetData(TYPE_VAULT, DONE);
            }
            break;
    }
}

void instance_blackrock_depths::SpawnDarkKeeper(Player* player, GameObject* go)
{
	if(m_darkKeeperSpawned)
		return;

	uint32 rndKeeper = urand(0, 5);
	auto keeperInfo = darkKeeperPositions[rndKeeper];

    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

	auto darkKeeper = pPlayer->SummonCreature(keeperInfo[0], keeperInfo[1], keeperInfo[2], keeperInfo[3], 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);
	if(darkKeeper == nullptr)
		return;
	
    player->SEND_GOSSIP_MENU(portraitTexts[rndKeeper], go->GetObjectGuid());

	m_darkKeeperSpawned = true;

	darkKeeper->SummonCreature(NPC_DARK_GUARD, keeperInfo[1] + 5, keeperInfo[2], keeperInfo[3], 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);
	darkKeeper->SummonCreature(NPC_DARK_GUARD, keeperInfo[1] - 5, keeperInfo[2], keeperInfo[3], 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);
}

void instance_blackrock_depths::OnPatronAttacked()
{
	if(!this || !instance)
		return;

	for(auto guid : m_grimGuzzlerPatrons)
	{
		auto creature = instance->GetCreature(guid);
		if(!creature)
			continue;

		creature->setFaction(54);
		creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	}
}

void instance_blackrock_depths::OnPatronDied(Unit* unit)
{
	const uint32 maxPatronsKilled = 5;

	if(++mNumKilledPatrons < maxPatronsKilled)
		return;

	ActivatePhalanx();
}

void instance_blackrock_depths::Update(uint32 timeDelta)
{
	if(mFireguardTimer)
	{
		if(mFireguardTimer <= timeDelta)
		{
			mFireguardTimer = 0;
			if(mPhalanx)
				SpawnFireGuard(mPhalanx);
		}
		else
			mFireguardTimer -= timeDelta;
	}
}

void instance_blackrock_depths::SpawnFireGuard(Unit* unit)
{
	if(mFireguardSpanwed)
		return;

	auto fireguard = unit->SummonCreature(NPC_FIREGUARD_DESTROYER, 872.31f, -231.11f, -43.75f, 2.21f, TEMPSUMMON_DEAD_DESPAWN, 0);
	auto officer1 = unit->SummonCreature(NPC_ANVILRAGE_OFFICER, 872.89f, -233.75f, -43.75f, 2.17f, TEMPSUMMON_DEAD_DESPAWN, 0);
	auto officer2 = unit->SummonCreature(NPC_ANVILRAGE_OFFICER, 874.41f, -232.21f, -43.75f, 2.17f, TEMPSUMMON_DEAD_DESPAWN, 0);

	if(!fireguard || !officer1 || !officer2)
		return;

	fireguard->GetMotionMaster()->MovePoint(0, 860.10f, -213.02f, -43.70f, false);
	officer1->GetMotionMaster()->MovePoint(0, 860.40f, -217.76f, -43.70f, false); //left
	officer2->GetMotionMaster()->MovePoint(0, 863.64f, -215.72f, -43.70f, false); //right

	fireguard->MonsterYell(-1540022, 0);
	fireguard->MonsterYell(-1540023, 0);

	if(mBarDoor && GetData(TYPE_BAR) != DONE)
		mBarDoor->SetGoState(GO_STATE_ACTIVE);
	
	mFireguardSpanwed = true;
}

void instance_blackrock_depths::ActivatePhalanx()
{
	if(!mPhalanx)
		return;

	auto ai = static_cast<mob_phalanxAI*>(mPhalanx->AI());
	if(ai)
		ai->Activate();

	mFireguardTimer = 60000;
}

void instance_blackrock_depths::OnFoodLooted(Player* looter)
{
	const uint32 TOTAL_FOOD = 3;

	if(!this || !mPlugger || mPlugger->isDead() || !instance)
		return;

	if(++numFoodLooted < TOTAL_FOOD)
	{
		int32 textid = urand(1540029, 1540031) * -1;
		mPlugger->MonsterYell(textid, 0);
		return;
	}

	mPlugger->MonsterYell(-1540032, 0);

	for(auto guid : m_grimGuzzlerPatrons)
	{
		auto creature = instance->GetCreature(guid);
		if(!creature)
			continue;

		creature->setFaction(54);
		creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	}

	mPlugger->AI()->AttackStart(looter);
}

void instance_blackrock_depths::OnKegKlicked(Player* plr)
{
	const uint32 TOTAL_KEGS = 3;
	const float x =  893.72f;
	const float y = -132.08f;
	const float z =  -49.74f;
	const float o =    2.01f;

	if(++numKlickedKegs < TOTAL_KEGS)
		return;
	if(GetData(TYPE_BLACKBREATH))
		return;

	auto blackbreath = plr->SummonCreature(NPC_HURLEY_BLACKBREATH, x, y, z, o, TEMPSUMMON_DEAD_DESPAWN, 0);
	
	auto summon1 = plr->SummonCreature(NPC_BLACKBREATH_CRONY, x + 5, y + 5, z, o, TEMPSUMMON_DEAD_DESPAWN, 0);
	auto summon2 = plr->SummonCreature(NPC_BLACKBREATH_CRONY, x    , y + 5, z, o, TEMPSUMMON_DEAD_DESPAWN, 0);
	auto summon3 = plr->SummonCreature(NPC_BLACKBREATH_CRONY, x + 5, y    , z, o, TEMPSUMMON_DEAD_DESPAWN, 0);

	if(!blackbreath || !summon1 || !summon2 || !summon3)
		return;

	blackbreath->MonsterYell(-1540020, 0);
	blackbreath->AI()->AttackStart(plr);
	summon1->AI()->AttackStart(plr);
	summon2->AI()->AttackStart(plr);
	summon3->AI()->AttackStart(plr);
}

void instance_blackrock_depths::OnRibblyGossip(Player* plr)
{
	if(!this || !instance)
		return;

	for(auto guid : m_ribblyCronies)
	{
		auto creature = instance->GetCreature(guid);
		if(creature)
		{
			creature->setFaction(54);
			creature->AI()->AttackStart(plr);
		}
	}
}

InstanceData* GetInstanceData_instance_blackrock_depths(Map* pMap)
{
    return new instance_blackrock_depths(pMap);
}

void AddSC_instance_blackrock_depths()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_blackrock_depths";
    pNewScript->GetInstanceData = &GetInstanceData_instance_blackrock_depths;
    pNewScript->RegisterSelf();
}
