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

#ifndef DEF_ZULGURUB_H
#define DEF_ZULGURUB_H

enum
{
    MAX_ENCOUNTER           = 12,
    MAX_PRIESTS             = 5,

    TYPE_JEKLIK             = 0,
    TYPE_VENOXIS            = 1,
    TYPE_MARLI              = 2,
    TYPE_THEKAL             = 3,
    TYPE_ARLOKK             = 4,
    TYPE_OHGAN              = 5,                            // Do not change, used by Acid
    TYPE_LORKHAN            = 6,
    TYPE_ZATH               = 7,
	TYPE_MANDOKIR			= 8,
	TYPE_HAKKAR				= 9,
	TYPE_GAHZRANKA			= 10,
	TYPE_RND_BOSS			= 11,

    NPC_LORKHAN             = 11347,
    NPC_ZATH                = 11348,
    NPC_THEKAL              = 14509,
    NPC_JINDO               = 11380,
    NPC_HAKKAR              = 14834,
	NPC_VILEBRANCH_SPEAKER	= 11391,
    NPC_POISONOUS_CLOUD     = 14989,
	NPC_BLOODSEEKER_BAT		= 11368,
	NPC_FRENZIED_BAT		= 14965,
	NPC_JEKLIK_FIRE_DUMMY	= 800051,

	//Mini Bosse
	NPC_HAZZARAH			= 15083,
	NPC_RENATAKI			= 15084,
	NPC_WUSHOOLAY			= 15085,
	NPC_GRILEK				= 15082,

	GO_JEKLIK_FIRE			= 180125,
    GO_FORCEFIELD           = 180497,

    SAY_MINION_DESTROY      = -1309022,
    SAY_HAKKAR_PROTECT      = -1309023,

    HP_LOSS_PER_PRIEST      = 60000,

    AREATRIGGER_ENTER       = 3958,
    AREATRIGGER_ALTAR       = 3960,
};

enum Misc
{
    AT_ZULGURUB_ENTRANCE_1          = 3958,
    AT_ZULGURUB_ENTRANCE_2          = 3956,
    AT_ALTAR_OF_THE_BLOOD_GOD_DOWN  = 3964,
    AT_ALTAR_OF_THE_BLOOD_GOD_UP    = 3960,
    FACTION_ENEMY                   = 14,
    FACTION_FRIENDLY                = 35,
    SAY_HAKKAR_MINION_DESTROY       = -1309022,         // Yell when player steps into ZG instance
    SAY_HAKKAR_PROTECT_ALTAR        = -1309023,         // Yell when player steps on Altar of Blood
};

class MANGOS_DLL_DECL instance_zulgurub : public ScriptedInstance
{
    public:
        instance_zulgurub(Map* pMap);
        ~instance_zulgurub() {}

        void Initialize();
        // IsEncounterInProgress() const { return false; }  // not active in Zul'Gurub

        void OnCreatureCreate(Creature* pCreature);
		void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

		void HakkarYell(uint32 atEntry);

    protected:
        void DoLowerHakkarHitPoints();
		void DespawnJeklikFire();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

		GuidList m_lJeklikFireGuids;
		GuidList m_lJeklikFireMobGuids;

        bool m_bHasIntroYelled;
        bool m_bHasAltarYelled;
        bool m_bEnterZulGurubYelled;
        bool m_bProtectAltarYelled;
};

#endif
