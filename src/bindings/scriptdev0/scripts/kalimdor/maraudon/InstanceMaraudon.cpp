#include "precompiled.h"
#include "InstanceMaraudon.h"


InstanceMaraudon::InstanceMaraudon(Map* pMap) : 
	ScriptedInstance(pMap),
	mCelbrasRedeemedGUID(0),
	mStaffCreatorGUID(0),
	mCepterQuestProgress(NOT_STARTED)
{
	SetData(QUEST_CEPTER, NOT_STARTED);
}

void InstanceMaraudon::OnGameObjectCreate(GameObject* pGO)
{
	if(pGO->GetEntry() == STAFF_CREATOR)
		mStaffCreatorGUID = pGO->GetGUID();
}

void InstanceMaraudon::OnCreatureCreate(Creature* pCreature)
{
	if(pCreature->GetEntry() == CELEBRAS_THE_REDEEMED)
		mCelbrasRedeemedGUID = pCreature->GetGUID();
}

uint32 InstanceMaraudon::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case QUEST_CEPTER:
            return mCepterQuestProgress;
		default:
			return 0;
    }
}

void InstanceMaraudon::SetData(uint32 uiType, uint32 uiData)
{
	switch (uiType)
	{
	case QUEST_CEPTER:
		{
			mCepterQuestProgress = (EncounterState)uiData;
		} break;
	}

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream <<  mCepterQuestProgress;

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void InstanceMaraudon::Load(const char* chrIn)
{
    OUT_LOAD_INST_DATA(chrIn);

	uint32 tmp;

    std::istringstream loadStream(chrIn);
    loadStream >> tmp;

	mCepterQuestProgress = (EncounterState)tmp;

	if(mCepterQuestProgress)
		SetData(mCepterQuestProgress, IN_PROGRESS);

    OUT_LOAD_INST_DATA_COMPLETE;
}


void InstanceMaraudon::ActivateCreator()
{
	DoUseDoorOrButton(ObjectGuid(mStaffCreatorGUID));
}


void InstanceMaraudon::Update(uint32)
{
}