#pragma once
#include <vector>

namespace Premade
{
	enum class Content
	{
		PreRaid,
		Entry,
		BWL,
		AQ,
		Naxx
	};

	std::vector<uint32> GetMage(Content c);
	std::vector<uint32> GetHunter(Content c);
	std::vector<uint32> GetPriest(Content c);
	std::vector<uint32> GetWarrior(Content c);
	std::vector<uint32> GetShaman(Content c);
	std::vector<uint32> GetWarlock(Content c);
	std::vector<uint32> GetDruid(Content c);
}