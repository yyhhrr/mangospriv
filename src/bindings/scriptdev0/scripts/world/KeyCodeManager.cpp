#include "precompiled.h"
#include "../system/system.h"
#include "Database/DatabaseEnv.h"
#include "KeyCodeManager.h"


class QueryResultHandle
{
	QueryResult* mResult;

public:
	QueryResultHandle(QueryResult* result) :
		mResult(result)
	{
	}

	~QueryResultHandle()
	{
		delete mResult;
	}

	QueryResult* operator->()
	{
		return mResult;
	}

	bool IsValid() const
	{
		return mResult != nullptr;
	}
};

MANGOS_EXPORT bool KeyCodeManager::IsValidKey(const std::string& key)
{
	QueryResultHandle queryResult = SD0Database.PQuery("SELECT used FROM premium_key WHERE keycode = '%s'", key.c_str());

	if(queryResult.IsValid() == false)
		return false;

	return queryResult->GetRowCount() > 0 && queryResult->Fetch()->GetBool() == false;
}
	
MANGOS_EXPORT bool KeyCodeManager::RemoveKey(const std::string& key)
{
	return SD0Database.PExecute("UPDATE premium_key SET used = 1 WHERE keycode = '%s'", key.c_str());
}

MANGOS_EXPORT std::vector<std::pair<int, std::string>> KeyCodeManager::GetItems()
{
	std::vector<std::pair<int, std::string>> vec;

    QueryResultHandle queryResult = SD0Database.Query("SELECT * FROM premium_item");
	if(queryResult.IsValid() == false)
		return vec;
	
    do
    {
        Field* fields = queryResult->Fetch();
		if(fields == nullptr)
			return vec;

        auto id = fields[0].GetInt32();
		auto name = fields[2].GetString();

		vec.push_back(std::pair<int, std::string>(id, name));

    } while (queryResult->NextRow());

	return vec;
}

MANGOS_EXPORT uint32 KeyCodeManager::GetItemId(int index)
{
	std::vector<std::pair<int, std::string>> vec;

    QueryResultHandle queryResult = SD0Database.PQuery("SELECT itemid FROM premium_item WHERE id = %u", index);
	if(queryResult.IsValid() == false)
		return 0;
	
    Field* fields = queryResult->Fetch();
	if(fields == nullptr)
		return 0;

	return fields[0].GetUInt32();
}