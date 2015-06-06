#include "precompiled.h"
#include "InstanceMaraudon.h"
#include "escort_ai.h"


struct MANGOS_DLL_DECL CelebrasTheRedeemed : public ScriptedAI//npc_escortAI
{
	uint32 mTimer;
	Player* mPlayer;
	bool mCreationStarted;

	bool mStep1, mStep2, mStep3;

    CelebrasTheRedeemed(Creature* pCreature) :
		ScriptedAI(pCreature),
		mTimer(0),
		mPlayer(nullptr),
		mCreationStarted(false)
	{
		mStep1 = mStep2 = mStep3 = false;
	}

	void Reset()
	{
		mTimer = 0;
		mStep1 = mStep2 = mStep3;
		mCreationStarted = false;
	}

	void MovementInform(uint32, uint32 pointId)
	//void WaypointReached(uint32 pointId)
	{
		if(pointId == 0)
		{
			m_creature->GetMotionMaster()->MovePoint(1, 657.7f, 75.09f, -86.82f);
			m_creature->MonsterSay("For so long I have drifted in my cursed form. You have freed me... Your hard work shall be repaid.", 0);
		}
		else if(pointId == 1)
		{
			std::stringstream ss;
			ss << "Please do as I instruct you, ";
			ss << mPlayer->GetName() << ".";
			m_creature->MonsterSay(ss.str().c_str(), 0);
			
			mCreationStarted = true;
			m_creature->SummonObject(m_creature->GetMap(), InstanceMaraudon::TOME, 656.27f, 73.28f, -86.83f, 0.0f);
		}
	}

	void Go(bool, Player* plr, const Quest*)
	{
		mPlayer = plr;
		m_creature->MonsterSay("You wish to learn of the stone? Follow me.", 0);
        m_creature->GetMotionMaster()->MovePoint(0, 726.106f, 77.976f, -86.592f);
	}

	void UpdateAI(const uint32 delta)
	{
		if(mCreationStarted)
		{
			mTimer += delta;

			if(mTimer > 9000)
			{
				m_creature->MonsterSay("My scepter will once again become whole!", 0);

				Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
				
				for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
				{
					Player* plr = itr->getSource();
					plr->CompleteQuest(InstanceMaraudon::THE_CEPTER_OF_CELEBRAS);
				}

				Reset();
			}
			else if(mTimer > 6000 && mStep3 == false)
			{
				m_creature->MonsterSay("Shal myrinan ishnu daldorah...", 0);
				DoCastSpellIfCan(m_creature, InstanceMaraudon::SPELL_CREATE_SCEPTER);
				mStep3 = true;
			}
			else if(mTimer > 2000 && mStep2 == false)
			{
				m_creature->MonsterSay("Together, the two parts shall become one, once again.", 0);
				InstanceMaraudon* instance = static_cast<InstanceMaraudon*>(m_creature->GetInstanceData());
				instance->ActivateCreator();
				mStep2 = true;
			}
			else if(mStep1 == false)
			{
				m_creature->MonsterSay("Read this tome I have placed before you, and speak the words aloud.", 0);
				m_creature->MonsterTextEmote("Celebras the Redeemed begins to channel his energy, focusing on the stone.", nullptr);
				mStep1 = true;
			}
		}
	}
};

bool QuestAccept_npc_CelebrasRedeemed(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == InstanceMaraudon::THE_CEPTER_OF_CELEBRAS)
    {
        if (InstanceData* pInstance = pCreature->GetInstanceData())
        {
            if (pInstance->GetData(InstanceMaraudon::QUEST_CEPTER) == NOT_STARTED || true)
            {
                pInstance->SetData(InstanceMaraudon::QUEST_CEPTER, IN_PROGRESS);

                if (CelebrasTheRedeemed* pEscortAI = static_cast<CelebrasTheRedeemed*>(pCreature->AI()))
				{
                    pEscortAI->Go(false, pPlayer, pQuest);
				}
            }
        }
    }
    return true;
}

void AddCelebrasTheRedeemed()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "npc_celebras_the_redeemed";
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_CelebrasRedeemed;
    pNewScript->GetAI = [](Creature* creature) -> CreatureAI*
	{
		return new CelebrasTheRedeemed(creature);
	};
    pNewScript->RegisterSelf();
	

	//TODO: move me into InstanceMaraudon.cpp
    pNewScript = new Script;
    pNewScript->Name = "instance_maraudon";
    pNewScript->GetInstanceData = [](Map* pMap) -> InstanceData*
	{

		return new InstanceMaraudon(pMap);
	};

    pNewScript->RegisterSelf();
}