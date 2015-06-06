#pragma once



struct MANGOS_DLL_DECL boss_ragnarosAI : public ScriptedAI
{
private:
    ScriptedInstance* pInstance;

	bool m_eventPhase;
	bool m_eventDone;

	uint32 m_eventStage;
	uint32 m_eventTimer;

    bool m_bSubmerged;
    bool m_bSubmergedOnce;
    bool m_bSonsDead;

    uint32 m_uiSummonCount;
    uint32 m_uiElementalFireTimer;
    uint32 m_uiEmergeTimer;
    uint32 m_uiMightOfRagnarosTimer;
    uint32 m_uiMightOfRagnarosTargetGuid;
    uint32 m_uiLavaBurstTimer;
    uint32 m_uiMagmaBlastTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiWrathOfRagnarosTimer;
    uint32 m_uiMeltWeaponTimer;
    uint8  m_uiPhase;
    Creature* Trigger;
    std::list<Creature*> pSons;

	Player* DoSelectRandomNonMeleePlayer();

public:
	boss_ragnarosAI(Creature* pCreature);

	void Reset();
	void JustReachedHome();
	void StartEvent(bool instant);
    void JustDied(Unit* pKiller);
    void JustSummoned(Creature* pSummoned);
	void KilledUnit(Unit* pVictim);
    void Aggro(Unit* pAttacker);
    void SummonedCreatureJustDied(Creature* pSummoned);

	void UpdateAI(const uint32 uiDiff);
};