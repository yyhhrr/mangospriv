#include "precompiled.h"

enum
{
    EPICQUEST = 7636,
    MINUTES_30 = 1800,
    HOURS_4 = 10800,
    FOOLS_PLIGHT = 23504,
    DEMONIC_FRENZY = 23257,
    NPC_CLEANER = 14503,

};

static std::string GetFoolText(LocaleConstant lang)
{
    return (lang == LOCALE_deDE) ? "Nur ein Verr\303\274ckter w\303\274rde jetzt noch weiterk\303\244mpfen. Sodenn, Schwachkopf!" : "Only a fool would remain in this fight. So long, coward!";
}

static std::string GetTrueFaceText(LocaleConstant lang)
{

    return (lang == LOCALE_deDE) ? "Ich wei\303\237, wer Ihr wirklich seid. Zeigt Euer wahres Gesicht, D\303\244mon!" : "I know who you truly are. Reveal your true face, demon!";
}

struct Hunterquest_BossTemplate : public ScriptedAI
{
private:
	bool justSpawned;
	uint32 setupTime;
    	
public:
    Hunterquest_BossTemplate(Creature* creature) : ScriptedAI(creature)	{Reset();}

    void Reset()
    { 
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
        justSpawned = true;
        setupTime = 5000;
    }

    virtual void Update(const uint32 uidiff) = 0;

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

            Creature* cleaner = m_creature->SummonCreature(NPC_CLEANER, loc.coord_x - 5, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            if(cleaner)
                cleaner->AddThreat(unit, 100000);
        }
        if (Player* p = m_creature->GetMap()->GetPlayer(m_creature->getVictim()->GetObjectGuid()))
        {
            m_creature->MonsterSay(GetFoolText(p->GetSession()->GetSessionDbcLocale()).c_str(), 0);
        }
        else
            m_creature->MonsterSay(GetFoolText(LOCALE_deDE).c_str(), 0);
        
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

    static void DespawnDaemon(Creature* c)
    {
        c->ForcedDespawn(0);
        c->SetRespawnTime(urand(MINUTES_30, HOURS_4));
    }

    void UpdateAI(const uint32 uidiff)
    {
		if(justSpawned)
		{
            if (setupTime < uidiff)
			{
				m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PASSIVE);
				justSpawned = false;
			}
			else
                setupTime -= uidiff;
			return;
		}

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		auto* victim = m_creature->getVictim();

        ThreatList const& tl = m_creature->getThreatManager().getThreatList();
        if(tl.size() > 1 || victim->getClass() != CLASS_HUNTER)
        {
            Panik();
        }

        Update(uidiff);
        DoMeleeAttackIfReady();
    }
};

enum
{
    /* Spells */
    STINGING_TRAUMA = 23299,
    DEMONIC_DOOM = 23298,
    LULLISHOT = 19801,

    /* Entries */
    NPC_ARTORIUS_DEMON = 14535,
    NPC_ARTORIUS_FRIENDLY = 14531,
};

struct Hunterquest_Doombringer : public Hunterquest_BossTemplate
{
    uint32 m_DoomCheckIntervall;
    uint32 m_EnrageTimer;
    ObjectGuid hunter;

    Hunterquest_Doombringer(Creature* creature) : Hunterquest_BossTemplate(creature) { Reset(); }

    void Reset() 
    {
        m_DoomCheckIntervall = 500;
        m_EnrageTimer = 8000;
    }

    void Aggro(Unit* target) 
    {
        hunter = target->GetObjectGuid(); 
    }

    void Update(const uint32 uidiff)
    {
        if (m_EnrageTimer < uidiff)
        {
            if (DoCastSpellIfCan(m_creature, DEMONIC_FRENZY) == CAST_OK)
            {
                m_EnrageTimer = 15000;
            }
        }
        else
            m_EnrageTimer -= uidiff;

        if (m_DoomCheckIntervall < uidiff)
        {
            if (Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
            {
                float distance = m_creature->GetDistance(pHunter);
                if (distance <= 30.0f)
                    DoCastSpellIfCan(pHunter, DEMONIC_DOOM, CAST_TRIGGERED);
            }
            m_DoomCheckIntervall = 500;
        }
        else
            m_DoomCheckIntervall -= uidiff;
    }

    void SpellHit(Unit*, const SpellEntry* entry) 
    {
        
        if (m_creature->HasAura(DEMONIC_FRENZY) && entry->IsFitToFamily(SPELLFAMILY_HUNTER, 0x00000004000))
        {
            DoCastSpellIfCan(m_creature, STINGING_TRAUMA);
            m_creature->RemoveAurasWithDispelType(DISPEL_ENRAGE);
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
        if(pPlayer->IsCurrentQuest(EPICQUEST,1))
        {
            
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GetTrueFaceText(pPlayer->GetSession()->GetSessionDbcLocale()), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
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
            auto* creature = pCreature->SummonCreature(NPC_ARTORIUS_DEMON, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
			if(creature)
			{
				DespawnDaemon(pCreature);
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
    NPC_KLINFRAN_DEMON = 14534,
    NPC_KLINFRAN_FRIENDLY = 14529,
};

struct Hunterquest_Klinfran : public Hunterquest_BossTemplate
{
    uint32 m_EnrageTimer;
    float baseMinDmg;
    float baseMaxDmg;

    Hunterquest_Klinfran(Creature *creature) : Hunterquest_BossTemplate(creature) { Reset();}

    void Reset() 
    { 
        baseMinDmg = m_creature->GetFloatValue(UNIT_FIELD_MINDAMAGE);
        baseMaxDmg = m_creature->GetFloatValue(UNIT_FIELD_MAXDAMAGE);
        m_EnrageTimer = 8000;
        MaxDmg();
    }

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
    
    void Update(const uint32 uidiff)
    {

        if (m_EnrageTimer < uidiff)
        {
            if (DoCastSpellIfCan(m_creature, DEMONIC_FRENZY) == CAST_OK)
            {
                MaxDmg();
                m_EnrageTimer = 15000;
            }
        }
        else
            m_EnrageTimer -= uidiff;

        
    }

    void SpellHit(Unit*, const SpellEntry* entry) 
    {
        if (entry->Id == SCORPID_STING_RANK4 && m_creature->HasAura(DEMONIC_FRENZY))
        {
            m_creature->RemoveAurasWithDispelType(DISPEL_ENRAGE);
            MinDmg();
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
        if(pPlayer->IsCurrentQuest(EPICQUEST,1))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GetTrueFaceText(pPlayer->GetSession()->GetSessionDbcLocale()), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
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
            auto* creature = pCreature->SummonCreature(NPC_KLINFRAN_DEMON, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
			if(creature)
			{
				DespawnDaemon(pCreature);
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
    PET_FELHOUND = 14538,
    PET_FRIENDLY = 14528, 
    NPC_SIMONE_DEMON = 14533,
	NPC_SIMONE_FRIENDLY = 14527,
};

struct Hunterquest_PetTemplate : public ScriptedAI
{

public:
    Hunterquest_PetTemplate(Creature* creature) : ScriptedAI(creature) { }

    static CreatureAI* GetAI(Creature* creature)
    {
        return new Hunterquest_PetTemplate(creature);
    }

	
    void Reset()
    {
    }

    void Aggro(Unit* target)
    {
        Creature* cr = GetClosestCreatureWithEntry(m_creature, NPC_SIMONE_DEMON, 100);
        if(cr)
            cr->AddThreat(target, 100000);
        else
        {
            cr = GetClosestCreatureWithEntry(m_creature, NPC_SIMONE_FRIENDLY, 500);
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
        if(tl.size() > 1 || victim->getClass() != CLASS_HUNTER)
        {
            for (ThreatList::const_iterator itr = tl.begin();itr != tl.end(); ++itr)
            {
                Unit* unit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                WorldLocation loc;
                unit->GetPosition(loc);
                m_creature->SummonCreature(NPC_CLEANER, loc.coord_x - 5, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_MANUAL_DESPAWN, 1800000)->AddThreat(unit, 100000);
            };

            Creature* cr = GetClosestCreatureWithEntry(m_creature, NPC_SIMONE_DEMON, 100);
            if(cr)
            {
                if (Player* p = m_creature->GetMap()->GetPlayer(cr->getVictim()->GetObjectGuid()))
                {
                    cr->MonsterSay(GetFoolText(p->GetSession()->GetSessionDbcLocale()).c_str(), 0);
                }
                else
                    cr->MonsterSay(GetFoolText(LOCALE_deDE).c_str(), 0);

               
                cr->ForcedDespawn();
            }
            else
            {
                cr = GetClosestCreatureWithEntry(m_creature, NPC_SIMONE_FRIENDLY, 500);
                if(cr)
                {
                    if (Player* p = m_creature->GetMap()->GetPlayer(cr->getVictim()->GetObjectGuid()))
                    {
                        cr->MonsterSay(GetFoolText(p->GetSession()->GetSessionDbcLocale()).c_str(), 0);
                    }
                    else
                        cr->MonsterSay(GetFoolText(LOCALE_deDE).c_str(), 0);

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
    uint32 m_TemptressKissTimer;
    uint32 m_ChainLightningTimer;
    ObjectGuid hunter;

    Hunterquest_SimoneSeductress(Creature* creature) : Hunterquest_BossTemplate(creature) { Reset(); }
    
    
    void Reset() 
    {
        m_TemptressKissTimer = 0;
        m_ChainLightningTimer = 5000;
    }

    void Aggro(Unit* target)
    {
        DoCastSpellIfCan(target, TEMPTRESS_KISS);
        hunter = target->GetObjectGuid();
        Creature* felhound = GetClosestCreatureWithEntry(m_creature, PET_FELHOUND, 100);
        if (felhound)
        {
            felhound->AddThreat(target, 100000);
        }
        else if (felhound = m_creature->SummonCreature(PET_FELHOUND, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000))
        {
            felhound->AddThreat(target, 100000);
            felhound->Attack(target, true);
        }
     }

    void JustReachedHome()
    { 
        Creature* felhound = GetClosestCreatureWithEntry(m_creature, PET_FELHOUND, 100);
        if (!felhound)
        {
            felhound = m_creature->SummonCreature(PET_FELHOUND, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
        }
        

    }

    void Update(const uint32 uidiff)
    {
        if (Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
        {
            if (m_TemptressKissTimer < uidiff)
            {
                if (DoCastSpellIfCan(pHunter, TEMPTRESS_KISS) == CAST_OK)
                    m_TemptressKissTimer = 30000;
            }
            else
                m_TemptressKissTimer -= uidiff;

            if ( m_ChainLightningTimer < uidiff)
            {
                if (DoCastSpellIfCan(pHunter, CHAIN_LIGHTNING) == CAST_OK)
                        m_ChainLightningTimer = 10000;
            }
            else
                m_ChainLightningTimer -= uidiff;
        }
    }

    void SpellHit(Unit*, const SpellEntry* entry)
    {
        if (entry->Id == VIPER_STING)
        {
            m_ChainLightningTimer = 8500;
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
        if(pPlayer->IsCurrentQuest(EPICQUEST,1))
        {
            
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GetTrueFaceText(pPlayer->GetSession()->GetSessionDbcLocale()), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        }

        pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());

        return true;
    }

    static bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
    {
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            Creature* cr = GetClosestCreatureWithEntry(pCreature, PET_FRIENDLY, 100);
            if(cr)
            {
                WorldLocation loc;
                cr->GetPosition(loc);
                Creature* felhound = cr->SummonCreature(PET_FELHOUND, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
                cr->ForcedDespawn();

                pCreature->GetPosition(loc);
                auto* simone = pCreature->SummonCreature(NPC_SIMONE_DEMON, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
                if (simone)
				{
					DespawnDaemon(pCreature);
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
    NPC_SOLENOR_DEMON = 14530,
    NPC_SOLENOR_FRIENDLY = 14536,
    NPC_CREEPING_DOOM = 14761,
};


struct Hunterquest_Solenor : public Hunterquest_BossTemplate
{
    uint32 m_FrightenTimer;
    uint32 m_BugSpwanTimer;
    ObjectGuid hunter;

    bool applyAura;
    uint32 auraTimer;

    Hunterquest_Solenor(Creature* creature) : Hunterquest_BossTemplate(creature) { Reset(); DoCastSpellIfCan(m_creature, SOUL_FLAME);}

    void Reset()
    {
        m_FrightenTimer = 12000;
        m_BugSpwanTimer = 10000;
        applyAura = false;
        auraTimer = 0;
    }

    void Aggro(Unit* target) 
    {
        hunter = target->GetObjectGuid(); 
    }

    void Update(const uint32 uidiff)
    {
        if(m_creature->HasAura(ICE_TRAP))
        {
            m_creature->RemoveAura(SOUL_FLAME, EFFECT_INDEX_0);
            m_creature->RemoveAura(SOUL_FLAME, EFFECT_INDEX_1);
            m_creature->RemoveAura(SOUL_FLAME, EFFECT_INDEX_2);
        }

        if (m_FrightenTimer < uidiff)
        {
            if (Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
            {
                float distance = m_creature->GetDistance2d(pHunter->GetPositionX(), pHunter->GetPositionY());
                if (distance > 5.0f)
                    if (DoCastSpellIfCan(pHunter, DREAFUL_FRIGHT) == CAST_OK)
                        m_FrightenTimer = 12000;
            }
        }
        else
            m_FrightenTimer -= uidiff;

        //Hackfix - the aura sometimes disappear because of dmg done by the hunter
        if(applyAura)
        {
            auraTimer += uidiff;
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

        if (m_BugSpwanTimer < uidiff)
        {
            if (Unit* pHunter = m_creature->FindMap()->GetUnit(hunter))
            {
                float distance = m_creature->GetDistance2d(pHunter->GetPositionX(), pHunter->GetPositionY());
                if (distance > 5.0f)
                {
                    for (uint8 i = 0; i < 3; i++)
                    {
                        Creature* DoomBug = DoSpawnCreature(NPC_CREEPING_DOOM, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 1800000);
                        if (DoomBug)
                        {
                            DoomBug->AI()->AttackStart(pHunter);
                            DoomBug->AI()->JustSummoned(m_creature); //dosn't get called otherwise, weird
                            m_BugSpwanTimer = 10000;
                        }
                    }
                }
            }
        }
        else
            m_BugSpwanTimer -= uidiff;
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

    DoomBug(Creature* creature) : ScriptedAI(creature) { }
    void Reset()
    {
        m_creature->ForcedDespawn();
        m_creature->AddObjectToRemoveList();
    }

 

    void UpdateAI(const uint32 t)
    {
        ScriptedAI::UpdateAI(t);
    }

	void JustSummoned(Creature* summoner)
	{
		this->summoner = summoner->GetObjectGuid();
        hunter = summoner->getVictim()->GetObjectGuid();
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
            Creature* cleaner = m_creature->SummonCreature(NPC_CLEANER, loc.coord_x - 5, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);
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
        if(pPlayer->IsCurrentQuest(EPICQUEST,1))
        {
            
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GetTrueFaceText(pPlayer->GetSession()->GetSessionDbcLocale()), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
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
			auto* creature = pCreature->SummonCreature(NPC_SOLENOR_DEMON, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1200000);
			if(creature)
			{
				DespawnDaemon(pCreature);
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
    pScript->Name = "mob_simone_the_inconspicuous";
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