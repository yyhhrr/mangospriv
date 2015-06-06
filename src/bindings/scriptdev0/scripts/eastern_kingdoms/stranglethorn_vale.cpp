/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Stranglethorn_Vale
SD%Complete: 100
SDComment: Quest support: 592
SDCategory: Stranglethorn Vale
EndScriptData */

/* ContentData
mob_yenniku
EndContentData */

#include "precompiled.h"
#include "Timer.h"
/*

struct MANGOS_DLL_DECL mob_gurubashi_arenaAI : public ScriptedAI
{
	mob_gurubashi_arenaAI(Creature *c) : ScriptedAI(c)
	{
		bReset = false;
		Reset();
	}

	uint32 Reset_Timer;
	bool bReset;

	void Reset()
	{
		Reset_Timer = 0;
		m_creature->HandleEmote(EMOTE_STATE_NONE);
	}

	void UpdateAI(const uint32 diff)
	{

		if (bReset)
		{
			if (Reset_Timer < diff)
			{
				EnterEvadeMode();
				bReset = false;
				m_creature->setFaction(28);                     //troll, bloodscalp
			}
			else Reset_Timer -= diff;
		}

		//Return since we have no target
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_gurubashi_arena(Creature *_Creature)
{
	return new mob_gurubashi_arenaAI(_Creature);
}
*/
/*######
## mob_yenniku
######*/

struct MANGOS_DLL_DECL mob_yennikuAI : public ScriptedAI
{
    mob_yennikuAI(Creature *c) : ScriptedAI(c)
    {
        bReset = false;
        Reset();
    }

    uint32 Reset_Timer;
    bool bReset;

    void Reset()
    {
        Reset_Timer = 0;
        m_creature->HandleEmote(EMOTE_STATE_NONE);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
                                                            //Yenniku's Release
            if (!bReset && ((Player*)caster)->GetQuestStatus(592) == QUEST_STATUS_INCOMPLETE && spell->Id == 3607)
            {
                m_creature->HandleEmote(EMOTE_STATE_STUN);
                m_creature->CombatStop();                   //stop combat
                m_creature->DeleteThreatList();             //unsure of this
                m_creature->setFaction(83);                 //horde generic

                bReset = true;
                Reset_Timer = 60000;
            }
        }
        return;
    }

    void Aggro(Unit *) {}

    void UpdateAI(const uint32 diff)
    {
        if (bReset)
		{
            if (Reset_Timer < diff)
			{
				EnterEvadeMode();
				bReset = false;
				m_creature->setFaction(28);                     //troll, bloodscalp
			}
			else Reset_Timer -= diff;
		}

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_yenniku(Creature *_Creature)
{
    return new mob_yennikuAI (_Creature);
}

/*
	Yojamba Isle (ZG Stuff)
*/


enum
{
	SPELL_CREATE_HEART_OF_HAKKAR_RIFT = 24202,
	SPELL_HEART_OF_HAKKAR_RITUAL_CAST = 24217,
	SPELL_HEART_OF_HAKKAR_IMPLODE = 24204,
	SPELL_HELLFIRE_CAST_VISUAL = 24207,
	SPELL_HEART_OF_HAKKAR_SUMMON_CIRCLE = 24602,
	SPELL_HEART_OF_HAKKAR_BANNING = 24203,
	SPELL_HEART_OF_HAKKAR_MOLTHOR_CHUCKS_THE_HEART = 24214,
	SPELL_CREATE_HEART_OF_HAKKAR_EXPLOISON = 24215,
	SPELL_SPIRIT_OF_ZANDALAR = 24425,

	SPELL_TELEPORT_SPAWN_OUT = 24221,

	SPELL_SPAWN_RED_LIGHTNING = 24240,

	SPELL_QUEST_TROLL_HERO_SUMMON_VISUAL = 24258,

	NPC_TARGET_DUMMY = 90002,
	NPC_SERVANT = 15080,
};

const float servant_positions[4][4] =
{
	{ -11835.03f, 1349.51f, 2.01f, 5.90f },
	{ -11831.56f, 1331.27f, 1.84f, 1.14f },
	{ -11817.59f, 1324.89f, 1.46f, 1.14f },
	{ -11801.14f, 1335.25f, 1.26f, 2.87f },
};

struct MANGOS_DLL_DECL mob_molthor : public ScriptedAI
{
	mob_molthor(Creature *c) : ScriptedAI(c)
	{
		Reset();
	}
	
	uint32 mEventStep = 0;
	bool mJustStepped = true;
	uint32 mEventTimer = 0;

	Player* target;
	Unit* targetDummy;

	std::vector<Unit*> servants;

	void Reset() override
	{
		target = nullptr;
		targetDummy = nullptr;
		m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
		m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

		mEventStep = 0;
		mJustStepped = true;
		mEventTimer = 0;

		m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_WALK_MODE);
		servants.clear();
	}

	void MovementInform(uint32, uint32) override
	{
		++mEventStep;
		mJustStepped = false;
	}

	void StartQuestEvent(Player* plr)
	{
		target = plr;
		mJustStepped = false;
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		m_creature->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
	}

	void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
	{
	}

	void UpdateAI(uint32 timeDelta) override
	{
		if (mJustStepped)
			return;

		if (mEventTimer > timeDelta)
		{
			mEventTimer -= timeDelta;
			return;
		}

		if (mEventStep == 0)
		{
			m_creature->GetMotionMaster()->MovePoint(0, -11817.48f, 1250.02f, 2.64f);
			mJustStepped = true;
		}
		else if(mEventStep == 1)
		{
			m_creature->GetMotionMaster()->MovePoint(0, -11829.73f, 1258.05f, 1.88f);
			mJustStepped = true;
		}
		else if(mEventStep == 2)
		{
			m_creature->GetMotionMaster()->MovePoint(0, -11837.02f, 1293.10f, 0.69f);
			mJustStepped = true;
		}
		else if(mEventStep == 3)
		{
			targetDummy = GetClosestCreatureWithEntry(m_creature, NPC_TARGET_DUMMY, 100.0f);
			if(!targetDummy)
			{
				m_creature->MonsterSay("Etwas lief falsch, bitte beim Team melden!", 0);
				m_creature->Respawn();
				Reset();
				return;
			}
			
			m_creature->GetMotionMaster()->MovePoint(0, -11825.56f, 1322.88f, 0.29f);
			mJustStepped = true;
			mEventTimer = 1000;
		}
		else if(mEventStep == 4)
		{
			m_creature->CastSpell(m_creature, SPELL_QUEST_TROLL_HERO_SUMMON_VISUAL, false);
			++mEventStep;
			mEventTimer = 30000;
		}
		else if(mEventStep == 5)
		{
			for (int i = 0; i < 4; ++i)
			{
				auto cr = m_creature->SummonCreature(NPC_SERVANT, servant_positions[i][0], servant_positions[i][1], servant_positions[i][2], servant_positions[i][3],
													 TEMPSUMMON_TIMED_DESPAWN, 60000);
				if (!cr)
				{
					m_creature->MonsterSay("Etwas lief falsch, bitte beim Team melden!", 0);
					m_creature->Respawn();
					Reset();
					return;
				}

				cr->CastSpell(cr, 26638, true);
				servants.push_back(cr);
			}

			m_creature->CastSpell(targetDummy, SPELL_HEART_OF_HAKKAR_MOLTHOR_CHUCKS_THE_HEART, false);
			++mEventStep;
			mEventTimer = 2000;
		}
		else if(mEventStep == 6)
		{
			targetDummy->CastSpell(targetDummy, SPELL_CREATE_HEART_OF_HAKKAR_RIFT, true);
			++mEventStep;
			mEventTimer = 500;
		}
		else if(mEventStep == 7)
		{
			m_creature->MonsterYell(-1800000, 0, 0);
			for (int i = 0; i < 4; ++i)
				servants[i]->CastSpell(m_creature, SPELL_HEART_OF_HAKKAR_RITUAL_CAST, false);

			m_creature->CastSpell(m_creature, SPELL_HEART_OF_HAKKAR_BANNING, false);
			++mEventStep;
			mEventTimer = 30000;
		}
		else if(mEventStep == 8)
		{
			targetDummy->CastSpell(targetDummy, SPELL_CREATE_HEART_OF_HAKKAR_EXPLOISON, true);
			targetDummy->CastSpell(targetDummy, SPELL_HELLFIRE_CAST_VISUAL, true);
			++mEventStep;
			mEventTimer = 4000;
		}
		else if(mEventStep == 9)
		{
			m_creature->MonsterYell(-1800001, 0, target);
			++mEventStep;
			mEventTimer = 1000;

			for (int i = 0; i < 4; ++i)
				servants[i]->CastSpell(m_creature, SPELL_TELEPORT_SPAWN_OUT, false);
		}
		else if(mEventStep == 10)
		{
			m_creature->CastSpell(m_creature, SPELL_SPIRIT_OF_ZANDALAR, false);
			++mEventStep;
			mEventTimer = 4000;
		}
		else if(mEventStep == 11)
		{
			m_creature->GetMotionMaster()->MovePoint(0, -11837.02f, 1293.10f, 0.69f);
			mJustStepped = true;
		}
		else if(mEventStep == 12)
		{
			m_creature->GetMotionMaster()->MovePoint(0, -11829.73f, 1258.05f, 1.88f);
			mJustStepped = true;
		}
		else if(mEventStep == 13)
		{
			m_creature->GetMotionMaster()->MovePoint(0, -11817.48f, 1250.02f, 2.64f);
			mJustStepped = true;
		}
		else if(mEventStep == 14)
		{
			auto a = m_creature->GetMotionMaster()->top();

			float x, y, z;
			m_creature->GetRespawnCoord(x, y, z);
			m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
			mJustStepped = true;
		}
		else if(mEventStep == 15)
		{
			m_creature->SetFacingTo(4.24f);
			Reset();
		}
	}
};

bool onHakkarQuestComplete(Player* plr, Creature* cr, Quest const*)
{
	if(cr->AI() && plr && cr)
		static_cast<mob_molthor*>(cr->AI())->StartQuestEvent(plr);

	return true;
}

void AddSC_stranglethorn_vale()
{
    Script* pNewScript;
	/*
	pNewScript = new Script;
	pNewScript->Name = "mob_gurubashi_arena";
	pNewScript->GetAI = &GetAI_mob_gurubashi_arena;
	pNewScript->RegisterSelf();*/

    pNewScript = new Script;
    pNewScript->Name = "mob_yenniku";
    pNewScript->GetAI = &GetAI_mob_yenniku;
    pNewScript->RegisterSelf();


	pNewScript = new Script;
	pNewScript->Name = "mob_molthor";
	pNewScript->GetAI = [](Creature* c)->CreatureAI* { return new mob_molthor(c); };
	pNewScript->pQuestRewardedNPC = &onHakkarQuestComplete;
	pNewScript->RegisterSelf();
}
