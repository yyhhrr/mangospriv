#include "HarveyManager.h"
#include "Database/DatabaseEnv.h"
#include "Database/DatabaseImpl.h"
#include "Chat.h"
#include "Policies/Singleton.h"

INSTANTIATE_SINGLETON_1(HarveyManager);

void HarveyManager::initTable() {
	CharacterDatabase.AsyncPQuery(this, &HarveyManager::onQueryResult, "SELECT * FROM `harvey`");
}

void HarveyManager::onQueryResult(QueryResult* result) {
	if(result == nullptr) {
		return;
	}

	if(result->GetRowCount() == 0) {
		delete result;
		return;
	}

	time_t now = time(nullptr);
	mMaxID = 0;

	do {
		Field* fields = result->Fetch();
		uint32 type = fields[1].GetUInt32();
		uint64 uuid = fields[2].GetUInt64();
		uint64 expire = fields[3].GetUInt64();

		if(expire != 0 && expire <= (uint64)now) {
			std::stringstream strm;
			strm << "DELETE FROM `harvey` WHERE `id` = '" << fields[0].GetUInt32() << "'";
			CharacterDatabase.DirectExecute(strm.str().c_str());
			continue;
		}

		HarveyInfo info;
		info.id = fields[0].GetUInt32();
		info.isTemporary = expire != 0;
		info.expirationDate = expire;

		if(info.id > mMaxID) {
			mMaxID = info.id;
		}

		switch(type) {
		case 0:
			mCharacters[uuid] = info;
			break;

		case 1:
			mAccounts[uuid] = info;
			break;

		case 2:
			mIPs[uuid] = info;
			break;
		}

	} while(result->NextRow());

	delete result;
}

bool HarveyManager::checkHarvey(uint32 acct, uint64 guid, const std::string& ip) {
	mQueryLock.acquire();

	auto itrAcc = mAccounts.find(acct);
	auto itrChr = mCharacters.find(guid);
	auto itrIP = mIPs.find(inet_addr(ip.c_str()));

	if(itrAcc == mAccounts.end() && itrChr == mCharacters.end() && itrIP == mIPs.end()) {
		mQueryLock.release();
		return false;
	}

	time_t now = time(nullptr);
	if(itrAcc != mAccounts.end()) {
		auto& info = itrAcc->second;
		if(info.isTemporary == false) {
			mQueryLock.release();
			return true;
		}

		if(info.expirationDate <= (uint64)now) {
			std::stringstream strm;
			strm << "DELETE FROM `harvey` WHERE `id` = '" << info.id << "'";
			CharacterDatabase.DirectExecute(strm.str().c_str());

			mAccounts.erase(itrAcc);
		} else {
			mQueryLock.release();
			return true;
		}
	}

	if(itrChr != mCharacters.end()) {
		auto& info = itrChr->second;
		if(info.isTemporary == false) {
			mQueryLock.release();
			return true;
		}

		if(info.expirationDate <= (uint64)now) {
			std::stringstream strm;
			strm << "DELETE FROM `harvey` WHERE `id` = '" << info.id << "'";
			CharacterDatabase.DirectExecute(strm.str().c_str());

			mCharacters.erase(itrChr);
		} else {
			mQueryLock.release();
			return true;
		}
	}

	if(itrIP != mIPs.end()) {
		auto& info = itrIP->second;
		if(info.isTemporary == false) {
			mQueryLock.release();
			return true;
		}

		if(info.expirationDate <= (uint64)now) {
			std::stringstream strm;
			strm << "DELETE FROM `harvey` WHERE `id` = '" << info.id << "'";
			CharacterDatabase.DirectExecute(strm.str().c_str());
			mIPs.erase(itrIP);
		} else {
			mQueryLock.release();
			return true;
		}
	}

	mQueryLock.release();

	// unreachable
	return false;
}

void HarveyManager::addAccount(uint32 acct, bool temp, uint64 numSecs) {
	mQueryLock.acquire();

	HarveyInfo info;
	info.id = ++mMaxID;
	info.isTemporary = temp;
	info.expirationDate = temp ? (uint64)(time(nullptr) + numSecs) : 0;

	auto inserted = mAccounts.insert(std::make_pair(acct, info));
	if(inserted.second == false) {
		mAccounts[acct] = info;
	}

	std::stringstream strm;
	strm << "INSERT INTO `harvey` VALUES ('" << info.id << "', '" << 1 << "', '" << acct << "', '" << info.expirationDate << "')";
	CharacterDatabase.DirectExecute(strm.str().c_str());
	mQueryLock.release();
}

void HarveyManager::addCharacter(uint64 guid, bool temp, uint64 numSecs) {
	mQueryLock.acquire();

	HarveyInfo info;
	info.id = ++mMaxID;
	info.isTemporary = temp;
	info.expirationDate = temp ? (uint64)(time(nullptr) + numSecs) : 0;

	auto inserted = mCharacters.insert(std::make_pair(guid, info));
	if(inserted.second == false) {
		mCharacters[guid] = info;
	}

	std::stringstream strm;
	strm << "INSERT INTO `harvey` VALUES ('" << info.id << "', '" << 0 << "', '" << guid << "', '" << info.expirationDate << "')";
	CharacterDatabase.DirectExecute(strm.str().c_str());
	mQueryLock.release();
}

void HarveyManager::addIP(const std::string& ip, bool temp, uint64 numSecs) {
	mQueryLock.acquire();

	HarveyInfo info;
	info.id = ++mMaxID;
	info.isTemporary = temp;
	info.expirationDate = temp ? (uint64)(time(nullptr) + numSecs) : 0;

	auto inserted = mIPs.insert(std::make_pair(inet_addr(ip.c_str()), info));
	if(inserted.second == false) {
		mIPs[inet_addr(ip.c_str())] = info;
	}

	std::stringstream strm;
	strm << "INSERT INTO `harvey` VALUES ('" << info.id << "', '" << 2 << "', '" << inet_addr(ip.c_str()) << "', '" << info.expirationDate << "')";
	CharacterDatabase.DirectExecute(strm.str().c_str());

	mQueryLock.release();
}

void HarveyManager::removeAccount(uint32 acct) {
	mQueryLock.acquire();

	auto itr = mAccounts.find(acct);
	if(itr != mAccounts.end()) {
		std::stringstream strm;
		strm << "DELETE FROM `harvey` WHERE `id` = '" << itr->second.id << "'";
		CharacterDatabase.DirectExecute(strm.str().c_str());

		mAccounts.erase(itr);
	}

	mQueryLock.release();
}

void HarveyManager::removeCharacter(uint64 guid) {
	mQueryLock.acquire();

	auto itr = mCharacters.find(guid);
	if(itr != mCharacters.end()) {
		std::stringstream strm;
		strm << "DELETE FROM `harvey` WHERE `id` = '" << itr->second.id << "'";
		CharacterDatabase.DirectExecute(strm.str().c_str());

		mCharacters.erase(itr);
	}

	mQueryLock.release();
}

void HarveyManager::removeIP(const std::string& ip) {
	mQueryLock.acquire();

	auto itr = mIPs.find(inet_addr(ip.c_str()));
	if(itr != mIPs.end()) {
		std::stringstream strm;
		strm << "DELETE FROM `harvey` WHERE `id` = '" << itr->second.id << "'";
		CharacterDatabase.DirectExecute(strm.str().c_str());

		mIPs.erase(itr);
	}

	mQueryLock.release();
}

bool HarveyManager::process(WorldSession* session, uint32 acct, uint64 guid, WorldPacket& packet) {
	uint32 type;
    uint32 lang;

    packet >> type;
    packet >> lang;

	std::string message;

	if(type >= MAX_CHAT_MSG_TYPE) {
		return false;
	}

	if(lang == LANG_ADDON) {
		return false;
	}

	bool isHarvey = true;

	if(checkHarvey(acct, guid, session->GetRemoteAddress()) == false) {
		isHarvey = false;
	}

	bool hasMessage = false;


	switch(type) {
	case CHAT_MSG_SAY:
	case CHAT_MSG_YELL:
	case CHAT_MSG_GUILD:
	case CHAT_MSG_EMOTE:
	case CHAT_MSG_OFFICER:
	case CHAT_MSG_PARTY:
	case CHAT_MSG_BATTLEGROUND:
	case CHAT_MSG_BATTLEGROUND_LEADER:
	case CHAT_MSG_RAID:
	case CHAT_MSG_RAID_WARNING:
	case CHAT_MSG_WHISPER:
	case CHAT_MSG_CHANNEL:
		break;

	default:
		return false;
	}

	WorldPacket resp(SMSG_MESSAGECHAT);

	switch(type) {
	case CHAT_MSG_SAY:
	case CHAT_MSG_YELL:
	case CHAT_MSG_GUILD:
	case CHAT_MSG_EMOTE:
	case CHAT_MSG_OFFICER:
	case CHAT_MSG_PARTY:
	case CHAT_MSG_BATTLEGROUND:
	case CHAT_MSG_BATTLEGROUND_LEADER:
	case CHAT_MSG_RAID:
	case CHAT_MSG_RAID_WARNING:
		{
			hasMessage = true;
			packet >> message;
			if(isHarvey == true) {
				ChatHandler::FillMessageData(&resp, session, type, LANG_UNIVERSAL, message.c_str());
			}
		}
		break;

	case CHAT_MSG_WHISPER:
		{
			hasMessage = true;
			std::string to;
			packet >> to >> message;
			if(isHarvey == true) {
				ChatHandler::FillMessageData(&resp, nullptr, CHAT_MSG_WHISPER_INFORM, LANG_UNIVERSAL, ObjectGuid(), message.c_str());
			}
		}
		break;

	case CHAT_MSG_CHANNEL:
		{
			hasMessage = true;
			std::string channel;
			packet >> channel >> message;
			resp << uint8(CHAT_MSG_CHANNEL);
			resp << uint32(LANG_UNIVERSAL);
			resp << channel;
			resp << uint32(0);
			resp << guid;
			resp << uint32(message.length() + 1);
			resp << message;
			resp << uint8(0);
		}
		break;
	}

	if(isHarvey == true) {
		session->SendPacket(&resp);
	}

	if(hasMessage && isHarvey == false) {
		auto itr = mFilters.find(guid);
		if(itr != mFilters.end()) {
			isHarvey = itr->second.addMessage(message);
			if(isHarvey && hasMessage) {
				session->SendPacket(&resp);
			}
		} else {
			mFilters.insert(std::make_pair(guid, ChatRepeatFilter(guid)));
		}
	}

	return isHarvey;
}

bool HarveyManager::ChatRepeatFilter::addMessage(std::string message) {
	uint64 now = (uint64) time(nullptr);

	std::transform(message.begin(), message.end(), message.begin(), tolower);

	auto end = std::remove_if(mSimMessageChain.begin(), mSimMessageChain.end(), 
		[&now](MessageEntry& e) {
			return (e.timeStamp + 60 < now);
		});

	if(end != mSimMessageChain.end()) {
		mSimMessageChain.erase(end, mSimMessageChain.end());
	}

	MessageEntry me;
	me.message = message;
	me.timeStamp = now;

	if(mSimMessageChain.size() == 0) {
		mSimMessageChain.push_back(me);
		return false;
	}

	if(mSimMessageChain.front().message != message) {
		mSimMessageChain.clear();
		mSimMessageChain.push_back(me);
		return false;
	}

	mSimMessageChain.push_back(me);

	if(mSimMessageChain.size() < 6) {
		return false;
	}

	MessageEntry& oe = mSimMessageChain.front();
	int64 diff = now - oe.timeStamp;
	if(diff < 60) {
		sHarvey.addCharacter(mGuid, true, 300);
		return true;
	}

	return false;
}