/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


#ifndef DEF_DEADMINES_H
#define DEF_DEADMINES_H


//////////////////////////////////////////////////////////////////////////
// Common constants
enum
{
    MAX_ENCOUNTER           = 4,

    TYPE_RHAHKZOR           = 0,
    TYPE_SNEED              = 1,
    TYPE_GILNID             = 2,
    TYPE_IRON_CLAD_DOOR     = 3,

    INST_SAY_ALARM1         = -1036000,
    INST_SAY_ALARM2         = -1036001,

    GO_FACTORY_DOOR         = 13965,                        // rhahk'zor
    GO_MAST_ROOM_DOOR       = 16400,                        // sneed
    GO_FOUNDRY_DOOR         = 16399,                        // gilnid
    GO_HEAVY_DOOR_1         = 17153,                        // to sneed
    GO_HEAVY_DOOR_2         = 17154,                        // to gilnid
    GO_IRON_CLAD_DOOR       = 16397,
    GO_DEFIAS_CANNON        = 16398,
    GO_SMITE_CHEST          = 144111,                       // use to get correct location of mr.smites equipment changes
    GO_MYSTERIOUS_CHEST     = 180024,                       // used for quest 7938; spawns in the instance only if one of the players has the quest

    NPC_RHAHKZOR            = 644,
    NPC_SNEED               = 643,
    NPC_GILNID              = 1763,
    NPC_MR_SMITE            = 646,
    NPC_PIRATE              = 657,
    NPC_SQUALLSHAPER        = 1732,
	NPC_DEFIAS_BLACKGUARD	= 636,

	NPC_DEFIAS_OVERSEER		= 634,
	NPC_DEFIAS_EVOKER		= 1729,

	NPC_DEFIAS_TASKMASTER	= 4417,
	NPC_DEFIAS_WIZARD		= 4418,

    QUEST_FORTUNE_AWAITS    = 7938,
};

#endif
