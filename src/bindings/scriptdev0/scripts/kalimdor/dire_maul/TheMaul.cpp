#include "precompiled.h"

enum
{
	NPC_RAZZA		= 11497,
	NPC_SKARR		= 11498,
	NPC_MUSHGOG		= 11447,

	SAY_SKARR	= -1540100,
	SAY_RAZZA	= -1540101,
	SAY_MUSHGOG	= -1540102,
};

struct MANGOS_DLL_DECL Grinbiblix : public ScriptedAI
{
	bool firstTick;

    Grinbiblix(Creature* pCreature) : ScriptedAI(pCreature), firstTick(true)
	{
	}

    void Reset()
    {
	}

	void UpdateAI(uint32 timeDelta)
	{
		if(firstTick)
		{
			firstTick = false;
			uint32 entry = 0;
			int32 text = 0;
			switch(urand(0, 2))
			{
			case 0:
				entry = NPC_RAZZA;
				text = SAY_RAZZA;
				break;
			case 1:
				entry = NPC_SKARR;
				text = SAY_SKARR;
				break;
			case 2:
				entry = NPC_MUSHGOG;
				text = SAY_MUSHGOG;
				break;
			}

			m_creature->SummonCreature(entry, -3759.07f, 1093.38f, 131.96f, 3.04f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5*HOUR*MINUTE*IN_MILLISECONDS);
			m_creature->MonsterYellToZone(text, 0, nullptr);
		}
	}
};


void AddSC_theMaul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_grinbiblix";
    newscript->GetAI = [](Creature* creature) -> CreatureAI*
	{
		return new Grinbiblix(creature);
	};
    newscript->RegisterSelf();
}