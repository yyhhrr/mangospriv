#include "precompiled.h"

struct MANGOS_DLL_DECL npc_battle_chickenAI : public ScriptedAI
{
	enum
	{
		SPELL_BATTLE_SQUAWK	= 23060,
		SPELL_CHICKEN_FURY	= 13168,
	};

	npc_battle_chickenAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	int32 mCastTimer;

	void Reset()
	{
		mCastTimer = static_cast<int32>(urand(1000, 60000));
	}

	void UpdateAI(uint32 uiDiff) 
	{
		if(mCastTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, urand(0, 1) ? SPELL_BATTLE_SQUAWK : SPELL_CHICKEN_FURY);
			mCastTimer = 12345678;
		}
		else
			mCastTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};


//Explode: 4050
struct MANGOS_DLL_DECL npc_explosive_sheepAI : public ScriptedAI
{
	enum
	{
		EXPLODE_SHEEP = 4050,
	};

	npc_explosive_sheepAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

	void Reset(){}

	void UpdateAI(uint32 uiDiff) 
	{
		if (!m_creature->GetOwner() || !m_creature->GetOwner()->getVictim())
			return;

		Unit* victim = m_creature->GetOwner()->getVictim();

		if(m_creature->CanReachWithMeleeAttack(victim))
			m_creature->CastSpell(m_creature, EXPLODE_SHEEP, true);
	}
};


void AddSC_npc_EngineeringPets()
{
	Script* pNewScript;

	pNewScript = new Script;
	pNewScript->Name = "npc_battle_chicken";
	pNewScript->GetAI = [](Creature* pCreature) -> CreatureAI*
	{
		return new npc_battle_chickenAI(pCreature);
	};
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_exploding_sheep";
	pNewScript->GetAI = [](Creature* pCreature) -> CreatureAI*
	{
		return new npc_explosive_sheepAI(pCreature);
	};
	pNewScript->RegisterSelf();
}