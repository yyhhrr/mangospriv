/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos-zero>
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

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Player.h"
#include "Opcodes.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "revision_nr.h"
#include "Util.h"
#include "GossipDef.h"
#include "Premade.h"
#include "SpellMgr.h"

bool ChatHandler::HandleHelpCommand(char* args)
{
    if(!*args)
    {
        ShowHelpForCommand(getCommandTable(), "help");
        ShowHelpForCommand(getCommandTable(), "");
    }
    else
    {
        if (!ShowHelpForCommand(getCommandTable(), args))
            SendSysMessage(LANG_NO_CMD);
    }

    return true;
}

bool ChatHandler::HandleCommandsCommand(char* /*args*/)
{
    ShowHelpForCommand(getCommandTable(), "");
    return true;
}

bool ChatHandler::HandleAccountCommand(char* args)
{
    // let show subcommands at unexpected data in args
    if (*args)
        return false;

    AccountTypes gmlevel = GetAccessLevel();
    PSendSysMessage(LANG_ACCOUNT_LEVEL, uint32(gmlevel));
    return true;
}

bool ChatHandler::HandleStartCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    if(chr->IsTaxiFlying())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    if(chr->isInCombat())
    {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
    }

    // cast spell Stuck
    chr->CastSpell(chr,7355,false);
    return true;
}

bool ChatHandler::HandleServerInfoCommand(char* /*args*/)
{
    uint32 activeClientsNum = sWorld.GetActiveSessionCount();
    uint32 queuedClientsNum = sWorld.GetQueuedSessionCount();
    uint32 maxActiveClientsNum = sWorld.GetMaxActiveSessionCount();
    uint32 maxQueuedClientsNum = sWorld.GetMaxQueuedSessionCount();
    std::string str = secsToTimeString(sWorld.GetUptime());

    char const* full;
	full = _FULLVERSION(REVISION_DATE, REVISION_TIME, REVISION_NR);
    SendSysMessage(full);

    if (sScriptMgr.IsScriptLibraryLoaded())
    {
        char const* ver = sScriptMgr.GetScriptLibraryVersion();
        if (ver && *ver)
            PSendSysMessage(LANG_USING_SCRIPT_LIB, ver);
        else
            SendSysMessage(LANG_USING_SCRIPT_LIB_UNKNOWN);
    }
    else
        SendSysMessage(LANG_USING_SCRIPT_LIB_NONE);

    PSendSysMessage(LANG_USING_WORLD_DB,sWorld.GetDBVersion());
    PSendSysMessage(LANG_USING_EVENT_AI,sWorld.GetCreatureEventAIVersion());
    PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);
    PSendSysMessage(LANG_UPTIME, str.c_str());

    return true;
}

bool ChatHandler::HandleDismountCommand(char* /*args*/)
{
    //If player is not mounted, so go out :)
    if (!m_session->GetPlayer( )->IsMounted())
    {
        SendSysMessage(LANG_CHAR_NON_MOUNTED);
        SetSentErrorMessage(true);
        return false;
    }

    if(m_session->GetPlayer( )->IsTaxiFlying())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    m_session->GetPlayer()->Unmount();
    m_session->GetPlayer()->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
    return true;
}

bool ChatHandler::HandleSaveCommand(char* /*args*/)
{
    Player *player=m_session->GetPlayer();

    // save GM account without delay and output message (testing, etc)
    if(GetAccessLevel() > SEC_PLAYER)
    {
        player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save or plan save after 20 sec (logout delay) if current next save time more this value and _not_ output any messages to prevent cheat planning
    uint32 save_interval = sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVE);
    if (save_interval==0 || (save_interval > 20*IN_MILLISECONDS && player->GetSaveTimer() <= save_interval - 20*IN_MILLISECONDS))
        player->SaveToDB();

    return true;
}

bool ChatHandler::HandleGMListIngameCommand(char* /*args*/)
{
    std::list< std::pair<std::string, bool> > names;

    {
        HashMapHolder<Player>::ReadGuard g(HashMapHolder<Player>::GetLock());
        HashMapHolder<Player>::MapType &m = sObjectAccessor.GetPlayers();
        for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        {
            AccountTypes itr_sec = itr->second->GetSession()->GetSecurity();
            if ((itr->second->isGameMaster() || (itr_sec > SEC_PLAYER && itr_sec <= (AccountTypes)sWorld.getConfig(CONFIG_UINT32_GM_LEVEL_IN_GM_LIST))) &&
                (!m_session || itr->second->IsVisibleGloballyFor(m_session->GetPlayer())))
                names.push_back(std::make_pair<std::string, bool>(GetNameLink(itr->second), itr->second->isAcceptWhispers()));
        }
    }

    if (!names.empty())
    {
        SendSysMessage(LANG_GMS_ON_SRV);

        char const* accepts = GetMangosString(LANG_GM_ACCEPTS_WHISPER);
        char const* not_accept = GetMangosString(LANG_GM_NO_WHISPER);
        for(std::list<std::pair< std::string, bool> >::const_iterator iter = names.begin(); iter != names.end(); ++iter)
            PSendSysMessage("%s - %s", iter->first.c_str(), iter->second ? accepts : not_accept);
    }
    else
        SendSysMessage(LANG_GMS_NOT_LOGGED);

    return true;
}

bool ChatHandler::HandleAccountPasswordCommand(char* args)
{
    // allow use from RA, but not from console (not have associated account id)
    if (!GetAccountId())
    {
        SendSysMessage (LANG_RA_ONLY_COMMAND);
        SetSentErrorMessage (true);
        return false;
    }

    // allow or quoted string with possible spaces or literal without spaces
    char *old_pass = ExtractQuotedOrLiteralArg(&args);
    char *new_pass = ExtractQuotedOrLiteralArg(&args);
    char *new_pass_c = ExtractQuotedOrLiteralArg(&args);

    if (!old_pass || !new_pass || !new_pass_c)
        return false;

    std::string password_old = old_pass;
    std::string password_new = new_pass;
    std::string password_new_c = new_pass_c;

    if (password_new != password_new_c)
    {
        SendSysMessage (LANG_NEW_PASSWORDS_NOT_MATCH);
        SetSentErrorMessage (true);
        return false;
    }

    if (!sAccountMgr.CheckPassword (GetAccountId(), password_old))
    {
        SendSysMessage (LANG_COMMAND_WRONGOLDPASSWORD);
        SetSentErrorMessage (true);
        return false;
    }

    AccountOpResult result = sAccountMgr.ChangePassword(GetAccountId(), password_new);

    switch(result)
    {
        case AOR_OK:
            SendSysMessage(LANG_COMMAND_PASSWORD);
            break;
        case AOR_PASS_TOO_LONG:
            SendSysMessage(LANG_PASSWORD_TOO_LONG);
            SetSentErrorMessage(true);
            return false;
        case AOR_NAME_NOT_EXIST:                            // not possible case, don't want get account name for output
        default:
            SendSysMessage(LANG_COMMAND_NOTCHANGEPASSWORD);
            SetSentErrorMessage(true);
            return false;
    }

    // OK, but avoid normal report for hide passwords, but log use command for anyone
    LogCommand(".account password *** *** ***");
    SetSentErrorMessage(true);
    return false;
}

bool ChatHandler::HandleAccountLockCommand(char* args)
{
    // allow use from RA, but not from console (not have associated account id)
    if (!GetAccountId())
    {
        SendSysMessage (LANG_RA_ONLY_COMMAND);
        SetSentErrorMessage (true);
        return false;
    }

    bool value;
    if (!ExtractOnOff(&args, value))
    {
        SendSysMessage(LANG_USE_BOL);
        SetSentErrorMessage(true);
        return false;
    }

    if (value)
    {
        LoginDatabase.PExecute( "UPDATE account SET locked = '1' WHERE id = '%u'", GetAccountId());
        PSendSysMessage(LANG_COMMAND_ACCLOCKLOCKED);
    }
    else
    {
        LoginDatabase.PExecute( "UPDATE account SET locked = '0' WHERE id = '%u'", GetAccountId());
        PSendSysMessage(LANG_COMMAND_ACCLOCKUNLOCKED);
    }

    return true;
}

/*
 * User can specify how experience he will get by solving quests,
 * killing NPCs or exploring.
 * Command.Rate
 *
*/
bool ChatHandler::HandleRateCommand(char* args)
{
    if(!*args)
	{
        PSendSysMessage("Deine Erfahrungsrate ist aktuell auf %d.", m_session->GetPlayer()->getXPRate());
        return false;
	}

    Player *chr = m_session->GetPlayer();

    int32 xpRate = atoi(args);
	int32 xpRateM = xpRate;


	if (chr->GetSession()->IsPremium())
	{

	}
	else
	{
		if (xpRate <= 0 || xpRateM <= 0 || xpRateM < xpRate)
		{
			SendSysMessage(LANG_BAD_VALUE);
			SetSentErrorMessage(true);
			return false;
		}
	}
	if (chr->GetSession()->IsPremium())
	{
	    if(xpRate > sWorld.getConfig(CONFIG_FLOAT_RATE_XP_KILL_PREMIUM))
		{
			xpRate = sWorld.getConfig(CONFIG_FLOAT_RATE_XP_KILL_PREMIUM);
			PSendSysMessage("Die maximale Erfahrungsrate ist %d.", xpRate);
		}
	}
	else
	{
		if(xpRate > sWorld.getConfig(CONFIG_FLOAT_RATE_XP_KILL))
		{
			xpRate = sWorld.getConfig(CONFIG_FLOAT_RATE_XP_KILL);
			PSendSysMessage("Die maximale Erfahrungsrate ist %d.", xpRate);
		}
	}

    chr->setXPRate(xpRate);
    PSendSysMessage("Deine Erfahrungsrate wurde auf: %d gesetzt.", xpRate);

    return true;
}

bool ChatHandler::HandleInstantFlightCommand(char* args)
{
    if(!*args)
	{
		PSendSysMessage("Die Schnellreise ist aktuell auf: %d.", m_session->GetPlayer()->getInstantFlight());
        return false;
	}

    Player *chr = m_session->GetPlayer();

    int32 pInstantF = atoi(args);

    if (pInstantF < 0)
    {
        SendSysMessage(LANG_BAD_VALUE);
        SetSentErrorMessage(true);
        return false;
    }

	if (chr->GetSession()->IsPremium())
	{
	    if(pInstantF == 1)
		{
			chr->setInstantFlight(1);
			PSendSysMessage("Die Schnellreise wurde aktiviert!");
		}
		else
		{
			chr->setInstantFlight(0);
			PSendSysMessage("Die Schnellreise wurde deaktiviert!");		
		}
	}
	else
	{
		PSendSysMessage("Diese Funktion ist nur f\303\274r Premium Spieler verf\303\274gbar!");
	}

    return true;
}

/// Display the 'Message of the day' for the realm
bool ChatHandler::HandleServerMotdCommand(char* /*args*/)
{
    PSendSysMessage(LANG_MOTD_CURRENT, sWorld.GetMotd());
    return true;
}

bool ChatHandler::HandleSetPvPAccountCommand(char*)
{
	Player* plr = m_session->GetPlayer();
	std::string str;

	if (m_session->GetSessionDbLocaleIndex() == -1)
		str = "continue";
	if (m_session->GetSessionDbLocaleIndex() == 0)
		str = "weiter";

	plr->PlayerTalkClass->ClearMenus();
	plr->PlayerTalkClass->GetGossipMenu().AddMenuItem(9, str.c_str(), 1, 0, 0);
	plr->PlayerTalkClass->GetGossipMenu().AddMenuItem(0, "Nope", 1, 1, 0);
	plr->PlayerTalkClass->SendGossipMenu(90215, plr->GetGUID());

	return true;
}

const uint32 oneHandAxes = 15984;
const uint32 twoHandAxes = 15985;
const uint32 oneHandMaces = 15986;
const uint32 twoHandMaces = 15987;
const uint32 polearms = 15991;
const uint32 oneHandSwords = 1847;
const uint32 twoHandSwords = 15983;
const uint32 daggers = 1180;
const uint32 staves = 15989;
const uint32 guns = 15996;
const uint32 crossbows = 15995;
const uint32 fistWeapons = 15992;
const uint32 bows = 15994;
const uint32 thrown = 15997;
const uint32 riding = 33389;
const uint32 fastRiding = 33392;
const uint32 mail = 8737;
const uint32 plate = 750;

const uint32 spell_bear_form = 5487;
const uint32 spell_growl = 6795;
const uint32 spell_maul = 6807;
const uint32 spell_dire_bear_form = 9634;
const uint32 spell_aquatic_form = 1066;
const uint32 spell_cure_poison = 8946;
const uint32 spell_gift_of_the_wild = 21850;

const uint32 spell_summon_imp = 688;
const uint32 spell_summon_voidwalker = 697;
const uint32 spell_summon_succubus = 712;
const uint32 spell_summon_felhunter = 691;
const uint32 spell_summon_inferno = 1122;
const uint32 spell_ritual_of_doom = 18540;

const uint32 spell_tame_beast = 1515;
const uint32 spell_call_pet = 883;
const uint32 spell_revive_pet = 982;
const uint32 spell_feed_pet = 6991;
const uint32 spell_dismiss_pet = 2641;
const uint32 spell_beast_training = 5149;

const uint32 spell_defense_stance = 71;
const uint32 spell_sunder_armor = 7386;
const uint32 spell_taunt = 355;
const uint32 spell_berserker_stance = 2458;
const uint32 spell_intercept = 20252;

const uint32 spell_arcane_brilliance = 23028;

const uint32 item_fire_totem = 5176;
const uint32 spell_searing_totem = 3599;
const uint32 item_earth_totem = 5175;
const uint32 spell_stoneskin_totem = 8071;
const uint32 item_air_totem = 5178;
const uint32 item_water_totem = 5177;
const uint32 spell_healing_stream_totem = 5394;

void LearnProfession(Player* plr, uint32 spell, uint32 skillId)
{
	plr->learnSpell(spell, false);
	plr->SetSkill(skillId, 300, 300);

	uint32 classmask = plr->getClassMask();

	for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
	{
		SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
		if (!skillLine)
			continue;

		// wrong skill
		if (skillLine->skillId != skillId)
			continue;

		// not high rank
		if (skillLine->forward_spellid)
			continue;

		// skip racial skills
		if (skillLine->racemask != 0)
			continue;

		// skip wrong class skills
		if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
			continue;

		SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
		if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, plr, false))
			continue;

		plr->learnSpell(skillLine->spellId, false);
	}
}

void LearnEnchanting(Player* plr)
{
	const uint32 enchanting = 13920;
	LearnProfession(plr, enchanting, 333);
	plr->StoreNewItemInBestSlots(16207, 1);
	plr->StoreNewItemInBestSlots(11145, 1);
	plr->StoreNewItemInBestSlots(11130, 1);
	plr->StoreNewItemInBestSlots(6339, 1);
	plr->StoreNewItemInBestSlots(6218, 1);
}

void GiveBasic(Player* plr)
{
	const uint32 onyxiaHideBackpack = 17966;
	const uint32 zulianTiger = 19902;

    plr->SetMoney(500000000);
    plr->GiveLevel(60);
    plr->InitTalentForLevel();
    plr->SetUInt32Value(PLAYER_XP, 0);

	for (int i = 0; i < 4; ++i)
		plr->StoreNewItemInBestSlots(onyxiaHideBackpack, 1);

	plr->StoreNewItemInBestSlots(zulianTiger, 1);

    plr->CastSpell(plr, riding, true);
    plr->CastSpell(plr, fastRiding, true);
	LearnEnchanting(plr);
}

bool ChatHandler::GiveItems(Premade::Content c, Player* plr)
{
	GiveBasic(plr);
	std::vector<uint32> items;
	
	if (plr->getClass() == CLASS_MAGE)
	{
		items = Premade::GetMage(c);
        plr->CastSpell(plr, daggers, true);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, oneHandSwords, true);
        plr->learnSpell(spell_arcane_brilliance, false);
		LearnProfession(plr, 3908, 197);
	} else if (plr->getClass() == CLASS_HUNTER)
	{
		items = Premade::GetHunter(c);
        plr->CastSpell(plr, oneHandAxes, true);
        plr->CastSpell(plr, twoHandAxes, true);
        plr->CastSpell(plr, polearms, true);
        plr->CastSpell(plr, oneHandSwords, true);
        plr->CastSpell(plr, twoHandSwords, true);
        plr->CastSpell(plr, daggers, true);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, guns, true);
        plr->CastSpell(plr, crossbows, true);
        plr->CastSpell(plr, bows, true);
        plr->CastSpell(plr, fistWeapons, true);
        plr->learnSpell(mail, false);
        plr->learnSpell(spell_tame_beast, false);
        plr->learnSpell(spell_call_pet, false);
        plr->learnSpell(spell_revive_pet, false);
        plr->learnSpell(spell_feed_pet, false);
        plr->learnSpell(spell_dismiss_pet, false);
        plr->learnSpell(spell_beast_training, false);
	} else if (plr->getClass() == CLASS_PRIEST)
	{
		items = Premade::GetPriest(c);
        plr->CastSpell(plr, daggers, true);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, oneHandSwords, true);
        plr->CastSpell(plr, oneHandMaces, true);
	} else if (plr->getClass() == CLASS_WARRIOR)
	{
		items = Premade::GetWarrior(c);
        plr->CastSpell(plr, oneHandAxes, true);
        plr->CastSpell(plr, twoHandAxes, true);
        plr->CastSpell(plr, polearms, true);
        plr->CastSpell(plr, oneHandSwords, true);
        plr->CastSpell(plr, twoHandSwords, true);
        plr->CastSpell(plr, daggers, true);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, guns, true);
        plr->CastSpell(plr, crossbows, true);
        plr->CastSpell(plr, bows, true);
        plr->CastSpell(plr, thrown, true);
        plr->CastSpell(plr, fistWeapons, true);
        plr->CastSpell(plr, oneHandMaces, true);
        plr->CastSpell(plr, twoHandMaces, true);
        plr->learnSpell(plate, false);
        plr->learnSpell(spell_defense_stance, false);
        plr->learnSpell(spell_sunder_armor, false);
        plr->learnSpell(spell_taunt, false);
        plr->learnSpell(spell_berserker_stance, false);
        plr->learnSpell(spell_intercept, false);
	} else if (plr->getClass() == CLASS_SHAMAN)
	{
		items = Premade::GetShaman(c);
		plr->CastSpell(plr, daggers, true);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, oneHandMaces, true);
        plr->learnSpell(mail, false);
        plr->StoreNewItemInBestSlots(item_air_totem, 1);
        plr->StoreNewItemInBestSlots(item_water_totem, 1);
        plr->StoreNewItemInBestSlots(item_earth_totem, 1);
        plr->StoreNewItemInBestSlots(item_fire_totem, 1);
        plr->learnSpell(spell_searing_totem, false);
        plr->learnSpell(spell_stoneskin_totem, false);
        plr->learnSpell(spell_healing_stream_totem, false);
	} else if (plr->getClass() == CLASS_WARLOCK)
	{
		items = Premade::GetWarlock(c);
        plr->CastSpell(plr, daggers, true);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, oneHandSwords, true);
        plr->learnSpell(spell_summon_imp, false);
        plr->learnSpell(spell_summon_voidwalker, false);
        plr->learnSpell(spell_summon_succubus, false);
        plr->learnSpell(spell_summon_felhunter, false);
        plr->learnSpell(spell_summon_inferno, false);
        plr->learnSpell(spell_ritual_of_doom, false);
		LearnProfession(plr, 3908, 197);
	} else if (plr->getClass() == CLASS_DRUID)
	{
		items = Premade::GetDruid(c);
        plr->CastSpell(plr, staves, true);
        plr->CastSpell(plr, oneHandMaces, true);
        plr->learnSpell(spell_bear_form, false);
        plr->learnSpell(spell_growl, false);
        plr->learnSpell(spell_maul, false);
        plr->learnSpell(spell_dire_bear_form, false);
        plr->learnSpell(spell_aquatic_form, false);
        plr->learnSpell(spell_cure_poison, false);
        plr->learnSpell(spell_gift_of_the_wild, false);
	}

	if (items.empty())
	{
		SendSysMessage("Derzeit sind keine Items fuer diese Klasse vorhanden. Wenn du das aendern moechtest, dann schaue bitte im Forum nach (http://www.mmobase.de/forum/classic-server-news/6291-itemsammlung-testserver-premades.html).");
		return true;
	}

	uint8 slot = 0;
	for (auto& item : items)
	{
		if (slot == EQUIPMENT_SLOT_TABARD)
			slot = INVENTORY_SLOT_ITEM_START;

		if (item)
			plr->StoreNewItemInBestSlots(item, 1);

		++slot;
	}

    plr->UpdateSkillsToMaxSkillsForLevel();

	return true;
}

bool ChatHandler::HandlePremadePreCommand(char* args)
{
	std::vector<uint32> items;
	auto* plr = m_session->GetPlayer();

	return GiveItems(Premade::Content::PreRaid, plr);
}

bool ChatHandler::HandlePremadeEntryCommand(char* args)
{
	std::vector<uint32> items;
	auto* plr = m_session->GetPlayer();

	return GiveItems(Premade::Content::Entry, plr);
}

bool ChatHandler::HandlePremadeBWLCommand(char* args)
{
	std::vector<uint32> items;
	auto* plr = m_session->GetPlayer();

	return GiveItems(Premade::Content::BWL, plr);
}

bool ChatHandler::HandlePremadeAQCommand(char* args)
{
	std::vector<uint32> items;
	auto* plr = m_session->GetPlayer();

	return GiveItems(Premade::Content::AQ, plr);
}

bool ChatHandler::HandlePremadeNaxxCommand(char* args)
{
	std::vector<uint32> items;
	auto* plr = m_session->GetPlayer();

	return GiveItems(Premade::Content::Naxx, plr);
}