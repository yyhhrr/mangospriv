/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SCHOLOMANCE_H
#define DEF_SCHOLOMANCE_H

enum
{
    MAX_ENCOUNTER           = 11,

    GO_GATE_KIRTONOS        = 175570,
    GO_GATE_GANDLING        = 177374,
    GO_GATE_MALICIA         = 177375,
    GO_GATE_THEOLEN         = 177377,
    GO_GATE_POLKELT         = 177376,
    GO_GATE_RAVENIAN        = 177372,
    GO_GATE_BAROV           = 177373,
    GO_GATE_ILLUCIA         = 177371,
    GO_GATE_FACKEL			= 175610,

    TYPE_GANDLING           = 1,
    TYPE_THEOLEN            = 2,
    TYPE_MALICIA            = 3,
    TYPE_ILLUCIABAROV       = 4,
    TYPE_ALEXEIBAROV        = 5,
    TYPE_POLKELT            = 6,
    TYPE_RAVENIAN           = 7,
    TYPE_KIRTONOS           = 8,
    TYPE_FACKEL				= 9,
    TYPE_RAUM_ONE			=10,
    TYPE_RAUM_TWO			=11,
    TYPE_RAUM_THREE			=12,
    TYPE_RAUM_FOUR			=13,
    TYPE_RAUM_FIVE			=14,
    TYPE_RAUM_SIX			=15,

    NPC_GANDLING			= 1853
};

enum Creatures
{
    NPC_BLOOD_STEWARD_OF_KIRTONOS   = 14861,
    NPC_KIRTONOS_THE_HERALD         = 10506,
    NPC_ILLUSION_OF_JANDICE_BAROV   = 11439,
    NPC_RISEN_CONSTRUCT             = 10488,
    NPC_ASPECT_OF_BANALITY          = 14518,
    NPC_BANAL_SPIRIT                = 14514,
    NPC_ASPECT_OF_MALICE            = 14520,
    NPC_MALICIOUS_SPIRIT            = 14513,
    NPC_ASPECT_OF_CORRUPTION        = 14519,
    NPC_CORRUPTED_SPIRIT            = 14512,
    NPC_ASPECT_OF_SHADOW            = 14521,
    NPC_SHADOWED_SPIRIT             = 14511,
    NPC_DEATH_KNIGHT_DARKREAVER     = 14516,
    NPC_DARKREAVERS_FALLEN_CHARGER  = 14568,
    NPC_BONE_MINION                 = 16119,
    NPC_BONE_MAGE                   = 16120,
    NPC_SCHOLOMANCE_STUDENT         = 10475,
    NPC_MARDUK_BLACKPOOL            = 10433,
    NPC_VECTUS                      = 10432,
    NPC_KORMOK                      = 16118,
	NPC_RISEN_GUARDIAN              = 11598,
    NPC_DARKMASTER_GANDLING         = 1853,
};

enum Misc
{
    FACTION_FRIENDLY                = 35,
    FACTION_HOSTILE                 = 14,
};

#endif
