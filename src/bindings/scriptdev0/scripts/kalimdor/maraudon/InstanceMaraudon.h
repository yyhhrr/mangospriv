#pragma once



class MANGOS_DLL_DECL InstanceMaraudon : public ScriptedInstance
{
public:
	InstanceMaraudon(Map* pMap);
	~InstanceMaraudon() {}

	void OnCreatureCreate(Creature* pCreature);
	void OnGameObjectCreate(GameObject* pGO);

	void Update(uint32 diff);

	void SetData(uint32 uiType, uint32 uiData);
	uint32 GetData(uint32 uiType);

	const char* Save() { return m_strInstData.c_str(); }
	void Load(const char* chrIn);

	enum : uint32
	{
		CELEBRAS_THE_REDEEMED = 13716,
		THE_CEPTER_OF_CELEBRAS = 7046,
		STAFF_CREATOR = 178560,
		TOME = 178965,
		SPELL_CREATE_SCEPTER = 21939,
		QUEST_CEPTER = 0,
	};

	void ActivateCreator();
protected:
	std::string m_strInstData;

private:
	uint64 mCelbrasRedeemedGUID;
	uint64 mStaffCreatorGUID;
	EncounterState mCepterQuestProgress;
};