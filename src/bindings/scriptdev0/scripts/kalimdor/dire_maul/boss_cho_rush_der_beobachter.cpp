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

#include "precompiled.h"
#include "boss_cho_rush.h"


CreatureAI* GetAI_boss_cho_rush_der_beobachter(Creature *_Creature)
{
    return new boss_cho_rush_der_beobachterAI (_Creature);
}

void AddSC_boss_cho_rush_der_beobachter()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_cho_rush_der_beobachter";
    newscript->GetAI = &GetAI_boss_cho_rush_der_beobachter;
    newscript->RegisterSelf();
}

