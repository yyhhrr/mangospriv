#include "precompiled.h"
#include "KeyCodeManager.h"


bool GossipHello_npc_blizzcon_guy(Player* plr, Creature* creature)
{
	if (plr->isAlive())
		plr->PrepareGossipMenu(creature);

	if (plr->isAlive())
	{
		auto items = KeyCodeManager::GetItems();
		for(auto& item : items)
			plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, item.second.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + item.first, "Code:", true);
	}
	
	plr->SEND_GOSSIP_MENU(7780, creature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_blizzcon_guy(Player* plr, Creature*, uint32, uint32 uiAction, const char* key)
{
	plr->CLOSE_GOSSIP_MENU();

	if(KeyCodeManager::IsValidKey(key) == false)
		return true;
	
	auto queryId = uiAction - GOSSIP_ACTION_INFO_DEF;
	auto itemId = KeyCodeManager::GetItemId(queryId);

	ItemPosCountVec dest;
	if(plr->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1) != EQUIP_ERR_OK)
		return true;

	KeyCodeManager::RemoveKey(key);

	auto* item = plr->StoreNewItem(dest, itemId, true);
	if(item)
		plr->SendNewItem(item, 1, true, true, true);

    return true;
}

bool GossipSelect_npc_blizzcon_guy_no_code(Player* plr, Creature*, uint32, uint32 uiAction)
{
	plr->CLOSE_GOSSIP_MENU();
	return true;
}


void ADDSC_blizzconGuy()
{
	auto* script = new Script;
	script->Name = "npc_blizzcon_guy";
	script->pGossipHello =  &GossipHello_npc_blizzcon_guy;
	script->pGossipSelectWithCode = &GossipSelect_npc_blizzcon_guy;
	script->pGossipSelect = &GossipSelect_npc_blizzcon_guy_no_code;
	script->RegisterSelf();
}