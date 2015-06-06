#include "precompiled.h"
#include "dire_maul.h"

bool KnotQuestRewarded(Player*, Creature* cr, Quest const* quest)
{
	if((quest->GetQuestId() != 5525 && quest->GetQuestId() != 7429) || cr->GetEntry() != 14338)
	{
		return false;
	}

	if(cr->GetInstanceData() == nullptr)
		return false;

	bool spawned = ((ScriptedInstance*)cr->GetInstanceData())->GetData(TYPE_KNOT_CHEST) == DONE;
	if(spawned)
		return false;

	auto existing = GetClosestGameObjectWithEntry(cr, GO_KNOTS_CACHE, 100.0f);
	if(existing)
	{
		return false;
	}

	cr->SummonObject(cr->GetMap(), GO_KNOTS_CACHE, 583.688f, 523.646f, -25.401f, 1.545f);
	return true;
}

void KnotQuestAdd() {
	Script* s = new Script();
	s->Name = "knot_quest_cache";
	s->pQuestRewardedNPC = KnotQuestRewarded;

	s->RegisterSelf();
}