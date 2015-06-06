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
#include "escort_ai.h"
#include "dire_maul.h"

bool ItemUse_isalien_summon(Player*, Item*, SpellCastTargets const&)
{
//    ScriptedInstance* pInstance = (ScriptedInstance*)player->GetInstanceData();
    //pInstance->SetData(TYPE_ISALIEN_SUMMON, IN_PROGRESS);
    return true;
}

struct MANGOS_DLL_DECL schattenpirscherAI : public ScriptedAI
{
    schattenpirscherAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    void Reset()
    {

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
        {
            Reset();
            if (!m_creature->HasAura(1785))
                DoCastSpellIfCan(m_creature,1785);
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_schattenpirscher(Creature *_Creature)
{
    return new schattenpirscherAI (_Creature);
}

/*#####################################################################*/

#define SAY_EISENBORKE_START	-1901005
#define SAY_EISENBORKE_ENDE		-1901006

struct MANGOS_DLL_DECL npc_eisenborkeAI : public npc_escortAI
{
    npc_eisenborkeAI(Creature* pCreature) : npc_escortAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* pInstance;

    void Reset()
	{
	
	}

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 6:
				if (pInstance)
					pInstance->SetData(TYPE_IRONBARK, DONE);

				DoScriptText(SAY_EISENBORKE_ENDE, m_creature);
                break;
        }
    }
};

CreatureAI* GetAI_npc_eisenborke(Creature *_Creature)
{
    return new npc_eisenborkeAI (_Creature);
}

bool GossipHello_npc_eisenborke(Player* pPlayer, Creature* pCreature)
{
	ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData();

	if (pInstance->GetData(TYPE_ZEVRIM) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Vielen Dank, Eisenborke. Wir sind bereit. \303\226ffne uns bitte das Tor.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(90005, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_eisenborke(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
			pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			DoScriptText(SAY_EISENBORKE_START, pCreature);
			if (npc_eisenborkeAI* pEscortAI = dynamic_cast<npc_eisenborkeAI*>(pCreature->AI()))
				pEscortAI->Start(true, pPlayer);
            break;
    }
    return true;
}

/*########
# npc_knut
#########*/

enum
{
    QUEST_KNUT					= 5525,
	QUEST_KNUT_WIEDERHOHLBAR	= 7429,
};

struct MANGOS_DLL_DECL npc_knutAI : public ScriptedAI
{
    npc_knutAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* pInstance;

    void Reset()
    {

    }

    void ReceiveEmote(Player* pPlayer, uint32 uiEmote)
    {
        if (uiEmote == TEXTEMOTE_CHEER)
        {
            if (pPlayer->GetQuestStatus(QUEST_KNUT) == QUEST_STATUS_COMPLETE || pPlayer->GetQuestStatus(QUEST_KNUT_WIEDERHOHLBAR) == QUEST_STATUS_COMPLETE)
            {
                pInstance->DoRespawnGameObject(GO_KNOTS_CACHE, 604800);
            }
        }
    }

    void UpdateAI(const uint32)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_npc_knut(Creature* pCreature)
{
    return new npc_knutAI(pCreature);
}

/*####
# go_broken_trap
####*/

enum eBrokenTrap
{
    QUEST_A_BROKEN_TRAP   = 1193,
};

bool QuestRewarded_go_broken_trap(Player* pPlayer, GameObject* pGo, const Quest* pQuest)
{
	Map* pMap;
	pMap = pGo->GetMap();

	if(pMap == nullptr)
    return false;

	float CoordX;
	float CoordY;
	float CoordZ;
	float CoordO;

    if (pQuest->GetQuestId() == QUEST_A_BROKEN_TRAP)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (instance_dire_maul* m_pInstance = (instance_dire_maul*)pGo->GetInstanceData())
        {
			if (pGo->GetGUID())
			{
				CoordX = pGo->GetPositionX();
				CoordY = pGo->GetPositionY();
				CoordZ = pGo->GetPositionZ();
				CoordO = pGo->GetOrientation();

				pGo->SummonObject(pMap, GO_FIXED_TRAP, CoordX, CoordY, CoordZ, CoordO);
			}
			pGo->Delete();
        }
    }
    return true;
}

void AddSC_dire_maul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="isalien_summon";
    newscript->pItemUse = &ItemUse_isalien_summon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="schattenpirscher";
    newscript->GetAI = &GetAI_schattenpirscher;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_eisenborke";
    newscript->GetAI = &GetAI_npc_eisenborke;
    newscript->pGossipHello = &GossipHello_npc_eisenborke;
    newscript->pGossipSelect = &GossipSelect_npc_eisenborke;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_knut";
    newscript->GetAI = &GetAI_npc_knut;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_broken_trap";
    newscript->pQuestRewardedGO = &QuestRewarded_go_broken_trap;
    newscript->RegisterSelf();
}