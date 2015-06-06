#include "precompiled.h"
#include "scholomance.h"



struct MANGOS_DLL_DECL SpectralTutor : public ScriptedAI
{
    SpectralTutor(Creature *c) : ScriptedAI(c)
	{
		Reset();
	}

	uint32 mImageProjectionTimer;
	uint32 mSilenceTimer;
	uint32 mManaburnTimer;

	bool mAddsSpawned;

	enum Spells : uint32
	{
		SILENCE		= 12528,
		MANABURN	= 17630,
	};

    void Reset()
    {
		mAddsSpawned			= false;
		mSilenceTimer			= 2000;
		mManaburnTimer			= 5000;
        mImageProjectionTimer	= 12000;
		m_creature->SetVisibility(UnitVisibility::VISIBILITY_ON);
    }

	void ToggleVisibility()
	{
		mAddsSpawned = !mAddsSpawned;

		if(mAddsSpawned)
		{
			if(CanCast(m_creature, GetSpellStore()->LookupEntry(17651)) == false) //CC check
			{
				mAddsSpawned = !mAddsSpawned;
				return;
			}

			float x, y, z;
			m_creature->GetPosition(x, y, z);

			m_creature->SummonCreature(11263, x + 0, y + 0, z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN_FORCED, 10000);
			m_creature->SummonCreature(11263, x + 5, y + 0, z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN_FORCED, 10000);
			m_creature->SummonCreature(11263, x + 0, y + 5, z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN_FORCED, 10000);
			m_creature->SummonCreature(11263, x - 5, y + 0, z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN_FORCED, 10000);
			m_creature->SummonCreature(11263, x + 0, y - 5, z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN_FORCED, 10000);
			m_creature->SetVisibility(UnitVisibility::VISIBILITY_OFF);
			m_creature->RemoveAllAuras();
			mImageProjectionTimer = 10000;
		}
		else
		{
			mImageProjectionTimer = 12000;
			m_creature->SetVisibility(UnitVisibility::VISIBILITY_ON);
		}
	}

	void CastTimed(const uint32 diff, uint32& timer, uint32 spell, uint32 reset)
	{
		if(timer < diff && mAddsSpawned == false)
		{
			DoCastSpellIfCan(m_creature->getVictim(), spell);
			timer = reset;
		}
		else
			timer -= diff;
	}

	void DoTimed(const uint32 diff, uint32& timer, std::function<void()> func)
	{
		if(timer < diff)
			func();
		else
			timer -= diff;
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		//Cuz casting 17651 will reset the npc, this is a pretty good workaround though
		DoTimed(diff, mImageProjectionTimer, std::bind(&SpectralTutor::ToggleVisibility, this));

		CastTimed(diff, mSilenceTimer, Spells::SILENCE, 7000);
		CastTimed(diff, mManaburnTimer, Spells::MANABURN, 9000);

		if(mAddsSpawned == false)
			DoMeleeAttackIfReady();
    }

	void SummonedCreatureDespawn(Creature* creature)
	{
		if(creature->isAlive() && m_creature->isAlive())
		{
			auto currentHealth = m_creature->GetHealthPercent();
			m_creature->SetHealthPercent(std::max(100.0f, currentHealth + 20.0f));
		}
	}

	void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
	{
		if(mAddsSpawned)
			uiDamage = 0;
	}
};

void AddSC_SpectralTutor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="spectralTutor";

	newscript->GetAI = [](Creature* creature) -> CreatureAI*
	{
		return new SpectralTutor(creature);
	};

    newscript->RegisterSelf();
}