#include "Common.h"
#include "Premade.h"

namespace Premade
{
	uint32 magePre[] = { 22267, 22403, 11782, 0, 14152, 11662, 13170, 11822, 18497, 13253, 13001, 13001, 12930, 22268, 20697, 22383, 22253, 13938, 13968, 22231 };
	uint32 mageEntry[] = { 22267, 18814, 23264, 0, 19145, 19136, 23304, 16437, 16799, 16540, 19147, 19147, 18820, 22268, 20697, 23466, 19310, 19130, 12930, 13968 };
	uint32 mageBwl[] = { 19375, 18814, 23264, 0, 19145, 19136, 22883, 16539, 21186, 16913, 19403, 19147, 18820, 19379, 19857, 19356, 0, 19861, 19147, 19438, 19897, 19130, 23451, 19311, 19310, 13968, 19339 };

	uint32 hunterPre[] = { 18421, 19491, 12927, 11726, 18393, 18524, 13967, 13211, 13963, 13098, 19325, 18473, 13965, 13340, 18520, 18737, 18738, 18737 };
	uint32 hunterEntry[] = { 16846, 16850, 16848, 16852, 16845, 16851, 16847, 16849, 18404, 17102, 13340, 17063, 19325, 18821, 18473, 13965, 19865, 18832, 19859, 18805, 20488 };
	uint32 hunterBwl[] = { 16939, 16937, 16942, 16935, 16940, 16936, 16938, 16941, 19377, 18404, 17102, 13340, 17063, 19325, 19384, 18821, 19406, 13965, 19865, 19362, 19859, 19346, 19361, 20488 };
	uint32 hunterAq[] = { 21366, 21367, 21370, 16935, 18812, 21599, 16940, 21586, 16936, 21368, 21651, 21365, 21664, 19377, 21710, 18541, 21677, 21596, 21205, 21180, 23570, 13965, 21673, 19859, 21635, 19361, 23557, 21616 };
	uint32 hunterNaxx[] = { 22438, 22439, 22436, 22443, 22441, 21586, 22442, 22437, 22440, 23053, 19377, 23045, 21710, 23067, 21677, 22961, 23041, 22954, 22816, 22802, 23039, 22811, 22812 };

	uint32 shadowPriestBwl[] = { 22267, 21635, 19370, 14136, 21186, 19407, 19400, 19133, 19131, 18814, 19857, 19434, 19403, 19147, 19397, 19379, 12930, 18820, 17064, 18609, 19360, 19309, 19861 };

	uint32 furyWarriorPre[] = { 12640, 15411, 12927, 13340, 11726, 12936, 13963, 14551, 13142, 22385, 14616, 13098, 17713, 11815, 13965, 19289, 18323, 11684, 11815, 12590 };
	uint32 furyWarriorEntry[] = { 12640, 18404, 12927, 18541, 11726, 18812, 12936, 19143, 14551, 19137, 22385, 14616, 18821, 17063, 11815, 13965, 19289, 17069, 17068, 12590, 17075, 11815 };
	uint32 furyWarriorBwl[] = { 12640, 19377, 18404, 12927, 19394, 18541, 11726, 18812, 12936, 19143, 14551, 19137, 22385, 19387, 18821, 19384, 11815, 19406, 17069, 19352, 19019, 19363, 19351 };
	uint32 furyWarriorAQ[] = { 12640, 19377, 18404, 21664, 21330, 18541, 21701, 21814, 21602, 21581, 19137, 22385, 19387, 18821, 21677, 19384, 11815, 19406, 17069, 21459, 21650, 21126, 21673, 21837, 19019 };
	uint32 furyWarriorNaxx[] = { 12640, 19377, 18404, 21664, 21330, 18541, 21701, 23000, 21602, 21581, 19137, 23219, 23068, 19387, 23038, 19384, 18821, 11815, 22954, 19406, 17069, 21459, 23054, 23242, 23014, 19019 };

	uint32 healShamiePre[] = { 18308, 22234, 13346, 13179, 18367, 18327, 18386, 18318, 13141, 18510, 19140, 18314, 18371, 18371, 22315, 17718, 23200 };
	uint32 healShamieEntry[] = { 16842, 16844, 16841, 16840, 16839, 16838, 16843, 16837, 19885, 18510, 19140, 18314, 18371, 18371, 17105, 17106, 23200 };
	uint32 healShamieBwl[] = { 16842, 16844, 16950, 16943, 16948, 16838, 16946, 16949, 19885, 18510, 19397, 19382, 19395, 17064, 19347, 19348, 23200 };
	uint32 healShamieAq[] = { 16842, 16844, 16950, 16943, 16948, 16838, 16946, 16949, 21712, 21583, 21681, 21620, 19395, 17064, 21839, 21610, 22396 };
	uint32 healShamieNaxx[] = { 22466, 22467, 22464, 22471, 22469, 22470, 22465, 22468, 23036, 22960, 22939, 23065, 23047, 23027, 23056, 22819, 22396 };

	uint32 warlockPre[] = { 22267, 22403, 18681, 11623, 14136, 18497, 13396, 18407, 11662, 13170, 18735, 22433, 22383, 19309, 13001, 12930, 13968 };
	uint32 warlockEntry[] = { 22267, 18814, 18681, 19857, 19682, 21186, 17103, 19891, 19861, 19929, 19136, 19683, 19133, 19684, 19893, 19905, 18820, 12930 };
	uint32 warlockBwl[] = { 19375, 18814, 19370, 19857, 19682, 21186, 23466, 19356, 19891, 19861, 19407, 19136, 19683, 19684, 19434, 19403, 19379, 18820 };
	uint32 warlockAq[] = { 21337, 21608, 21335, 22731, 21334, 21186, 21622, 21597, 21603, 21585, 22730, 21336, 21338, 21709, 21417, 19379, 18820, 19683, 19682, 19684 };
	uint32 warlockNaxx[] = { 22506, 23057, 22507, 23050, 22504, 21186, 22630, 22821, 22509, 22510, 23070, 22508, 21709, 23031, 19379, 23046 };

	uint32 healDruidPre[] = { 13102, 18723, 22234, 18510, 13346, 18525, 18309, 19162, 18386, 22247, 13178, 22681, 18470, 18371, 11923, 19312 };
	uint32 healDruidEntry[] = { 16900, 19885, 18810, 18510, 13346, 19840, 18309, 19162, 18875, 19892, 19140, 19140, 18470, 18371, 19890, 19312 };
	uint32 healDruidBwl[] = { 16900, 19885, 18810, 19430, 16897, 16904, 16899, 19162, 16901, 19437, 19140, 19382, 18470, 19395, 19360, 19312, 19140, 18810 };
	uint32 healDruidAq[] = { 21615, 21712, 18810, 21583, 21663, 16904, 16899, 21582, 16901, 19437, 21620, 19382, 18470, 19395, 21839, 21666 };
	uint32 healDruidNaxx[] = { 21615, 21712, 22491, 22960, 22488, 22495, 22493, 22494, 22489, 22492, 21620, 23064, 23047, 19395, 22632, 22398 };

	std::vector<uint32> GetMage(Content c)
	{
		if (c == Content::PreRaid)
			return std::vector<uint32>(magePre, magePre + 19);
		else if (c == Content::Entry)
			return std::vector<uint32>(mageEntry, mageEntry + 19);
		else if (c == Content::BWL)
			return std::vector<uint32>(mageBwl, mageBwl + 26);

		return std::vector<uint32>();
	}

	std::vector<uint32> GetHunter(Content c)
	{
		if (c == Content::PreRaid)
			return std::vector<uint32>(hunterPre, hunterPre + 18);
		else if (c == Content::Entry)
			return std::vector<uint32>(hunterEntry, hunterEntry + 21);
		else if (c == Content::BWL)
			return std::vector<uint32>(hunterBwl, hunterBwl + 24);
		else if (c == Content::AQ)
			return std::vector<uint32>(hunterAq, hunterAq + 28);
		else if (c == Content::Naxx)
			return std::vector<uint32>(hunterNaxx, hunterNaxx + 23);

		return std::vector<uint32>();
	}

	std::vector<uint32> GetPriest(Content c)
	{
		if (c == Content::BWL)
			return std::vector<uint32>(shadowPriestBwl, shadowPriestBwl + 23);
		return std::vector<uint32>();
	}

	std::vector<uint32> GetWarrior(Content c)
	{
		if (c == Content::PreRaid)
			return std::vector<uint32>(furyWarriorPre, furyWarriorPre + 20);
		else if (c == Content::Entry)
			return std::vector<uint32>(furyWarriorEntry, furyWarriorEntry + 22);
		else if (c == Content::BWL)
			return std::vector<uint32>(furyWarriorBwl, furyWarriorBwl + 23);
		else if (c == Content::AQ)
			return std::vector<uint32>(furyWarriorAQ, furyWarriorAQ + 25);
		else if (c == Content::Naxx)
			return std::vector<uint32>(furyWarriorNaxx, furyWarriorNaxx + 26);

		return std::vector<uint32>();
	}

	std::vector<uint32> GetShaman(Content c)
	{
		if (c == Content::PreRaid)
			return std::vector<uint32>(healShamiePre, healShamiePre + 17);
		else if (c == Content::Entry)
			return std::vector<uint32>(healShamieEntry, healShamieEntry + 17);
		else if (c == Content::BWL)
			return std::vector<uint32>(healShamieBwl, healShamieBwl + 17);
		else if (c == Content::AQ)
			return std::vector<uint32>(healShamieAq, healShamieAq + 17);
		else if (c == Content::Naxx)
			return std::vector<uint32>(healShamieNaxx, healShamieNaxx + 17);

		return std::vector<uint32>();
	}

	std::vector<uint32> GetWarlock(Content c)
	{
		if (c == Content::PreRaid)
			return std::vector<uint32>(warlockPre, warlockPre + 17);
		else if (c == Content::Entry)
			return std::vector<uint32>(warlockEntry, warlockEntry + 18);
		else if (c == Content::BWL)
			return std::vector<uint32>(warlockBwl, warlockBwl + 18);
		else if (c == Content::AQ)
			return std::vector<uint32>(warlockAq, warlockAq + 20);
		else if (c == Content::Naxx)
			return std::vector<uint32>(warlockNaxx, warlockNaxx + 16);

		return std::vector<uint32>();
	}

	std::vector<uint32> GetDruid(Content c)
	{
		if (c == Content::PreRaid)
			return std::vector<uint32>(healDruidPre, healDruidPre + 16);
		else if (c == Content::Entry)
			return std::vector<uint32>(healDruidEntry, healDruidEntry + 16);
		else if (c == Content::BWL)
			return std::vector<uint32>(healDruidBwl, healDruidBwl + 18);
		else if (c == Content::AQ)
			return std::vector<uint32>(healDruidAq, healDruidAq + 16);
		else if (c == Content::Naxx)
			return std::vector<uint32>(healDruidNaxx, healDruidNaxx + 16);

		return std::vector<uint32>();
	}

}