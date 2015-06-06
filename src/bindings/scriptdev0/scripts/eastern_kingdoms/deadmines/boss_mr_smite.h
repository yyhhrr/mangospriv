/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */


#ifndef DEF_BOSS_MR_SMITE_H
#define DEF_BOSS_MR_SMITE_H


//////////////////////////////////////////////////////////////////////////
// Mr. Smite's AI struct
struct MANGOS_DLL_DECL boss_mr_smiteAI
	: public ScriptedAI
{
	//////////////////////////////////////////////////////////////////////////
	// Variables
	uint32 m_uiPhase;
	uint32 m_uiEquipTimer;
	uint32 m_uiSlamTimer;


	//////////////////////////////////////////////////////////////////////////
	// Ctor & Dtor
	boss_mr_smiteAI(Creature* pCreature);
	virtual ~boss_mr_smiteAI();


	//////////////////////////////////////////////////////////////////////////
	// Overridden
	virtual void Reset() override;
	virtual void AttackedBy(Unit* pAttacker) override;
	virtual void AttackStart(Unit* pWho) override;
	virtual void MovementInform(uint32 uiMotionType, uint32 /*uiPointId*/) override;
	virtual void UpdateAI(const uint32 uiDiff) override;


	//////////////////////////////////////////////////////////////////////////
	// Methods
	void PhaseEquipStart();
	void PhaseEquipProcess();
	void PhaseEquipEnd();
};


#endif	//DEF_BOSS_MR_SMITE_H
