#include "precompiled.h"
#include "blackrock_depths.h"
#include "the_grim_guzzler.h"


enum
{
	BLACKBREATH_YELL = -1540020,
	BLACKBREATH_ON_COMBAT = -1540021,

	SPELL_FLAME_BREATH = 9573,
	SPELL_DRUNKEN_RAGE = 14872,
};

bool GO_lager_keg_klicked(Player* plr, GameObject* go)
{
	instance_blackrock_depths* instance = static_cast<instance_blackrock_depths*>(go->GetInstanceData());
	if(instance)
		instance->OnKegKlicked(plr);

	return false;
}

struct MANGOS_DLL_DECL npc_hurly_blackbreathAI : public ScriptedAI
{
    ScriptedInstance* mInstance;
	bool mSay;

	uint32 mDrunkenRageTimer, mFlameBreathTimer;

    npc_hurly_blackbreathAI(Creature* m_creature) : 
		ScriptedAI(m_creature), 
		mSay(false),
		mDrunkenRageTimer(3000),
		mFlameBreathTimer(3500)
    {
        mInstance = static_cast<ScriptedInstance*>(m_creature->GetInstanceData());
    }

    void Reset()
	{
		mSay = false;
	}

	void EnterCombat(Unit*)
	{
		mInstance->SetData(TYPE_BLACKBREATH, IN_PROGRESS);
	}

	void JustDied(Unit*)
	{
		mInstance->SetData(TYPE_BLACKBREATH, DONE);
	}

	void UpdateAI(uint32 timeDelta)
	{
		if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		auto victim = m_creature->getVictim();

		if(!mSay)
		{
			if(!m_creature->CanReachWithMeleeAttack(victim))
			{
				mSay = true;
				m_creature->MonsterSay(BLACKBREATH_ON_COMBAT, 0);
			}
		}

		if(mDrunkenRageTimer < timeDelta)
		{
			DoCastSpellIfCan(m_creature, SPELL_DRUNKEN_RAGE);
			mDrunkenRageTimer = urand(20, 25) * 1000;
		}
		else
			mDrunkenRageTimer -= timeDelta;

		if(mFlameBreathTimer < timeDelta)
		{
			DoCastSpellIfCan(m_creature, SPELL_FLAME_BREATH);
			mFlameBreathTimer = urand(10, 17) * 1000;
		}
		else
			mFlameBreathTimer -= timeDelta;

		DoMeleeAttackIfReady();
	}
};

enum
{
	SPELL_CURSE_OF_AGONY = 14868,
	SPELL_SHADOW_BOLT = 20825,
	
	SPELL_KICK = 15614,

	SPELL_BACKHAND = 6253,
};

struct MANGOS_DLL_DECL npc_patronAI : public ScriptedAI
{
	uint32 mCastTimer;

    npc_patronAI(Creature* m_creature) : 
		ScriptedAI(m_creature),
		mCastTimer(3000)
    {
    }

    void Reset()
	{
		mCastTimer = 3000;
	}

	void EnterCombat(Unit*)
	{
		auto instance = static_cast<instance_blackrock_depths*>(m_creature->GetInstanceData());
		if(instance)
			instance->OnPatronAttacked();
	}

	void JustDied(Unit* killer)
	{
		auto instance = static_cast<instance_blackrock_depths*>(m_creature->GetInstanceData());
		if(instance)
			instance->OnPatronDied(killer);
	}

	void UpdateAI(uint32 timeDelta)
	{
		if(m_creature->isInCombat())
		{
			if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

			auto victim = m_creature->getVictim();
			if(mCastTimer < timeDelta)
			{
				switch(m_creature->GetEntry())
				{
				case NPC_GUZZLING_PATRON:
					if(urand(0, 1) == 0)
						DoCastSpellIfCan(victim, SPELL_CURSE_OF_AGONY);
					else
						DoCastSpellIfCan(victim, SPELL_SHADOW_BOLT);
					break;
				case NPC_GRIM_PATRON:
					DoCastSpellIfCan(victim, SPELL_KICK);
					break;
				case NPC_HAMMERED_PATRON:
					DoCastSpellIfCan(victim, SPELL_BACKHAND);
					break;
				}

				mCastTimer = urand(7, 14) * 1000;
			}
			else
				mCastTimer -= timeDelta;
		}
		else
		{
			float z = m_creature->GetPositionZ();
			if(z - -42.62f > -1.f)
			{
				m_creature->HandleEmote(EMOTE_STATE_DANCE);
				return;
			}

			switch(urand(0, 1000))
			{
			case 0:
				m_creature->HandleEmote(EMOTE_ONESHOT_CHEER);
				break;
			case 1:
				m_creature->HandleEmote(EMOTE_ONESHOT_LAUGH);
				break;
			case 2:
				m_creature->HandleEmote(EMOTE_ONESHOT_EAT);
				break;
			case 3:
			case 4:
				m_creature->HandleEmote(EMOTE_ONESHOT_TALK);
				break;
			default:
				break;
			}
		}

		DoMeleeAttackIfReady();
	}
};

enum
{
	SPELL_IMMOLATE = 12742,
	SPELL_SHADOWBOLT = 12739,
	SPELL_CURSE_OF_TONGUE = 13338,
};

struct MANGOS_DLL_DECL npc_spazzringAI : public ScriptedAI
{
	uint32 mImmolateTimer;
	uint32 mShadowBoltTimer;
	uint32 mCurseTimer;

    npc_spazzringAI(Creature* m_creature) : 
		ScriptedAI(m_creature),
		mImmolateTimer(2500),
		mShadowBoltTimer(5000),
		mCurseTimer(1000)
    {
    }

    void Reset() {}

	void UpdateAI(uint32 timeDelta)
	{
		if(m_creature->isInCombat())
		{
			if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

			if(mImmolateTimer < timeDelta)
			{
				mImmolateTimer = 2500;
				auto target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
				if(target)
					DoCastSpellIfCan(target, SPELL_IMMOLATE);
			}
			else
				mImmolateTimer -= timeDelta;

			if(mShadowBoltTimer < timeDelta)
			{
				mShadowBoltTimer = 5000;
				auto target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
				if(target)
					DoCastSpellIfCan(target, SPELL_SHADOWBOLT);
			}
			else
				mShadowBoltTimer -= timeDelta;

			if(mCurseTimer < timeDelta)
			{
				mCurseTimer = 1000;
				auto target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
				if(target)
					DoCastSpellIfCan(target, SPELL_CURSE_OF_TONGUE);
			}
			else
				mCurseTimer -= timeDelta;
		}
		else
		{
			if(urand(0, 1000) == 10)
			{
				int32 textId = urand(1540025, 1540028) * -1;
				m_creature->MonsterYell(textId, 0);
			}
		}

		DoMeleeAttackIfReady();
	}
};


bool GO_food_klicked(Player* plr, GameObject* go)
{
	instance_blackrock_depths* instance = static_cast<instance_blackrock_depths*>(go->GetInstanceData());
	if(instance)
		instance->OnFoodLooted(plr);

	return false;
}

bool GossipHello_npc_ribbly(Player* pPlayer, Creature* pCreature)
{
	pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
	pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -3469200, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
	pPlayer->SEND_GOSSIP_MENU(2643, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_ribbly(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
	if(uiAction == GOSSIP_ACTION_INFO_DEF)
	{
		auto instance = static_cast<instance_blackrock_depths*>(pCreature->GetInstanceData());
		if(instance)
			instance->OnRibblyGossip(pPlayer);

		pCreature->setFaction(54);
		pCreature->MonsterSay(-1540033, 0);
		pCreature->AI()->AttackStart(pPlayer);
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}

void AddSC_blackrock_grim_guzzler()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "go_lager_keg";
    pNewScript->pGOUse = &GO_lager_keg_klicked;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_blackbreath";
    pNewScript->GetAI = [](Creature* c) -> CreatureAI*
	{
		return new npc_hurly_blackbreathAI(c);
	};
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_patron";
    pNewScript->GetAI = [](Creature* c) -> CreatureAI*
	{
		return new npc_patronAI(c);
	};
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_phalanx";
    pNewScript->GetAI = [](Creature* c) -> CreatureAI*
	{
		return new mob_phalanxAI(c);
	};
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_spazzring";
    pNewScript->GetAI = [](Creature* c) -> CreatureAI*
	{
		return new npc_spazzringAI(c);
	};
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_guzzler_food";
    pNewScript->pGOUse = &GO_food_klicked;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_ribbly";
    pNewScript->pGossipHello =  &GossipHello_npc_ribbly;
    pNewScript->pGossipSelect = &GossipSelect_npc_ribbly;
    pNewScript->RegisterSelf();
}