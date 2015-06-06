/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


//////////////////////////////////////////////////////////////////////////
// ** deadmines.cpp
//  Main file of deadmines scripts.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Header files
#include "precompiled.h"
#include "deadmines.h"


//////////////////////////////////////////////////////////////////////////
// Global functions: Script registration
bool GOUse_go_defias_cannon(Player* /*pPlayer*/, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if (!pInstance)
        return false;

    if (pInstance->GetData(TYPE_IRON_CLAD_DOOR) == DONE)
        return false;

    pInstance->SetData(TYPE_IRON_CLAD_DOOR, DONE);
    return false;
}

bool GOUse_go_defias_gunpowder(Player* pPlayer, GameObject* pGo)
{
	ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
	if (!pInstance)
		return false;

	// Temporary					  x				y			 z			 o
	const float coordinates[] = { -130.625626f, -604.718567f, 15.351316f, 5.062049f };

	// TODO: Enable respawn of patrol after death

	// Spawn patrol
	Creature *newCreature = pGo->SummonCreature(NPC_DEFIAS_OVERSEER, coordinates[0], coordinates[1], coordinates[2], coordinates[3], TEMPSUMMON_DEAD_DESPAWN, 0);
	newCreature->SetWalk(true);	// This creature will just walk slowly
	newCreature->GetMotionMaster()->Clear();
	newCreature->GetMotionMaster()->MovePoint(0, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ());
	
	return true;
}

void AddSC_deadmines()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "go_defias_cannon";
    pNewScript->pGOUse = &GOUse_go_defias_cannon;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "go_defias_gunpoweder";
	pNewScript->pGOUse = &GOUse_go_defias_gunpowder;
	pNewScript->RegisterSelf();
}

//////////////////////////////////////////////////////////////////////////
