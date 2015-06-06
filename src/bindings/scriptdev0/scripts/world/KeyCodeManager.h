#pragma once
#include "Common.h"


class KeyCodeManager
{
public:
	MANGOS_EXPORT static bool IsValidKey(const std::string& key);
	MANGOS_EXPORT static bool RemoveKey(const std::string& key);

	MANGOS_EXPORT static std::vector<std::pair<int, std::string>> GetItems();
	MANGOS_EXPORT static uint32 GetItemId(int index);
};