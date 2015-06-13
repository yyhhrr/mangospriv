#include "precompiled.h"

struct Hunterquest_BossTemplate : public ScriptedAI
{
private:
	bool justSpawned;
	uint32 setupTime;

	uint64 ownerGuid;

public:
    Hunterquest_BossTemplate(Creature* creature) : 
		ScriptedAI(creature), 
		justSpawned(true), 
		setupTime(5000),
		ownerGuid(0)
	{
		m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
	}

	void SetOwner(uint64 guid)
	{
		ownerGuid = guid;
	}

    virtual void Update(const uint32 time) = 0;

    void Panik()
    {
        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
        {
            Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
            WorldLocation loc;
			if (unit)
			{
				unit->GetPosition(loc);
			}

			if(m_creature->GetDistance2d(unit) > 100)
				continue;

			if(loc.mapid != m_creature->GetMapId())
				continue;

            Creature* cleaner = m_creature->SummonCreature(14503, loc.coord_x - 5, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            if(cleaner)
                cleaner->AddThreat(unit, 100000);
        }
        std::string msg = "Only a fool would now remain in this battle. So long, coward";
        m_creature->MonsterSay(msg.c_str(), 0);
        m_creature->ForcedDespawn();

        Creature* cr = GetClosestCreatureWithEntry(m_creature, 14528, 1500);
        if(cr)
            cr->ForcedDespawn();
        else
        {
            cr = GetClosestCreatureWithEntry(m_creature, 14538, 1500);
            if(cr)
                cr->ForcedDespawn();
        }
    }

    static void DespawnDaemon(Creature* c, uint32 addTime)
    {
        c->ForcedDespawn(0);
        //minimal 30 minutes, maximal respawn after 3 hours
        c->SetRespawnTime(urand(1800 + addTime, 10800 + addTime));
    }

    void UpdateAI(const uint32 time)
    {
		if(justSpawned)
		{
			if(setupTime < time)
			{
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
				justSpawned = false;
			}
			else
				setupTime -= time;
			return;
		}

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		auto* victim = m_creature->getVictim();

        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        if(tl.size() > 1 || victim->GetGUID() != ownerGuid)
        {
            Panik();
        }

        Update(time);
        DoMeleeAttackIfReady();
    }
};

enum
{
    /* Spells */
    STINGING_TRAUMA = 23299,
    FOOLS_PLIGHT = 23504,
    DEMONIC_FRENZY = 23257,
    SERPENT_STING_9 = 25295,
    SERPENT_STING_8 = 13555,
    DEMONIC_DOOM = 23298,

    /* Entries */
    DOOMBRINGER = 14535,
    AMIABLE = 14531,
};

struct Hunterquest_Doombringer : public Hunterquest_BossTemplate
{
    uint32 stingHit;
    uint32 frenzy;
    uint32 doom;

    ObjectGuid hunter;

    Hunterquest_Doombringer(Creature* creature) : Hunterquest_BossTemplate(creature), stingHit(0), frenzy(0), doom(60000), hunter(ObjectGuid()) {}

    void Reset() { stingHit = frenzy = 0; doom = 0; hunter = ObjectGuid(); }

    void Aggro(Unit* target) 
    {
        frenzy = urand(0, 20000);
        doom = urand(0, 20000);
        hunter = target->GetObjectGuid(); 
    }

    void Update(const uint32 time) 
    {
        frenzy += time;
        doom += time;

        if(!m_creature->HasAura(SERPENT_STING_8))
            stingHit = 0;

        if(frenzy >= 25000)
        {
            DoCastSpellIfCan(m_creature, DEMONIC_FRENZY);
            frenzy = 0;
        }

        if(doom >= 30000)
        {
            if (Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
            {
                float distance = m_creature->GetDistance2d(pHunter->GetPositionX(), pHunter->GetPositionY());
                if(distance < 31)
                    DoCastSpellIfCan(pHunter, DEMONIC_DOOM);
            }
            doom = 0;
        }
    }

    void SpellHit(Unit*, const SpellEntry* entry) 
    {
        if(entry->Id == SERPENT_STING_8)
        {
            if(++stingHit == 2)
            {
                DoCastSpellIfCan(m_creature, STINGING_TRAUMA);

                m_creature->RemoveAura(SERPENT_STING_8, EFFECT_INDEX_0);
                m_creature->RemoveAura(SERPENT_STING_8, EFFECT_INDEX_1);
                m_creature->RemoveAura(SERPENT_STING_8, EFFECT_INDEX_2);

                stingHit = 0;
            }
        }
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_Doombringer(creature);
    }
};


struct Hunterquest_Amiable : public Hunterquest_BossTemplate
{
    Hunterquest_Amiable(Creature* creature) : Hunterquest_BossTemplate(creature)
    { 
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }
    void Reset() {}
    void Update(const uint32)
    {
        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
        {
            Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
            if(unit)
                DoCastSpellIfCan(unit, FOOLS_PLIGHT);
        }
    }

    static bool GossipHello(Player* pPlayer, Creature* pCreature)
    {
        if(pPlayer->IsActiveQuest(7636))
        {
            std::string msg = "I know who you are, reveal your true face, demon!";
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }

        pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());

        return true;
    }

    static bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
    {
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            WorldLocation loc;
            pCreature->GetPosition(loc);
            //despawn after 20 minutes the summoned mob
            auto* creature = pCreature->SummonCreature(DOOMBRINGER, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
			if(creature)
			{
				DespawnDaemon(pCreature, 1200);
				auto* ai = static_cast<Hunterquest_BossTemplate*>(creature->AI());
				if(ai)
					ai->SetOwner(pPlayer->GetGUID());
			}
        }

        return true;
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_Amiable(creature);
    }
};

enum
{
    /* Spells */
    SCORPID_STING_RANK4 = 14277,

    /* Entries */
    CRAZED_ENTRY = 14534,
    FRIENDLY_ENTRY = 14529,
};

struct Hunterquest_Klinfran : public Hunterquest_BossTemplate
{
    uint32 enrage;
    float baseMinDmg;
    float baseMaxDmg;

    Hunterquest_Klinfran(Creature *creature) : Hunterquest_BossTemplate(creature), enrage(0)
    {
        baseMinDmg = m_creature->GetFloatValue(UNIT_FIELD_MINDAMAGE);
        baseMaxDmg = m_creature->GetFloatValue(UNIT_FIELD_MAXDAMAGE);
    }

    void Reset() { enrage = 0; MaxDmg(); }

    void MinDmg()
    {
        m_creature->SetFloatValue(UNIT_FIELD_MINDAMAGE, 1.0f);
        m_creature->SetFloatValue(UNIT_FIELD_MAXDAMAGE, 1.0f);
    }

    void MaxDmg()
    {
        m_creature->SetFloatValue(UNIT_FIELD_MINDAMAGE, baseMinDmg);
        m_creature->SetFloatValue(UNIT_FIELD_MAXDAMAGE, baseMaxDmg);
    }
    
    void Update(const uint32 time)
    {
        enrage += time;

        if(m_creature->HasAura(SCORPID_STING_RANK4) && !m_creature->HasAura(DEMONIC_FRENZY))
            MinDmg();
        else
            MaxDmg();
        
        if(enrage >= 15000 && !m_creature->HasAura(SCORPID_STING_RANK4))
        {
            MaxDmg();

            if(DoCastSpellIfCan(m_creature, DEMONIC_FRENZY) != CAST_OK)
                MinDmg();

            enrage = 0;
        }
    }

    void SpellHit(Unit*, const SpellEntry* entry) 
    {
        if(entry->Id == SCORPID_STING_RANK4)
        {
			m_creature->RemoveAura(DEMONIC_FRENZY, EFFECT_INDEX_0);
			m_creature->RemoveAura(DEMONIC_FRENZY, EFFECT_INDEX_1);
			m_creature->RemoveAura(DEMONIC_FRENZY, EFFECT_INDEX_2);
        }
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_Klinfran(creature);
    }
};

struct Hunterquest_Franklin : public Hunterquest_BossTemplate
{
    Hunterquest_Franklin(Creature* creature) : Hunterquest_BossTemplate(creature) 
    { 
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }
    void Reset() {}
    void Update(const uint32)
    {
        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
        {
            Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
            if(unit)
                DoCastSpellIfCan(unit, FOOLS_PLIGHT);
        }
    }

    static bool GossipHello(Player* pPlayer, Creature* pCreature)
    {
        if(pPlayer->IsActiveQuest(7636))
        {
            std::string msg = "I know who you are, reveal your true face, demon!";
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }

        pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());

        return true;
    }

    static bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
    {
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            WorldLocation loc;
            pCreature->GetPosition(loc);
            auto* creature = pCreature->SummonCreature(CRAZED_ENTRY, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
			if(creature)
			{
				DespawnDaemon(pCreature, 1200);
				auto* ai = static_cast<Hunterquest_BossTemplate*>(creature->AI());
				if(ai)
					ai->SetOwner(pPlayer->GetGUID());
			}
        }

        return true;
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_Franklin(creature);
    }
};

enum
{
    /* Spells */
    TEMPTRESS_KISS = 23205,
    CHAIN_LIGHTNING = 23206,
    VIPER_STING = 14280,

    /* Spawns */
    PET_ID = 14538,
    SIMONE_SUCCUBUS_ID = 14533,
	SIMONE_INCONSPICUOIS = 14527,
};

struct Hunterquest_PetTemplate : public ScriptedAI
{
private:
	uint64 ownerGuid;

public:
    Hunterquest_PetTemplate(Creature* creature) : ScriptedAI(creature) { }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_PetTemplate(creature);
    }

	void SetOwner(uint64 guid)
	{
		ownerGuid = guid;
	}

    void Reset()
    {
    }

    void Aggro(Unit* target)
    {
        Creature* cr = GetClosestCreatureWithEntry(m_creature, SIMONE_SUCCUBUS_ID, 100);
        if(cr)
            cr->AddThreat(target, 100000);
        else
        {
            cr = GetClosestCreatureWithEntry(m_creature, SIMONE_INCONSPICUOIS, 500);
            if(cr)
            {
                cr->AddThreat(target, 100000);
            }
        }
    }

    void UpdateAI(const uint32)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		auto* victim = m_creature->getVictim();

        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        if(tl.size() > 1 || victim->GetGUID() != ownerGuid)
        {
            for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
            {
                Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                WorldLocation loc;
                unit->GetPosition(loc);
                m_creature->SummonCreature(14503, loc.coord_x - 5, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_MANUAL_DESPAWN, 1800000)->AddThreat(unit, 100000);
            };

            Creature* cr = GetClosestCreatureWithEntry(m_creature, SIMONE_SUCCUBUS_ID, 100);
            if(cr)
            {
                std::string msg = "Only a fool would now remain in this battle. So long, coward";
                cr->MonsterSay(msg.c_str(), 0);
                cr->ForcedDespawn();
            }
            else
            {
                cr = GetClosestCreatureWithEntry(m_creature, SIMONE_INCONSPICUOIS, 500);
                if(cr)
                {
                    std::string msg = "Only a fool would now remain in this battle. So long, coward";
                    cr->MonsterSay(msg.c_str(), 0);
                    cr->ForcedDespawn();
                }
            }
            m_creature->ForcedDespawn();
        }
        DoMeleeAttackIfReady();
    }
};

struct Hunterquest_SimoneSeductress : public Hunterquest_BossTemplate
{
    uint32 tk_time;
    uint32 cl_time;

    Hunterquest_SimoneSeductress(Creature* creature) : Hunterquest_BossTemplate(creature), tk_time(0), cl_time(0) 
    { 
        m_creature->SetMaxPower(POWER_MANA, 12170);
    }
    void Reset() { tk_time = cl_time = 0; }

    void Aggro(Unit* target)
    {
        DoCastSpellIfCan(target, TEMPTRESS_KISS);
		
        Creature* cr = GetClosestCreatureWithEntry(m_creature, PET_ID, 100);
        if(cr)
		{
			auto* ai = static_cast<Hunterquest_PetTemplate*>(cr->AI());
			if(ai)
				ai->SetOwner(target->GetGUID());
            cr->AddThreat(target, 100000);
		}
    }

    void Update(const uint32 time)
    {
        tk_time += time;
        cl_time += time;

        if(tk_time >= 45000)
        {
            Unit* unit = m_creature->GetMap()->GetUnit(m_creature->GetTargetGuid());
            if(unit)
                DoCastSpellIfCan(unit, TEMPTRESS_KISS);
            tk_time = 0;
        }
        if(cl_time >= 12000)
        {
            Unit* unit = m_creature->GetMap()->GetUnit(m_creature->GetTargetGuid());
            if(unit)
                DoCastSpellIfCan(unit, CHAIN_LIGHTNING);
            cl_time = 0;
        }
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_SimoneSeductress(creature);
    }
};

struct Hunterquest_SimoneInconspicuois : public Hunterquest_BossTemplate
{
    Hunterquest_SimoneInconspicuois(Creature* creature) : Hunterquest_BossTemplate(creature) 
    { 
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }
    void Reset() {}
    void Update(const uint32)
    {
        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
        {
            Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
            if(unit)
                DoCastSpellIfCan(unit, FOOLS_PLIGHT);
        }
    }

    static bool GossipHello(Player* pPlayer, Creature* pCreature)
    {
        if(pPlayer->IsActiveQuest(7636))
        {
            std::string msg = "I know who you are, reveal your true face, demon!";
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }

        pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());

        return true;
    }

    static bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
    {
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            Creature* cr = GetClosestCreatureWithEntry(pCreature, 14528, 100);
            if(cr)
            {
                WorldLocation loc;
                cr->GetPosition(loc);
                cr->SummonCreature(PET_ID, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
                cr->ForcedDespawn();

                pCreature->GetPosition(loc);
				auto* creature = pCreature->SummonCreature(SIMONE_SUCCUBUS_ID, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
				if(creature)
				{
					DespawnDaemon(pCreature, 1200);
					auto* ai = static_cast<Hunterquest_BossTemplate*>(creature->AI());
					if(ai)
						ai->SetOwner(pPlayer->GetGUID());
				}
            }
        }

        return true;
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_SimoneInconspicuois(creature);
    }
};

enum
{
    /* Spells */
    DREAFUL_FRIGHT = 23275,
    SOUL_FLAME = 23272,
    WING_CLIP = 14268,
    CRIPPLING_CLIP = 23279,
    ICE_TRAP = 13810,

    /* Entries */
    SLAYER = 14530,
    NICE = 14536,
    CREEPING_DOOM = 14761,
};


struct Hunterquest_Solenor : public Hunterquest_BossTemplate
{
    uint32 fright;
    uint32 bug;
    ObjectGuid hunter;

    bool applyAura;
    uint32 auraTimer;

    Hunterquest_Solenor(Creature* creature) : Hunterquest_BossTemplate(creature), fright(0), bug(0), hunter(ObjectGuid()),
        applyAura(false), auraTimer(0) {DoCastSpellIfCan(m_creature, SOUL_FLAME);}

    void Reset() { hunter = ObjectGuid(); fright = bug = 0; }

    void Aggro(Unit* target) { hunter = target->GetObjectGuid(); }

    void Update(const uint32 time)
    {
        fright += time;
        bug += time;

        if(m_creature->HasAura(ICE_TRAP))
        {
            m_creature->RemoveAura(SOUL_FLAME, EFFECT_INDEX_0);
            m_creature->RemoveAura(SOUL_FLAME, EFFECT_INDEX_1);
            m_creature->RemoveAura(SOUL_FLAME, EFFECT_INDEX_2);
        }

        if(fright >= 12000)
        {
            if(Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
            {					
                float distance1 = m_creature->GetDistance2d(pHunter->GetPositionX(), pHunter->GetPositionY());
                if(distance1 > 5.0f)
                    DoCastSpellIfCan(pHunter, DREAFUL_FRIGHT);
            }
            fright = 0;
        }

        //Hackfix - the aura sometimes disappear because of dmg done by the hunter
        if(applyAura)
        {
            auraTimer += time;

            if(auraTimer < 30000)
            {
                if(!m_creature->HasAura(CRIPPLING_CLIP))
                    DoCastSpellIfCan(m_creature, CRIPPLING_CLIP);
            }
            else
            {
                if(m_creature->HasAura(CRIPPLING_CLIP))
                {
                    m_creature->RemoveAura(CRIPPLING_CLIP, EFFECT_INDEX_0);
                    m_creature->RemoveAura(CRIPPLING_CLIP, EFFECT_INDEX_1);
                    m_creature->RemoveAura(CRIPPLING_CLIP, EFFECT_INDEX_2);
                }
                auraTimer = 0;
                applyAura = false;
            }
        }

        if(bug >= 10000)
        {
            if (Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
            {			
                float distance1 = m_creature->GetDistance2d(pHunter->GetPositionX(), pHunter->GetPositionY());
                if(distance1 > 5.0f)
                {
                    for (uint8 i = 0; i < 3; i++)
                    {
                        Creature* DoomBug = DoSpawnCreature(CREEPING_DOOM, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 1800000);
                        if(DoomBug)
                        {
                            DoomBug->SetSpeedRate(MOVE_WALK, 0.3f);
                            DoomBug->SetSpeedRate(MOVE_RUN, 0.3f);
                            DoomBug->AI()->AttackStart(pHunter);
							DoomBug->AI()->JustSummoned(m_creature); //dosn't get called otherwise, weird
                        }
                    }
                }
            }

            bug = 0;
        }
    }

    void SpellHit(Unit*, const SpellEntry* entry) 
    {
        if(entry->Id == WING_CLIP)
        {
            auraTimer = 0;
            applyAura = true;
        }
    }

	static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_Solenor(creature);
    }
};

struct DoomBug : public ScriptedAI
{
    ObjectGuid hunter;
    ObjectGuid summoner;

    DoomBug(Creature* creature) : ScriptedAI(creature), hunter(ObjectGuid()), summoner(ObjectGuid()) { }
    void Reset()
    {
        m_creature->ForcedDespawn();
        m_creature->AddObjectToRemoveList();
    }

    void Aggro(Unit* target)
    {
        hunter = target->GetTargetGuid();
    }

    void UpdateAI(const uint32 t)
    {
        ScriptedAI::UpdateAI(t);
    }

	void JustSummoned(Creature* summoner)
	{
		this->summoner = summoner->GetObjectGuid();
	}

    void JustDied(Unit* killer)
    {
		if(killer->GetObjectGuid() != hunter)
		{
			if(Creature* pSummoner = m_creature->FindMap()->GetCreature(summoner))
			{
                auto ai = static_cast<Hunterquest_Solenor*>(pSummoner->AI());
				if(ai)
					ai->Panik();
			}

            WorldLocation loc;
            killer->GetPosition(loc);
            Creature* cleaner = m_creature->SummonCreature(14503, loc.coord_x - 5, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);
            if(cleaner)
                cleaner->AddThreat(killer, 100000);
		}

        m_creature->ForcedDespawn();
        m_creature->AddObjectToRemoveList();
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new DoomBug(creature);
    }
};

struct Hunterquest_Nelson : public Hunterquest_BossTemplate
{
    Hunterquest_Nelson(Creature* creature) : Hunterquest_BossTemplate(creature) 
    {
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }
    void Reset() {}
    void Update(const uint32)
    {
        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
        {
            Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
            if(unit)
                DoCastSpellIfCan(unit, FOOLS_PLIGHT);
        }
    }

    static bool GossipHello(Player* pPlayer, Creature* pCreature)
    {
        if(pPlayer->IsActiveQuest(7636))
        {
            std::string msg = "I know who you are, reveal your true face, demon!";
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }

        pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());

        return true;
    }

    static bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
    {
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            WorldLocation loc;
            pCreature->GetPosition(loc);
			auto* creature = pCreature->SummonCreature(SLAYER, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
			if(creature)
			{
				DespawnDaemon(pCreature, 1200);
				auto* ai = static_cast<Hunterquest_BossTemplate*>(creature->AI());
				if(ai)
					ai->SetOwner(pPlayer->GetGUID());
			}
        }

        return true;
    }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_Nelson(creature);
    }
};

void AddSC_Hunter_Epic_Quest()
{
    /* Klinfran the Crazed	*/
    Script* pScript = new Script;
    pScript->GetAI = &Hunterquest_Klinfran::GetAI;
    pScript->Name = "Hunterquest_KlinfranCrazed";
    pScript->RegisterSelf(false);

    /* Franklin the Friendly */
    pScript = new Script;
    pScript->GetAI = &Hunterquest_Franklin::GetAI;
    pScript->Name = "Hunterquest_FranklinFriendly";
    pScript->pGossipHello = &Hunterquest_Franklin::GossipHello;
    pScript->pGossipSelect = &Hunterquest_Franklin::GossipSelect;
    pScript->RegisterSelf(false);

    /* Artorius the Doombringer */
    pScript = new Script;
    pScript->GetAI = &Hunterquest_Doombringer::GetAI;
    pScript->Name = "Hunterquest_ArtoriusDoom";
    pScript->RegisterSelf(false);

    /* Artorius the Amiable */
    pScript = new Script;
    pScript->GetAI = &Hunterquest_Amiable::GetAI;
    pScript->Name = "Hunterquest_ArtoriusAmia";
    pScript->pGossipHello = &Hunterquest_Amiable::GossipHello;
    pScript->pGossipSelect = &Hunterquest_Amiable::GossipSelect;
    pScript->RegisterSelf(false);

    /* Simone the Seductress	*/
    pScript = new Script;
    pScript->GetAI = &Hunterquest_SimoneSeductress::GetAI;
    pScript->Name = "Hunterquest_SimoneSeductress";
    pScript->RegisterSelf(false);

    /* Simone the Inconspicuous	*/
    pScript = new Script;
    pScript->Name = "Hunterquest_SimoneInconspicuous";
    pScript->GetAI = &Hunterquest_SimoneInconspicuois::GetAI;
    pScript->pGossipHello = &Hunterquest_SimoneInconspicuois::GossipHello;
    pScript->pGossipSelect = &Hunterquest_SimoneInconspicuois::GossipSelect;
    pScript->RegisterSelf(false);

    /* Precious */
    pScript = new Script;
    pScript->Name = "Hunterquest_Precious";
    pScript->GetAI = &Hunterquest_PetTemplate::GetAI;
    pScript->RegisterSelf(false);

    /* Solenor the Slayer */
    pScript = new Script;
    pScript->GetAI = &Hunterquest_Solenor::GetAI;
    pScript->Name = "Hunterquest_Solenor";
    pScript->RegisterSelf(false);

    /* Creeping Doom */
    pScript = new Script;
    pScript->GetAI = &DoomBug::GetAI;
    pScript->Name = "Hunterquest_DoomBug";
    pScript->RegisterSelf(false);

    /* Nelson the Nice */
    pScript = new Script;
    pScript->GetAI = &Hunterquest_Nelson::GetAI;
    pScript->Name = "Hunterquest_Nelson";
    pScript->pGossipHello = &Hunterquest_Nelson::GossipHello;
    pScript->pGossipSelect = &Hunterquest_Nelson::GossipSelect;
    pScript->RegisterSelf(false);
}