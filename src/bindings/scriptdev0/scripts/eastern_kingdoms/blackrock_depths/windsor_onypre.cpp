/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: redridge_mountains
SD%Complete: 100%
SDComment: Quest support: 219
SDCategory: Redridge Mountains
EndScriptData */


#include "precompiled.h"
#include "escort_ai.h"
#include "blackrock_depths.h"


enum
{
    QUEST_MISSING_IN_ACTION = 4322,

    SAY_MARSHAL_WINDSOR_1  = -1000561,
    SAY_MARSHAL_WINDSOR_2  = -1000562,
    SAY_MARSHAL_WINDSOR_3  = -1000563,
    SAY_MARSHAL_WINDSOR_4  = -1000564,
    SAY_MARSHAL_WINDSOR_5  = -1000565,
};

struct MANGOS_DLL_DECL npc_marshall_windsor_escortAI : public npc_escortAI
{
    npc_marshall_windsor_escortAI(Creature* pCreature) : npc_escortAI(pCreature) {
        Reset();
    }

    void Reset()
    {

    }


    void WaypointStart(uint32 uiWP)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        switch (uiWP)
        {

        }
    }

    void WaypointReached(uint32 uiWP)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        switch (uiWP)
        {
        case 11:
            pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR_ONE),0,false);
            break;
        case 13:
            pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_CHEST_ONE),0,false);
            m_creature->SetDisplayId(9052);
            break;
        case 24:
            pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR_TWO),0,false);
            break;
        case 27:
            pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR_THREE),0,false);
            break;
        case 43:
            pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR_FOUR),0,false);
            break;
        case 50:
            pInstance->DoUseDoorOrButton(pInstance->GetData64(DATA_DOOR_FIVE),0,false);
            break;
        case 53:
            m_creature->SetSpeedRate(MOVE_RUN,1.4f,true);
            break;
        case 70:
            if (Player* pPlayer = GetPlayerForEscort())
                pPlayer->GroupEventHappens(QUEST_MISSING_IN_ACTION, m_creature);
            break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (Player* pPlayer = GetPlayerForEscort())
            pPlayer->FailQuest(QUEST_MISSING_IN_ACTION);
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        //Combat check
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_marshall_windsor(Creature* pCreature)
{
    return new npc_marshall_windsor_escortAI(pCreature);
}

bool QuestAccept_npc_marshall_windsor(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_MISSING_IN_ACTION)
    {
        if (npc_marshall_windsor_escortAI* pEscortAI = dynamic_cast<npc_marshall_windsor_escortAI*>(pCreature->AI()))
        {
            //DoScriptText(SAY_MARSHAL_WINDSOR_1, pCreature);
            //pEscortAI->Start(false, pPlayer->GetGUID(), pQuest, false, false);
			pEscortAI->Start(false, pPlayer);
        }
    }
    return true;

}

void AddSC_npc_marshall_windsor()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "npc_marshall_windsor";
    NewScript->GetAI = &GetAI_npc_marshall_windsor;
    NewScript->pQuestAcceptNPC = &QuestAccept_npc_marshall_windsor;
    NewScript->RegisterSelf();
}
