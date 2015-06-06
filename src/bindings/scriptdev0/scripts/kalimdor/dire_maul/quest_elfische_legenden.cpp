/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################
*/

// UPDATE gameobject_template SET ScriptName = 'Quest_ElfischeLegenden' WHERE entry = 178544;

#include "precompiled.h"
#include "dire_maul.h"

bool GossipHello(Player* plr, GameObject*)
{
	if(plr->IsActiveQuest(7481)) {
		plr->CompleteQuest(7481);
	}

	if(plr->IsActiveQuest(7482))
		plr->CompleteQuest(7482);
	return true;
}

void AddSC_QuestElfischeLegenden()
{
	Script* pScript = new Script;
	pScript->Name = "Quest_ElfischeLegenden";
	pScript->pGossipHelloGO = &GossipHello;
	pScript->RegisterSelf();
}