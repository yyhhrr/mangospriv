#include "precompiled.h"
#include "Pat.h"

// UPDATE creature_template SET ScriptName = 'aq20_StingerPat' WHERE entry = 15327;

struct StingerPat : public Pat
{
	StingerPat(Creature* creature) : Pat(creature)
	{
		PatAdd add(15325);
		add.speed = 2000;
		add.abstand = 5;
		add.winkel = 90;
		NewAdd(add);

		add.winkel = 270;
		NewAdd(add);
	}

	void Update(const uint32 time){}

	void Reset(){}


	static CreatureAI* GetAI(Creature* creature)
	{
		return new StingerPat(creature);
	}
};

void AddSC_StingerPat()
{
	Script* pScript = new Script;
	pScript->GetAI = &StingerPat::GetAI;
	pScript->Name = "aq20_StingerPat";
	pScript->RegisterSelf(false);
}