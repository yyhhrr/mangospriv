#include "precompiled.h"

/*######
## npc_orgrimmar_uc
######*/

bool GossipHello_npc_orgrimmar_uc(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Undercity.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_orgrimmar_uc(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, 2066.916504f, 286.200500f, 97.031906f, 4.589250f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_undercity_og(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Orgrimmar.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_undercity_og(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, 1332.507202f, -4650.166016f, 53.543610f, 6.272145f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_gromgol_og(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Orgrimmar.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_gromgol_og(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, 1352.009155f, -4644.555176f, 53.543777f, 3.857046f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_orgrimmar_gromgol(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Grom'Gol.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_orgrimmar_gromgol(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, -12438.978516f, 213.476990f, 31.291590f, 5.754197f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_gromgol_uc(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Undercity.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_gromgol_uc(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, 2057.792480f, 242.228790f, 99.767632f, 1.404461f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_undercity_gromgol(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Grom'Gol.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_undercity_gromgol(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, -12421.134766f, 207.844894f, 31.292362f, 3.240926f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_ratschet_bb(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Booty Bay.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_ratschet_bb(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, -14284.496094f, 558.249634f, 8.832505f, 4.353838f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_bootybay_ratschet(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		if (pPlayer->GetTeam() == HORDE)
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Ratchet.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		else
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Ratchet.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_bootybay_ratschet(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1: //Ratchet
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, -995.532959f, -3825.746094f, 5.576647f, 0.976207f);
		break;

	case GOSSIP_ACTION_INFO_DEF + 2: //Menethil
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, -995.532959f, -3825.746094f, 5.576647f, 0.976207f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_menethil_auberdine(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Auberdine.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_menethil_auberdine(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, 6423.250000f, 817.465881f, 5.530914f, 6.013740f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_auberdine_menethil(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Menethil.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_auberdine_menethil(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, -3727.666260f, -583.837464f, 6.244044f, 3.689313f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_auberdine_ruttheran(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Rut'theran.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_auberdine_ruttheran(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, 8553.945313f, 1020.105896f, 5.503716f, 6.182240f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_ruttheran_auberdine(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Auberdine.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_ruttheran_auberdine(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, 6574.964355f, 769.546265f, 5.483481f, 2.886283f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_theramore_menethil(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Menethil.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_theramore_menethil(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(0, -3894.315918f, -601.069885f, 5.475846f, 5.475846f);
		break;
	}
	return true;
}

//###########################################################################################
bool GossipHello_npc_menethil_theramore(Player* pPlayer, Creature* pCreature)
{
	if (pPlayer->GetSession()->IsPremium())
	{
		pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bringt mich nach Theramore.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		pPlayer->SEND_GOSSIP_MENU(90002, pCreature->GetObjectGuid());
	}
	else
		pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetObjectGuid());

	return true;
}

bool GossipSelect_npc_menethil_theramore(Player* pPlayer, Creature*, uint32, uint32 uiAction)
{
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		pPlayer->CLOSE_GOSSIP_MENU();
		pPlayer->CastSpell(pPlayer, 2479, true);
		pPlayer->TeleportTo(1, -4002.463379f, -4727.212402f, 4.989918f, 0.809825f);
		break;
	}
	return true;
}

void AddSC_world_premium_system()
{
	Script* pNewScript;
	pNewScript = new Script;
	pNewScript->Name = "npc_orgrimmar_uc";
	pNewScript->pGossipHello = &GossipHello_npc_orgrimmar_uc;
	pNewScript->pGossipSelect = &GossipSelect_npc_orgrimmar_uc;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_undercity_og";
	pNewScript->pGossipHello = &GossipHello_npc_undercity_og;
	pNewScript->pGossipSelect = &GossipSelect_npc_undercity_og;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_orgrimmar_gromgol";
	pNewScript->pGossipHello = &GossipHello_npc_orgrimmar_gromgol;
	pNewScript->pGossipSelect = &GossipSelect_npc_orgrimmar_gromgol;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_gromgol_og";
	pNewScript->pGossipHello = &GossipHello_npc_gromgol_og;
	pNewScript->pGossipSelect = &GossipSelect_npc_gromgol_og;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_bootybay_ratschet";
	pNewScript->pGossipHello = &GossipHello_npc_bootybay_ratschet;
	pNewScript->pGossipSelect = &GossipSelect_npc_bootybay_ratschet;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_gromgol_uc";
	pNewScript->pGossipHello = &GossipHello_npc_gromgol_uc;
	pNewScript->pGossipSelect = &GossipSelect_npc_gromgol_uc;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_ratschet_bb";
	pNewScript->pGossipHello = &GossipHello_npc_ratschet_bb;
	pNewScript->pGossipSelect = &GossipSelect_npc_ratschet_bb;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_auberdine_menethil";
	pNewScript->pGossipHello = &GossipHello_npc_auberdine_menethil;
	pNewScript->pGossipSelect = &GossipSelect_npc_auberdine_menethil;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_menethil_auberdine";
	pNewScript->pGossipHello = &GossipHello_npc_menethil_auberdine;
	pNewScript->pGossipSelect = &GossipSelect_npc_menethil_auberdine;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_ruttheran_auberdine";
	pNewScript->pGossipHello = &GossipHello_npc_ruttheran_auberdine;
	pNewScript->pGossipSelect = &GossipSelect_npc_ruttheran_auberdine;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_auberdine_ruttheran";
	pNewScript->pGossipHello = &GossipHello_npc_auberdine_ruttheran;
	pNewScript->pGossipSelect = &GossipSelect_npc_auberdine_ruttheran;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_theramore_menethil";
	pNewScript->pGossipHello = &GossipHello_npc_theramore_menethil;
	pNewScript->pGossipSelect = &GossipSelect_npc_theramore_menethil;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_menethil_theramore";
	pNewScript->pGossipHello = &GossipHello_npc_menethil_theramore;
	pNewScript->pGossipSelect = &GossipSelect_npc_menethil_theramore;
	pNewScript->RegisterSelf();
}