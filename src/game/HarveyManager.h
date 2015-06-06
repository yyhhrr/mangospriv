#pragma once

#include "Common.h"

class HarveyManager
{
	struct HarveyInfo
	{
		uint32 id;
		bool isTemporary;
		uint64 expirationDate;
	};

	class ChatRepeatFilter
	{
		struct MessageEntry
		{
			uint64 timeStamp;
			std::string message;
		};

		std::list<MessageEntry> mSimMessageChain;
		uint64 mGuid;

	public:
		ChatRepeatFilter() : mGuid(0) {

		}

		ChatRepeatFilter(uint64 guid) : mGuid(guid) {
		
		}

		bool addMessage(std::string message);
	};

	ACE_Thread_Mutex mQueryLock;

	std::map<uint32, HarveyInfo> mAccounts;
	std::map<uint64, HarveyInfo> mCharacters;
	std::map<uint32, HarveyInfo> mIPs;
	std::map<uint64, ChatRepeatFilter> mFilters;

	uint32 mMaxID;

	void onQueryResult(QueryResult* result);

public:
	void initTable();

	bool checkHarvey(uint32 acct, uint64 guid, const std::string& ip);

	void addIP(const std::string& ip, bool temporary, uint64 numSeconds);
	void addAccount(uint32 acct, bool temporary, uint64 numSeconds);
	void addCharacter(uint64 guid, bool temporary, uint64 numSeconds);
	void removeAccount(uint32 acct);
	void removeCharacter(uint64 guid);
	void removeIP(const std::string& ip);

	bool process(WorldSession* session, uint32 accout, uint64 guid, WorldPacket& packet);
};

#define sHarvey (MaNGOS::Singleton<HarveyManager>::Instance())