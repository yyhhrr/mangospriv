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

#ifndef SC_INSTANCE_H
#define SC_INSTANCE_H

#include "InstanceData.h"
#include "Map.h"

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAIL          = 2,
    DONE          = 3,
    SPECIAL       = 4
};

#define OUT_SAVE_INST_DATA             debug_log("SD0: Saving Instance Data for Instance %s (Map %d, Instance Id %d)", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_SAVE_INST_DATA_COMPLETE    debug_log("SD0: Saving Instance Data for Instance %s (Map %d, Instance Id %d) completed.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA(a)          debug_log("SD0: Loading Instance Data for Instance %s (Map %d, Instance Id %d). Input is '%s'", instance->GetMapName(), instance->GetId(), instance->GetInstanceId(), a)
#define OUT_LOAD_INST_DATA_COMPLETE    debug_log("SD0: Instance Data Load for Instance %s (Map %d, Instance Id: %d) is complete.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA_FAIL        error_log("SD0: Unable to load Instance Data for Instance %s (Map %d, Instance Id: %d).", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())

class MANGOS_DLL_DECL ScriptedInstance : public InstanceData
{
    public:
        ScriptedInstance(Map* pMap) : InstanceData(pMap) {}
        ~ScriptedInstance() {}

        // Default accessor functions
        GameObject* GetSingleGameObjectFromStorage(uint32 uiEntry);
        Creature* GetSingleCreatureFromStorage(uint32 uiEntry, bool bSkipDebugLog = false);

        // Change active state of doors or buttons
        void DoUseDoorOrButton(ObjectGuid guid, uint32 uiWithRestoreTime = 0, bool bUseAlternativeState = false);
        // FIXME revert back to uint32 uiEntry when no more GO-Guids are stored as uint64
        void DoUseDoorOrButton(uint64 uiEntry, uint32 uiWithRestoreTime = 0, bool bUseAlternativeState = false);

        // Respawns a GO having negative spawntimesecs in gameobject-table
        void DoRespawnGameObject(ObjectGuid guid, uint32 uiTimeToDespawn = MINUTE);
        // FIXME revert back to uint32 uiEntry when no more GO-Guids are stored as uint64
        void DoRespawnGameObject(uint64 uiEntry, uint32 uiTimeToDespawn = MINUTE);

        // Sends world state update to all players in instance
        void DoUpdateWorldState(uint32 uiStateId, uint32 uiStateData);

        // Get a Player from map
        Player* GetPlayerInMap(bool bOnlyAlive = false, bool bCanBeGamemaster = true);

        /// Wrapper for simulating map-wide text in this instance. It is expected that the Creature is stored in m_mNpcEntryGuidStore if loaded.
        void DoOrSimulateScriptTextForThisInstance(int32 iTextEntry, uint32 uiCreatureEntry)
        {
            // Prevent debug output in GetSingleCreatureFromStorage
            DoOrSimulateScriptTextForMap(iTextEntry, uiCreatureEntry, instance, GetSingleCreatureFromStorage(uiCreatureEntry, true));
        }

		void DoToggleGameObjectFlags(uint32 entry, uint32 flag, bool unk)
		{
			GameObject* obj = GetSingleGameObjectFromStorage(entry);
			if(!obj)
				return;


			if(obj->HasFlag(GAMEOBJECT_FLAGS, flag))
				flag = ~flag;
			
			obj->SetFlag(GAMEOBJECT_FLAGS, flag);
		}

        //use HandleGameObject(GUID,boolen,NULL); in any other script
        void HandleGameObject(ObjectGuid guid, bool open);
        void HandleGameObject(uint32 uiEntry, bool open);

        // set interaction with GameObject
		void InteractWithGo(ObjectGuid guid, bool state);
		void InteractWithGo(uint32 uiEntry, bool state);

    protected:
        // Storage for GO-Guids and NPC-Guids
        EntryGuidMap m_mGoEntryGuidStore;                   ///< Store unique GO-Guids by entry
        EntryGuidMap m_mNpcEntryGuidStore;                  ///< Store unique NPC-Guids by entry
};

// Class for world maps (May need additional zone-wide functions later on)
class MANGOS_DLL_DECL ScriptedMap : public ScriptedInstance
{
    public:
        ScriptedMap(Map* pMap) : ScriptedInstance(pMap) {}
};

/// A static const array of this structure must be handled to DialogueHelper
struct DialogueEntry
{
    int32 iTextEntry;                                       ///< To be said text entry
    uint32 uiSayerEntry;                                    ///< Entry of the mob who should say
    uint32 uiTimer;                                         ///< Time delay until next text of array is said (0 stops)
};

/// A static const array of this structure must be handled to DialogueHelper
struct DialogueEntryTwoSide
{
    int32 iTextEntry;                                       ///< To be said text entry (first side)
    uint32 uiSayerEntry;                                    ///< Entry of the mob who should say (first side)
    int32 iTextEntryAlt;                                    ///< To be said text entry (second side)
    uint32 uiSayerEntryAlt;                                 ///< Entry of the mob who should say (second side)
    uint32 uiTimer;                                         ///< Time delay until next text of array is said (0 stops)
};

/// Helper class handling a dialogue given as static const array of DialogueEntry or DialogueEntryTwoSide
class DialogueHelper
{
    public:
        // The array MUST be terminated by {0,0,0}
        DialogueHelper(DialogueEntry const* pDialogueArray);
        // The array MUST be terminated by {0,0,0,0,0}
        DialogueHelper(DialogueEntryTwoSide const* aDialogueTwoSide);

        /// Function to initialize the dialogue helper for instances. If not used with instances, GetSpeakerByEntry MUST be overwritten to obtain the speakers
        void InitializeDialogueHelper(ScriptedInstance* pInstance, bool bCanSimulateText = false) { m_pInstance = pInstance; m_bCanSimulate = bCanSimulateText; }
        /// Set if take first entries or second entries
        void SetDialogueSide(bool bIsFirstSide) { m_bIsFirstSide = bIsFirstSide; }

        void StartNextDialogueText(int32 iTextEntry);

        void DialogueUpdate(uint32 uiDiff);

    protected:
        /// Will be called when a dialogue step was done
        virtual void JustDidDialogueStep(int32 iEntry) {}
        /// Will be called to get a speaker, MUST be implemented if not used in instances
        virtual Creature* GetSpeakerByEntry(uint32 uiEntry) { return NULL; }

    private:
        void DoNextDialogueStep();

        ScriptedInstance* m_pInstance;

        DialogueEntry const* m_pDialogueArray;
        DialogueEntry const* m_pCurrentEntry;
        DialogueEntryTwoSide const* m_pDialogueTwoSideArray;
        DialogueEntryTwoSide const* m_pCurrentEntryTwoSide;

        uint32 m_uiTimer;
        bool m_bIsFirstSide;
        bool m_bCanSimulate;
};

#endif
