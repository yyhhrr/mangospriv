#pragma once

#include "blackfathom_deeps.h"

/* Encounter 0 = Twilight Lord Kelris
   Encounter 1 = Shrine event
   Must kill twilight lord for shrine event to be possible
 */

class MANGOS_DLL_DECL instance_blackfathom_deeps : public ScriptedInstance
{
private:
	std::list<uint64> mShrineLists[4];
	std::vector<uint64> mShrineGuids;
	bool mHasShrineActivated[4];

	uint64 m_uiTwilightLordKelrisGUID;
    uint64 m_uiShrineOfGelihastGUID;
    uint64 m_uiAltarOfTheDeepsGUID;
    uint64 m_uiMainDoorGUID;
    uint8  m_uiShrinesLit;
	uint32 m_auiEncounter[MAX_ENCOUNTER];
	uint32 mShrineData[4];

	std::string mInstanceData;

	void checkShrines();

public:
    instance_blackfathom_deeps(Map* pMap);

	const char* Save() { return mInstanceData.c_str(); }

	void Load(const char* data);
	void OnCreatureDeath(Creature* pCreature);
	void pushShrineCreature(uint32 shrine, Creature* pCreature);
    void Initialize();
    void OnCreatureCreate(Creature* pCreature);
    void OnObjectCreate(GameObject* pGo);
    void SetData(uint32 uiType, uint32 uiData);
    uint32 GetData(uint32 uiType);
    uint64 GetData64(uint32 uiData);
};
