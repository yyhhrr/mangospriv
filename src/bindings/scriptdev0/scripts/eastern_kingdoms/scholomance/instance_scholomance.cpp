/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Scholomance
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"
#include "scholomance.h"

struct MANGOS_DLL_DECL instance_scholomance : public ScriptedInstance
{
    instance_scholomance(Map* pMap) : ScriptedInstance(pMap) {
        Initialize();
    };

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint64 m_uiGateKirtonosGUID;
    uint64 m_uiGateGandlingGUID;
    uint64 m_uiGateMiliciaGUID;
    uint64 m_uiGateTheolenGUID;
    uint64 m_uiGatePolkeltGUID;
    uint64 m_uiGateRavenianGUID;
    uint64 m_uiGateBarovGUID;
    uint64 m_uiGateIlluciaGUID;
    uint64 m_uiGateFackelGUID;

    uint64 m_uiGandlingGUID;

    Creature* grandling;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiGateKirtonosGUID = 0;
        m_uiGateGandlingGUID = 0;
        m_uiGateMiliciaGUID = 0;
        m_uiGateTheolenGUID = 0;
        m_uiGatePolkeltGUID = 0;
        m_uiGateRavenianGUID = 0;
        m_uiGateBarovGUID = 0;
        m_uiGateIlluciaGUID = 0;
        m_uiGateFackelGUID = 0;

        m_uiGandlingGUID = 0;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch (pGo->GetEntry())
        {
        case GO_GATE_KIRTONOS:
            m_uiGateKirtonosGUID = pGo->GetGUID();
            break;
        case GO_GATE_GANDLING:
            m_uiGateGandlingGUID = pGo->GetGUID();
            break;
        case GO_GATE_MALICIA:
            m_uiGateMiliciaGUID = pGo->GetGUID();
            break;
        case GO_GATE_THEOLEN:
            m_uiGateTheolenGUID = pGo->GetGUID();
            break;
        case GO_GATE_POLKELT:
            m_uiGatePolkeltGUID = pGo->GetGUID();
            break;
        case GO_GATE_RAVENIAN:
            m_uiGateRavenianGUID = pGo->GetGUID();
            break;
        case GO_GATE_BAROV:
            m_uiGateBarovGUID = pGo->GetGUID();
            break;
        case GO_GATE_ILLUCIA:
            m_uiGateIlluciaGUID = pGo->GetGUID();
            break;
        case GO_GATE_FACKEL:
            m_uiGateFackelGUID = pGo->GetGUID();
            break;
        }
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
        case NPC_GANDLING:
            m_uiGandlingGUID = pCreature->GetGUID();
            grandling = pCreature;
            grandling->setFaction(35);
            grandling->SetVisibility(VISIBILITY_OFF);
            break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
	{
        switch (uiType)
        {
        case TYPE_GANDLING:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateGandlingGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateGandlingGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateGandlingGUID);
                m_auiEncounter[0] = uiData;
                break;
            }
            break;

        case TYPE_FACKEL:
            switch (uiData)
            {
            case IN_PROGRESS:
                break;
            case FAIL:
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateFackelGUID);
                m_auiEncounter[9] = uiData;
                break;
            }
            break;

        case TYPE_KIRTONOS:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateKirtonosGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateKirtonosGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateKirtonosGUID);
                m_auiEncounter[1] = uiData;
                break;
            }
            break;

        case TYPE_ALEXEIBAROV:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateBarovGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateBarovGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateBarovGUID);
                m_auiEncounter[2] = uiData;
                GetData(TYPE_GANDLING);
                break;
            }
            break;
        case TYPE_THEOLEN:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateTheolenGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateTheolenGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateTheolenGUID);
                m_auiEncounter[3] = uiData;
                GetData(TYPE_GANDLING);
                break;
            }
            break;
        case TYPE_RAVENIAN:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateRavenianGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateRavenianGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateRavenianGUID);
                m_auiEncounter[4] = uiData;
                GetData(TYPE_GANDLING);
                break;
            }
            break;
        case TYPE_POLKELT:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGatePolkeltGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGatePolkeltGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGatePolkeltGUID);
                m_auiEncounter[5] = uiData;
                GetData(TYPE_GANDLING);
                break;
            }
            break;
        case TYPE_MALICIA:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateMiliciaGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateMiliciaGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateMiliciaGUID);
                m_auiEncounter[6] = uiData;
                GetData(TYPE_GANDLING);
                break;
            }
            break;
        case TYPE_ILLUCIABAROV:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateIlluciaGUID);
                break;
            case FAIL:
                DoUseDoorOrButton(m_uiGateIlluciaGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateIlluciaGUID);
                m_auiEncounter[7] = uiData;
                GetData(TYPE_GANDLING);
                break;
            }
            break;

        case TYPE_RAUM_ONE:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGatePolkeltGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGatePolkeltGUID);
                break;
            }
            break;

        case TYPE_RAUM_TWO:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateTheolenGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateTheolenGUID);
                break;
            }
            break;

        case TYPE_RAUM_THREE:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateMiliciaGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateMiliciaGUID);
                break;
            }
            break;

        case TYPE_RAUM_FOUR:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateIlluciaGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateIlluciaGUID);
                break;
            }
            break;

        case TYPE_RAUM_FIVE:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateBarovGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateBarovGUID);
                break;
            }
            break;

        case TYPE_RAUM_SIX:
            switch (uiData)
            {
            case IN_PROGRESS:
                DoUseDoorOrButton(m_uiGateRavenianGUID);
                break;
            case DONE:
                DoUseDoorOrButton(m_uiGateRavenianGUID);
                break;
            }
            break;
		}

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " " << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " " << m_auiEncounter[9] << " " << m_auiEncounter[10] << " ";

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    uint32 GetData(uint32 uiType)
    {

        if (uiType == TYPE_GANDLING)
        {

            if (m_auiEncounter[2] == DONE && m_auiEncounter[3] == DONE && m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE && m_auiEncounter[6] == DONE && m_auiEncounter[7] == DONE)
            {
                grandling->SetVisibility(VISIBILITY_ON);
                grandling->setFaction(754);
                m_auiEncounter[0] = DONE;
            }

        }

        return 0;
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10];

        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;

    }
};

InstanceData* GetInstanceData_instance_scholomance(Map* pMap)
{
    return new instance_scholomance(pMap);
}

void AddSC_instance_scholomance()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_scholomance";
    newscript->GetInstanceData = &GetInstanceData_instance_scholomance;
    newscript->RegisterSelf();
}
