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
#include "blackfathom_deeps.h"
#include "instance_blackfathom_deeps.h"

bool GOHello_go_akumai_one(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        // Elementare
        Creature* ele1 = pPlayer->SummonCreature(4978, -768.21f, -164.25f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        ele1->AI()->AttackStart(pPlayer);
        Creature* ele2 = pPlayer->SummonCreature(4978, -868.21f, -164.25f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        ele2->AI()->AttackStart(pPlayer);

		instance_blackfathom_deeps* pBFInstance = (instance_blackfathom_deeps*)pInstance;
		pBFInstance->pushShrineCreature(0, ele1);
		pBFInstance->pushShrineCreature(0, ele2);
    }
    return false;
}

bool GOHello_go_akumai_two(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        // Schildkröten
        Creature* schild1 = pPlayer->SummonCreature(4825, -768.80f, -153.84f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        schild1->AI()->AttackStart(pPlayer);
        Creature* schild2 = pPlayer->SummonCreature(4825, -868.80f, -153.84f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        schild2->AI()->AttackStart(pPlayer);

       instance_blackfathom_deeps* pBFInstance = (instance_blackfathom_deeps*)pInstance;
		pBFInstance->pushShrineCreature(1, schild1);
		pBFInstance->pushShrineCreature(1, schild2);
    }
    return false;
}

bool GOHello_go_akumai_three(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        // Krebse
        Creature* krebs1 = pPlayer->SummonCreature(4823, -859.64f, -158.84f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        krebs1->AI()->AttackStart(pPlayer);
        Creature* krebs2 = pPlayer->SummonCreature(4823, -778.59f, -159.85f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        krebs2->AI()->AttackStart(pPlayer);
        Creature* krebs3 = pPlayer->SummonCreature(4823, -859.64f, -169.45f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        krebs3->AI()->AttackStart(pPlayer);
        Creature* krebs4 = pPlayer->SummonCreature(4823, -778.59f, -169.85f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        krebs4->AI()->AttackStart(pPlayer);

        instance_blackfathom_deeps* pBFInstance = (instance_blackfathom_deeps*)pInstance;
		pBFInstance->pushShrineCreature(2, krebs1);
		pBFInstance->pushShrineCreature(2, krebs2);
		pBFInstance->pushShrineCreature(2, krebs4);
		pBFInstance->pushShrineCreature(2, krebs3);
    }
    return false;
}

bool GOHello_go_akumai_four(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        // Krabben
        Creature* krabbe1 = pPlayer->SummonCreature(4977, -768.73f, -152.89f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe1->AI()->AttackStart(pPlayer);
        Creature* krabbe2 = pPlayer->SummonCreature(4977, -768.73f, -156.89f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe2->AI()->AttackStart(pPlayer);
        Creature* krabbe3 = pPlayer->SummonCreature(4977, -768.73f, -164.89f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe3->AI()->AttackStart(pPlayer);
        Creature* krabbe4 = pPlayer->SummonCreature(4977, -768.73f, -172.89f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe4->AI()->AttackStart(pPlayer);
        Creature* krabbe5 = pPlayer->SummonCreature(4977, -768.73f, -176.89f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe5->AI()->AttackStart(pPlayer);
        Creature* krabbe6 = pPlayer->SummonCreature(4977, -868.73f, -176.29f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe6->AI()->AttackStart(pPlayer);
        Creature* krabbe7 = pPlayer->SummonCreature(4977, -868.73f, -172.29f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe7->AI()->AttackStart(pPlayer);
        Creature* krabbe8 = pPlayer->SummonCreature(4977, -868.73f, -164.29f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe8->AI()->AttackStart(pPlayer);
        Creature* krabbe9 = pPlayer->SummonCreature(4977, -868.73f, -155.29f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN,200000);
        krabbe9->AI()->AttackStart(pPlayer);
        Creature* krabbe10 = pPlayer->SummonCreature(4977, -868.73f, -151.29f, -25.87f, 0, TEMPSUMMON_DEAD_DESPAWN, 200000);
        krabbe10->AI()->AttackStart(pPlayer);

		instance_blackfathom_deeps* pBFInstance = (instance_blackfathom_deeps*)pInstance;
		pBFInstance->pushShrineCreature(3, krabbe1);
		pBFInstance->pushShrineCreature(3, krabbe2);
		pBFInstance->pushShrineCreature(3, krabbe3);
		pBFInstance->pushShrineCreature(3, krabbe4);
		pBFInstance->pushShrineCreature(3, krabbe5);
		pBFInstance->pushShrineCreature(3, krabbe6);
		pBFInstance->pushShrineCreature(3, krabbe7);
		pBFInstance->pushShrineCreature(3, krabbe8);
		pBFInstance->pushShrineCreature(3, krabbe9);
		pBFInstance->pushShrineCreature(3, krabbe10);
    }

    return false;
}


bool GOHello_go_gm_door(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        pPlayer->Say("Ich habe die T\303\274r ge\303\266ffnet.",LANG_UNIVERSAL);
    }
    return false;
}

void AddSC_blackfathom_deeps()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_gm_door";
    newscript->pGOUse = &GOHello_go_gm_door;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_akumai_one";
    newscript->pGOUse = &GOHello_go_akumai_one;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_akumai_two";
    newscript->pGOUse = &GOHello_go_akumai_two;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_akumai_three";
    newscript->pGOUse = &GOHello_go_akumai_three;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_akumai_four";
    newscript->pGOUse = &GOHello_go_akumai_four;
    newscript->RegisterSelf();
}
