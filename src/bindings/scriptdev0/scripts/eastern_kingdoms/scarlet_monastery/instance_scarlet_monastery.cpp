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
SDName: Instance_Scarlet_Monastery
SD%Complete: 50
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"
#include "scarlet_monastery.h"

instance_scarlet_monastery::instance_scarlet_monastery(Map* pMap) : 
	ScriptedInstance(pMap),
	m_ashbringer(nullptr)
{
    Initialize();
}

void instance_scarlet_monastery::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_scarlet_monastery::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
		case NPC_MOGRAINE_TRIGGER:
        case NPC_MOGRAINE:
        case NPC_WHITEMANE:
        case NPC_VORREL:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_scarlet_monastery::OnObjectCreate(GameObject* pGo)
{
    if (pGo->GetEntry() == GO_WHITEMANE_DOOR)
        m_mGoEntryGuidStore[GO_WHITEMANE_DOOR] = pGo->GetObjectGuid();
}

void instance_scarlet_monastery::SetData(uint32 uiType, uint32 uiData)
{
    if (uiType == TYPE_MOGRAINE_AND_WHITE_EVENT)
    {
        if (uiData == IN_PROGRESS)
            DoUseDoorOrButton(GO_WHITEMANE_DOOR);
        if (uiData == FAIL)
            DoUseDoorOrButton(GO_WHITEMANE_DOOR);

        m_auiEncounter[0] = uiData;
    }
}

uint32 instance_scarlet_monastery::GetData(uint32 uiData)
{
    if (uiData == TYPE_MOGRAINE_AND_WHITE_EVENT)
        return m_auiEncounter[0];

    return 0;
}

bool isPlayerInCathedral(Player* plr)
{
	float x, y, z;
	plr->GetPosition(x, y, z);
	
	return floor(x) == 855 && floor(y) == 1321 && floor(z) == 18;
}

void instance_scarlet_monastery::OnPlayerEnter(Player* player)
{
	if(player->HasItemCount(22691, 1) && m_ashbringer == nullptr && isPlayerInCathedral(player))
	{
		auto mograine = this->GetSingleCreatureFromStorage(NPC_MOGRAINE_TRIGGER);
		
		if(mograine == nullptr)
			return;

		m_ashbringer = player;
		mograine->MonsterYell(-1540000, 0);
	}
}

bool instance_scarlet_monastery::IsAshbringerEventRunning() const
{
	return m_ashbringer != nullptr;
}

Player* instance_scarlet_monastery::GetAshbringer()
{
	return m_ashbringer;
}



InstanceData* GetInstanceData_instance_scarlet_monastery(Map* pMap)
{
    return new instance_scarlet_monastery(pMap);
}

uint32 trash_text[] =
{
	-1540001,
	-1540002,
	-1540003,
	-1540004,
	-1540005
};

int random_int(int min, int max)
{
    return (int)((max-min) * rand()/(float)RAND_MAX +min);
}

struct Trash_AI : public ScriptedAI
{
	bool m_faceingDone;
	bool m_emoteDone;
	uint32 m_time;

    Trash_AI(Creature* pCreature) : 
		ScriptedAI(pCreature),
		m_faceingDone(false),
		m_emoteDone(false),
		m_time(0)
	{
	}

	static CreatureAI* Get(Creature* creature)
	{
		return new Trash_AI(creature);
	}
	
    void Reset() 
	{
		m_faceingDone = m_emoteDone = false;
		m_time = 0;
	}

	void UpdateAI(const uint32 timeDelta)
	{
		auto instance = static_cast<instance_scarlet_monastery*>(m_creature->GetInstanceData());
		if(m_emoteDone || instance == nullptr || instance->IsAshbringerEventRunning() == false)
			return ScriptedAI::UpdateAI(timeDelta);
		
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NONE);
		m_creature->SetFactionTemporary(35);

		if(m_creature->GetDistance(instance->GetAshbringer()) > 15.f)
			return ScriptedAI::UpdateAI(timeDelta);

		m_time += timeDelta;

		if(m_faceingDone == false)
		{
			m_creature->GetMotionMaster()->MoveIdle();
			m_creature->SetFacingToObject(instance->GetAshbringer());
			m_creature->SetStandState(UNIT_STAND_STATE_STAND);
			m_faceingDone = true;
			return;
		}

		if(m_time < 1000)
			return;


		int32 rnd = random_int(0, 6);
		if(rnd == 2)
			m_creature->MonsterSay(trash_text[random_int(0, 4)], 0);

		m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
		m_emoteDone = true;
	}
};


void AddSC_instance_scarlet_monastery()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_scarlet_monastery";
    pNewScript->GetInstanceData = &GetInstanceData_instance_scarlet_monastery;
    pNewScript->RegisterSelf();
	
    pNewScript = new Script;
    pNewScript->Name = "cathedral_trash_ai";
    pNewScript->GetAI = &Trash_AI::Get;
    pNewScript->RegisterSelf();
}
