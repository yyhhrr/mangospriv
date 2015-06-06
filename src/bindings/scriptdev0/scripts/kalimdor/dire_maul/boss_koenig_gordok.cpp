/*
#################################################################################
#  _____ _               _          __          ____          __				#
# / ____| |             (_)         \ \        / /\ \        / /				#
#| |    | | __ _ ___ ___ _  ___ _____\ \  /\  / /__\ \  /\  / / ___  _ __ __ _	#
#| |    | |/ _` / __/ __| |/ __|______\ \/  \/ / _ \\ \/  \/ / / _ \| '__/ _` |	#
#| |____| | (_| \__ \__ \ | (__        \  /\  / (_) |\  /\  / | (_) | | | (_| |	#
# \_____|_|\__,_|___/___/_|\___|        \/  \/ \___/  \/  \/ (_)___/|_|  \__, |	#
#                                                                         __/ |	#
#                                                                        |___/	#
#################################################################################
*/

#include "precompiled.h"
#include "dire_maul.h"
#include "boss_cho_rush.h"

#define Spell_Berserkerattacke		22886
#define Spell_Kriegsdonner			16727
#define Spell_Ruestung_Zerreissen	15572
#define Spell_Toedlicher_Stoss		15708

#define NPC_MIZZLE_THE_CRAFTY		14353

enum GordokEventFinish
{
    ChoRush_SAY_1       = -1901007,
    Mizzle_SAY_1        = -1901008,
    Mizzle_SAY_2		= -1901009,
};

class MANGOS_DLL_DECL mizzle_the_crafty : public ScriptedAI
{
private:
	uint32 mTimerKneel;
	uint32 mTimerSpell;
public:
	mizzle_the_crafty(Creature* pCreature) : ScriptedAI(pCreature) {
		m_creature->SetDefaultMovementType(MovementGeneratorType::IDLE_MOTION_TYPE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->setAttackEnabled(false);
		mTimerKneel = 0;
		mTimerSpell = 0;
	}

	void Reset() {
	}

	void JustRespawned() {
	}

	void checkTributeSpawn() {
		if(m_creature->isDead())
			return;

		auto pMap = m_creature->GetMap();
		auto pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		uint64 guidMoldar = pInstance->GetData64(NPC_GUARD_MOLDAR);
		uint64 guidFengus = pInstance->GetData64(NPC_GUARD_FENGUS);
		uint64 guidSlipki = pInstance->GetData64(NPC_GUARD_SLIPKIK);
		uint64 guidKromcr = pInstance->GetData64(NPC_CAPTAIN_KROMCRUSH);

		auto moldar = pMap->GetCreature(guidMoldar);
		auto fengus = pMap->GetCreature(guidFengus);
		auto slipki = pMap->GetCreature(guidSlipki);
		auto cromcr = pMap->GetCreature(guidKromcr);

		bool moldarOk = pInstance->GetData(TYPE_GUARD_MOLDAR) != DONE;
		bool fengusOk = pInstance->GetData(TYPE_GUARD_FENGUS) != DONE;
		bool slipkiOk = pInstance->GetData(TYPE_GUARD_SLIPKIK) != DONE;
		bool cromcrOk = pInstance->GetData(TYPE_CAPTAIN_KROMCRUSH) != DONE;

		if(moldar)
			moldar->setFaction(35);
		if(fengus)
			fengus->setFaction(35);
		if(slipki)
			slipki->setFaction(35);
		if(cromcr)
			cromcr->setFaction(35);

		if (moldarOk && fengusOk && slipkiOk && cromcrOk) {
			m_creature->SummonObject(pMap, GO_GORDOK_TRIBUTE, 809.899719f, 482.306366f, 37.318359f, 0.212846f);
		}
	}

	void UpdateAI(const uint32 diff) {
		if(mTimerKneel != 0) {
			if(mTimerKneel <= diff) {
				mTimerKneel = 0;
				m_creature->SetStandState(UNIT_STAND_STATE_STAND);
				DoScriptText(GordokEventFinish::Mizzle_SAY_2, m_creature);
				mTimerSpell = 5000;
			} else {
				mTimerKneel -= diff;
			}

			return;
		}

		if(mTimerSpell != 0) {
			if(mTimerSpell <= diff) {
				mTimerSpell = 0;
				m_creature->HandleEmoteCommand(EMOTE_ONESHOT_APPLAUD);
				auto map = m_creature->GetMap();
				const Map::PlayerList& playerMap = map->GetPlayers();
				for(auto& player : playerMap) {
					player.getSource()->CastSpell(player.getSource(), 22799, true);
				}

				checkTributeSpawn();
			} else {
				mTimerSpell -= diff;
			}
		}
	}

	void MovementInform(uint32 uiMotionType, uint32 uiPointId) {
        if (uiMotionType != POINT_MOTION_TYPE || !uiPointId)
            return;

		if(uiPointId == 1) {
			DoScriptText(GordokEventFinish::Mizzle_SAY_1, m_creature);
			m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
			mTimerKneel = 5000;
			return;
		}
	}
};

struct MANGOS_DLL_DECL boss_koenig_gordokAI : public ScriptedAI
{
    boss_koenig_gordokAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;
    Unit* target;

    uint32 Berserkerattacke_Timer;
    uint32 Kriegsdonner_Timer;
    uint32 Ruestung_Zerreissen_Timer;
    uint32 Toedlicher_Stoss_Timer;

    void Reset()
    {
        Berserkerattacke_Timer 		= 15000;
        Kriegsdonner_Timer 			= 5000;
        Ruestung_Zerreissen_Timer 	= 8000;
        Toedlicher_Stoss_Timer 		= 7500;
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
			pInstance->SetData(TYPE_KING_GORDOK, IN_PROGRESS);

		uint64 guidCho = pInstance->GetData64(NPC_CHORUSH);
		Creature* cho = m_creature->GetMap()->GetCreature(guidCho);

		if(cho != nullptr && cho->GetEntry() == NPC_CHORUSH) {
			cho->AddThreat(who);
			cho->SelectHostileTarget();
		}
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_KING_GORDOK, FAIL);
	}

    void JustDied(Unit*)
    {
		if (pInstance) {
			pInstance->SetData(TYPE_KING_GORDOK, DONE);
			std::list<Creature*> creatures;
			GetCreatureListWithEntryInGrid(creatures, m_creature, 14324, 500.0f);

			if(creatures.size() > 0) {
				auto cr = creatures.back();
				if(cr->isAlive()) {
					((boss_cho_rush_der_beobachterAI*)cr->AI())->handleGordokDied();

					Unit* victim = cr->getVictim();
					if(victim != nullptr) {
						victim->StopAttackObject(cr->GetObjectGuid());
					}

					Unit::AttackerSet attackers = cr->getAttackers();

					for(auto guid : attackers) {
						if(auto* attacker = m_creature->GetMap()->GetUnit(guid))
							attacker->StopAttackObject(cr->GetObjectGuid());
					}

					m_creature->AttackStop(true);
					cr->setFaction(35);

					DoScriptText(GordokEventFinish::ChoRush_SAY_1, cr);
				}

				auto crMizzle = cr->SummonCreature(NPC_MIZZLE_THE_CRAFTY, 831.925f, 446.586f, 37.318f, 2.46f, TEMPSUMMON_MANUAL_DESPAWN, 0);
				crMizzle->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
			}
			
		}
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Berserkerattacke_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), Spell_Berserkerattacke);
            Berserkerattacke_Timer = random(18000,23000);
        } else Berserkerattacke_Timer -= diff;

        if (Kriegsdonner_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Kriegsdonner);
            Kriegsdonner_Timer = random(8000,16000);
        } else Kriegsdonner_Timer -= diff;

        if (Ruestung_Zerreissen_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Ruestung_Zerreissen);
            Ruestung_Zerreissen_Timer = random(5000,9000);
        } else Ruestung_Zerreissen_Timer -= diff;

        if (Toedlicher_Stoss_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), Spell_Toedlicher_Stoss);
            Toedlicher_Stoss_Timer = random(16000,18000);
        } else Toedlicher_Stoss_Timer -= diff;
		
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_koenig_gordok(Creature *_Creature)
{
    return new boss_koenig_gordokAI (_Creature);
}

CreatureAI* GetAI_mizzle(Creature *_Creature)
{
    return new mizzle_the_crafty (_Creature);
}

template<uint32 size>
void fillDropVector(uint32 (&possibleDrop)[size], std::vector<uint32>& drop) {
	for(uint32 i = 0; i < size; ++i) {
		uint32 roll = urand(0, 100);
		if(roll < 25)
			drop.push_back(possibleDrop[i]);
	}

	if(drop.size() > 2) {
		std::map<uint32, uint32> rerollMap;
		for(uint32 i = 0; i < drop.size(); ++i) {
			uint32 roll = urand(0, 100);
			while(rerollMap.find(roll) != rerollMap.end()) {
				roll = urand(0, 100);
			}

			rerollMap[roll] = drop[i];
		}

		drop.clear();

		auto itr = std::max_element(rerollMap.begin(), rerollMap.end(), 
			[](std::map<uint32, uint32>::value_type& v1, std::map<uint32, uint32>::value_type& v2) {
				return v1.first < v2.first;
			}
		);

		drop.push_back(itr->second);
		rerollMap.erase(itr);
		itr = std::max_element(rerollMap.begin(), rerollMap.end(), 
			[](std::map<uint32, uint32>::value_type& v1, std::map<uint32, uint32>::value_type& v2) {
				return v1.first < v2.first;
			}
		);
			
		drop.push_back(itr->second);
	}

	if(drop.size() == 0 && size >= 1) {
		uint32 fillItem = possibleDrop[urand(0, size - 1)];
		drop.push_back(fillItem);
	}
}

bool appendLootMoldar(Loot& loot, Map* pMap) {
	auto pInstance = ((ScriptedInstance*)pMap->GetInstanceData());
	bool done = pInstance->GetData(TYPE_GUARD_MOLDAR) == DONE;

	if(!done) {
		static uint32 possibleDrop[] = { 18480, 18477, 18475, 18482, 18478 };
		std::vector<uint32> dropItems;
		fillDropVector(possibleDrop, dropItems);
		for(auto item : dropItems)
			loot.AddItem(LootStoreItem(item, 100.0f, 0, 0, 1, 1));

		return true;
	}

	return false;
}

bool appendLootFengus(Loot& loot, Map* pMap) {
	auto pInstance = ((ScriptedInstance*)pMap->GetInstanceData());
	bool done = pInstance->GetData(TYPE_GUARD_FENGUS) == DONE;

	if(!done) {
		static uint32 possibleDrop[] = { 18479, 18528, 18529, 18476, 18500 };
		std::vector<uint32> dropItems;
		fillDropVector(possibleDrop, dropItems);
		for(auto item : dropItems)
			loot.AddItem(LootStoreItem(item, 100.0f, 0, 0, 1, 1));

		return true;
	}

	return false;
}

bool appendLootSlipkik(Loot& loot, Map* pMap) {
	auto pInstance = ((ScriptedInstance*)pMap->GetInstanceData());
	bool done = pInstance->GetData(TYPE_GUARD_SLIPKIK) == DONE;

	if(!done) {
		static uint32 possibleDrop[] = { 18481, 18532, 18530, 18499, 18531 };
		std::vector<uint32> dropItems;
		fillDropVector(possibleDrop, dropItems);
		for(auto item : dropItems)
			loot.AddItem(LootStoreItem(item, 100.0f, 0, 0, 1, 1));

		return true;
	}

	return false;
}

bool appendLootKromcrush(Loot& loot, Map* pMap) {
	auto pInstance = ((ScriptedInstance*)pMap->GetInstanceData());
	bool done = pInstance->GetData(TYPE_CAPTAIN_KROMCRUSH) == DONE;

	if(!done) {
		static uint32 possibleDrop[] = { 18655, 18537, 18533, 18495, 18534 };
		std::vector<uint32> dropItems;
		fillDropVector(possibleDrop, dropItems);
		for(auto item : dropItems)
			loot.AddItem(LootStoreItem(item, 100.0f, 0, 0, 1, 1));

		return true;
	}

	return false;
}

void GordokTributeLootFill(Loot& loot, Player* owner) {
	bool moldar = appendLootMoldar(loot, owner->GetMap());
	bool fengus = appendLootFengus(loot, owner->GetMap());
	bool slipkik = appendLootSlipkik(loot, owner->GetMap());
	bool kromcrush = appendLootKromcrush(loot, owner->GetMap());

	if(moldar && fengus && slipkik && kromcrush) {
		LootStoreItem li(18538, 4, 1, 0, 1, 1);
		if(li.Roll(true))
			loot.AddItem(li);
	}

	loot.AddItem(LootStoreItem(8952, 100, false, 0, 15, 20));
	loot.AddItem(LootStoreItem(13444, 100, false, 0, 2, 6));
	loot.AddItem(LootStoreItem(13446, 100, false, 0, 2, 6));
}

void AddSC_boss_koenig_gordok()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_koenig_gordok";
    newscript->GetAI = &GetAI_boss_koenig_gordok;
    newscript->RegisterSelf();

	Script* mizzle = new Script();
	mizzle->Name = "mizzle_the_crafty";
	mizzle->GetAI = GetAI_mizzle;
	mizzle->RegisterSelf();

	Script* loot = new Script();
	loot->LootId = 150000;
	loot->pFillLoot = GordokTributeLootFill;
	loot->RegisterAsLootScript();
}