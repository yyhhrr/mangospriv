/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################
*/

/* ScriptData
SDName: Instance_Blackfathom_Deeps
SD%Complete: 50
SDComment:
SDCategory: Blackfathom Deeps
EndScriptData */

#include "precompiled.h"
#include "instance_blackfathom_deeps.h"

/* Encounter 0 = Twilight Lord Kelris
   Encounter 1 = Shrine event
   Must kill twilight lord for shrine event to be possible
 */


instance_blackfathom_deeps::instance_blackfathom_deeps(Map* pMap) : ScriptedInstance(pMap) {
    Initialize();
};

void instance_blackfathom_deeps::pushShrineCreature(uint32 shrine, Creature* pCreature) {
	if(shrine >= 4 || pCreature == nullptr)
		return;

	mShrineLists[shrine].push_back(pCreature->GetGUID());
	mHasShrineActivated[shrine] = true;
}

void instance_blackfathom_deeps::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

	m_uiTwilightLordKelrisGUID = 0;
    m_uiShrineOfGelihastGUID = 0;
    m_uiAltarOfTheDeepsGUID = 0;
    m_uiMainDoorGUID = 0;
    m_uiShrinesLit = 0;

	memset(mHasShrineActivated, false, sizeof(mHasShrineActivated));
}

void instance_blackfathom_deeps::OnCreatureCreate(Creature* pCreature)
{
    if (pCreature->GetEntry() == 4832)
        m_uiTwilightLordKelrisGUID = pCreature->GetGUID();
}

void instance_blackfathom_deeps::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
    case 103015:
        m_uiShrineOfGelihastGUID = pGo->GetGUID();
        break;
    case 103016:
        m_uiAltarOfTheDeepsGUID = pGo->GetGUID();
        break;
    case 21117:
		{
			m_uiMainDoorGUID = pGo->GetGUID();
			if(GetData(TYPE_SHRINE) == DONE && GetData(TYPE_KELRIS) == DONE) {
				pGo->SetGoState(GOState::GO_STATE_ACTIVE);
				pGo->SetFlag(GAMEOBJECT_FLAGS, GameObjectFlags::GO_FLAG_NO_INTERACT);
			}
			break;
		}

	case 21119:
	case 21120:
	case 21121:
	case 21118:
		{
			pGo->SetFlag(GAMEOBJECT_FLAGS, GameObjectFlags::GO_FLAG_NO_INTERACT);
			mShrineGuids.push_back(pGo->GetGUID());
		}
		break;
    }
}

void instance_blackfathom_deeps::OnCreatureDeath(Creature* creature) {
	if(creature == nullptr)
		return;

	switch(creature->GetEntry()) {
	case 4978:
		{
			mShrineLists[0].remove(creature->GetGUID());
		}
		break;

	case 4825:
		{
			mShrineLists[1].remove(creature->GetGUID());
		}
		break;

	case 4823:
		{
			mShrineLists[2].remove(creature->GetGUID());
		}
		break;

	case 4977:
		{
			mShrineLists[3].remove(creature->GetGUID());
		}
		break;
	}

	checkShrines();
}

void instance_blackfathom_deeps::checkShrines() {
	if(GetData(TYPE_SHRINE) == DONE)
		return;

	for(uint32 i = 0; i < 4; ++i) {
		if(mHasShrineActivated[i] == false || mShrineLists[i].size() > 0) {
			return;
		}
	}

	for(auto shrine : mShrineGuids) {
		GameObject* obj = instance->GetGameObject(shrine);
		if(obj != nullptr)
			obj->SetFlag(GAMEOBJECT_FLAGS, GameObjectFlags::GO_FLAG_NO_INTERACT);
	}

	SetData(TYPE_SHRINE, DONE);
}

void instance_blackfathom_deeps::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
    case TYPE_KELRIS:
        if (uiData == DONE && m_auiEncounter[1] == DONE)
            DoUseDoorOrButton(m_uiMainDoorGUID);
		
		if(uiData == DONE) {
			for(auto shrine : mShrineGuids) {
				GameObject* obj = instance->GetGameObject(shrine);
				if(obj != nullptr) {
					obj->RemoveFlag(GAMEOBJECT_FLAGS, GameObjectFlags::GO_FLAG_NO_INTERACT);
				}
			}
		}

        m_auiEncounter[0] = uiData;
        break;

    case TYPE_SHRINE:
        if (uiData == DONE && m_auiEncounter[0] == DONE)
            DoUseDoorOrButton(m_uiMainDoorGUID);

        m_auiEncounter[1] = uiData;
        break;

	case DATA_SHRINE1:
		{
			mShrineData[0] = uiData;
		}
		break;

	case DATA_SHRINE2:
		{
			mShrineData[1] = uiData;
		}
		break;

	case DATA_SHRINE3:
		{
			mShrineData[2] = uiData;
		}
		break;

	case DATA_SHRINE4:
		{
			mShrineData[3] = uiData;
		}
		break;
    }

	if(uiData == DONE) {
		std::ostringstream strm;
		strm << m_auiEncounter[0] << " " << m_auiEncounter[1];

		mInstanceData = strm.str();
		SaveToDB();

		OUT_SAVE_INST_DATA_COMPLETE;
	}
}

void instance_blackfathom_deeps::Load(const char* data) {
	std::string strData = data;

	std::stringstream strm;
	strm << data;
	strm >> m_auiEncounter[0] >> m_auiEncounter[1];
}

uint32 instance_blackfathom_deeps::GetData(uint32 uiType)
{
    switch (uiType)
    {
    case TYPE_KELRIS:
        return m_auiEncounter[0];

    case TYPE_SHRINE:
        return m_auiEncounter[1];

	case DATA_SHRINE1:
		return mShrineData[0];

	case DATA_SHRINE2:
		return mShrineData[1];

	case DATA_SHRINE3:
		return mShrineData[2];

	case DATA_SHRINE4:
		return mShrineData[3];
    }

    return 0;
}

uint64 instance_blackfathom_deeps::GetData64(uint32 uiData)
{
    switch (uiData)
    {
    case DATA_TWILIGHT_LORD_KELRIS:
        return m_uiTwilightLordKelrisGUID;
    case DATA_SHRINE_OF_GELIHAST:
        return m_uiShrineOfGelihastGUID;
    case DATA_MAINDOOR:
        return m_uiMainDoorGUID;
    }

    return 0;
}

InstanceData* GetInstanceData_instance_blackfathom_deeps(Map* pMap)
{
    return new instance_blackfathom_deeps(pMap);
}

void AddSC_instance_blackfathom_deeps()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "instance_blackfathom_deeps";
    newscript->GetInstanceData = &GetInstanceData_instance_blackfathom_deeps;
    newscript->RegisterSelf();
}
