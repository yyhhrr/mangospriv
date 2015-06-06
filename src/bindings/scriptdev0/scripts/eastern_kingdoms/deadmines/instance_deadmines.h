/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


#ifndef DEF_INSTANCE_DEADMINES_H
#define DEF_INSTANCE_DEADMINES_H


#include "deadmines.h"


//////////////////////////////////////////////////////////////////////////
// Mr. Smite's AI struct
class MANGOS_DLL_DECL instance_deadmines 
	: public ScriptedInstance
{
public:
	instance_deadmines(Map* pMap);

	void Initialize() override;

	void OnPlayerEnter(Player* pPlayer) override;

	void OnCreatureCreate(Creature* pCreature) override;
	void OnObjectCreate(GameObject* pGo) override;

	void OnCreatureEnterCombat(Creature* pCreature) override;
	void OnCreatureDeath(Creature* pCreature) override;

	void SetData(uint32 uiType, uint32 uiData) override;
	uint32 GetData(uint32 uiType) const;

	const char* Save() { return m_strInstData.c_str(); }
	void Load(const char* chrIn) override;

	void Update(uint32 uiDiff) override;

private:
	uint32 m_auiEncounter[MAX_ENCOUNTER];
	std::string m_strInstData;

	uint32 m_uiIronDoorTimer;
	uint32 m_uiDoorStep;
};


#endif	//DEF_INSTANCE_DEADMINES_H
