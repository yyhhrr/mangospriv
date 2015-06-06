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

/* ScriptData
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/* ContentData
EndContentData */

#include "precompiled.h"
#include "Spell.h"

enum PremiumItems
{
	ITEM_PREMIUM_EIN_MONAT		= 50010,
	ITEM_PREMIUM_DREI_MONATE	= 50011,
	ITEM_PREMIUM_SECHS_MONATE	= 50012
};

bool ItemUse_item_premium_ein_monat(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
	if (pPlayer->GetSession()->IsPremium())
		return EQUIP_ERR_CANT_DO_RIGHT_NOW;
	else
	{
		if (pPlayer->HasItemCount(ITEM_PREMIUM_EIN_MONAT, 1))
		{
			pPlayer->Whisper("Dein Account wurde für einen Monat auf Premium aufgestuft. Damit die Änderungen wirksam werden, empfehlen wir dir, dich einmal neu einzuloggen. Viel Spaß!", 0, pPlayer->GetObjectGuid());
			pPlayer->DestroyItemCount(ITEM_PREMIUM_EIN_MONAT, 1, true);
			pPlayer->CastSpell(pPlayer, 2061, false);

			LoginDatabase.PExecute("INSERT INTO account_premium (id, setdate, unsetdate, premium_type, active, accountname, laufzeit, email) VALUES ('%u', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP + 2629743, 1, 1, '', '', '')", pPlayer->GetAccountId());
			return true;
		}
	}
	return false;
}

bool ItemUse_item_premium_drei_monate(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
	if (pPlayer->GetSession()->IsPremium())
		return EQUIP_ERR_CANT_DO_RIGHT_NOW;
	else
	{
		if (pPlayer->HasItemCount(ITEM_PREMIUM_DREI_MONATE, 1))
		{
			pPlayer->Whisper("Dein Account wurde für drei Monate auf Premium aufgestuft. Damit die Änderungen wirksam werden, empfehlen wir dir, dich einmal neu einzuloggen. Viel Spaß!", 0, pPlayer->GetObjectGuid());
			pPlayer->DestroyItemCount(ITEM_PREMIUM_DREI_MONATE, 1, true);
			pPlayer->CastSpell(pPlayer, 2061, false);

			LoginDatabase.PExecute("INSERT INTO account_premium (id, setdate, unsetdate, premium_type, active, accountname, laufzeit, email) VALUES ('%u', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP + 7889231, 1, 1, '', '', '')", pPlayer->GetAccountId());
			return true;
		}
	}
	return false;
}

bool ItemUse_item_premium_sechs_monate(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
	if (pPlayer->GetSession()->IsPremium())
		return EQUIP_ERR_CANT_DO_RIGHT_NOW;
	else
	{
		if (pPlayer->HasItemCount(ITEM_PREMIUM_SECHS_MONATE, 1))
		{
			pPlayer->Whisper("Dein Account wurde für sechs Monate auf Premium aufgestuft. Damit die Änderungen wirksam werden, empfehlen wir dir, dich einmal neu einzuloggen. Viel Spaß!", 0, pPlayer->GetObjectGuid());
			pPlayer->DestroyItemCount(ITEM_PREMIUM_SECHS_MONATE, 1, true);
			pPlayer->CastSpell(pPlayer, 2061, false);

			LoginDatabase.PExecute("INSERT INTO account_premium (id, setdate, unsetdate, premium_type, active, accountname, laufzeit, email) VALUES ('%u', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP + 15778463, 1, 1, '', '', '')", pPlayer->GetAccountId());
			return true;
		}
	}
	return false;
}
/*######
## AddSC
######*/

void AddSC_item_scripts()
{
	Script* pNewscript;

	pNewscript = new Script;
	pNewscript->Name = "item_premium_ein_monat";
	pNewscript->pItemUse = &ItemUse_item_premium_ein_monat;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "item_premium_drei_monate";
	pNewscript->pItemUse = &ItemUse_item_premium_drei_monate;
	pNewscript->RegisterSelf();

	pNewscript = new Script;
	pNewscript->Name = "item_premium_sechs_monate";
	pNewscript->pItemUse = &ItemUse_item_premium_sechs_monate;
	pNewscript->RegisterSelf();
}