#include "precompiled.h"
#include "ProgressMonitor.h"

namespace ProgressMonitor
{
	void Track(Player* plr, Raids raid, Bosses boss)
	{
		if (!plr->GetGroup())
		{
			plr->UpdateRaidProgress(static_cast<uint32>(raid), static_cast<uint32>(boss));
			return;
		}
		
		auto* memberRef = plr->GetGroup()->GetFirstMember();
		do
		{
			if (auto* pl = memberRef->getSource())
				pl->UpdateRaidProgress(static_cast<uint32>(raid), static_cast<uint32>(boss));

		} while (memberRef = memberRef->next());

	}
}