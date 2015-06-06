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

#define kniesehne	9080
#define spalten	15284

struct MANGOS_DLL_DECL npc_khan_geisterAI : public ScriptedAI
{
    npc_khan_geisterAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 kniesehne_timer;
    uint32 manifestation_endet_timer;
    uint32 spalten_timer;

    void Reset()
    {
        kniesehne_timer = 3000;
        spalten_timer	= 4000;

		m_creature->setFaction(35);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (kniesehne_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), kniesehne);
            kniesehne_timer = random(4000, 8000);
        } else
			kniesehne_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), spalten);
            spalten_timer = random(4000, 7000);
        } else
			spalten_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_npc_khan_geister(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->HasQuest(7067))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Ich bin hier, um Euch zu besiegen!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_khan_geister(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->setFaction(14);
            break;
    }
    return true;
}

CreatureAI* GetAI_npc_khan_geister(Creature *_Creature)
{
    return new npc_khan_geisterAI (_Creature);
}

void AddSC_npc_khan_geister()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_khan_geister";
    newscript->GetAI = &GetAI_npc_khan_geister;
    newscript->pGossipHello = &GossipHello_npc_khan_geister;
    newscript->pGossipSelect = &GossipSelect_npc_khan_geister;
    newscript->RegisterSelf();
}

