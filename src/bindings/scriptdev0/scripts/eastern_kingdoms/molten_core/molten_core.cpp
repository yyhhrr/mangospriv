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

ScriptName: Molten Core Trash
Instanz: Molten Core
*/

#include "precompiled.h"
#include "molten_core.h"

/*
### Großer Kernhund in Molten Core - ID: 11673 ###
*/

#define SPELL_LAVA_BREATH			19272
#define SPELL_BITE					19771

#define SPELL_GROUND_STOMP			19364
#define SPELL_ANCIENT_DREAD			19365
#define SPELL_CAUTERIZING_FLAMES	19366
#define SPELL_WITHERING_HEAT		19367
#define	SPELL_ANCIENT_DESPAIR		19369
#define	SPELL_ANCIENT_HYSTERIA		19372

struct MANGOS_DLL_DECL npc_core_houndAI : public ScriptedAI
{
    npc_core_houndAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 uiLavaBreath;
    uint32 uiBiteTimer;
    uint32 m_uiRandomDebuffTimer;
    uint32 m_uiRandomDebuffId;
    uint32 uiDespawnTimer;

    void Reset()
    {
        uiLavaBreath			= 5000;
        uiBiteTimer				= 8000;
        m_uiRandomDebuffTimer	= 10000;

        m_uiRandomDebuffId = 0;
        switch(urand(0,5))
        {
            case 0:
                m_uiRandomDebuffId = SPELL_GROUND_STOMP;
                break;
            case 1:
                m_uiRandomDebuffId = SPELL_ANCIENT_DREAD;
                break;
            case 2:
                m_uiRandomDebuffId = SPELL_CAUTERIZING_FLAMES;
                break;
            case 3:
                m_uiRandomDebuffId = SPELL_WITHERING_HEAT;
                break;
            case 4:
                m_uiRandomDebuffId = SPELL_ANCIENT_DESPAIR;
                break;
            case 5:
                m_uiRandomDebuffId = SPELL_ANCIENT_HYSTERIA;
                break;
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void JustRespawned()
    {
        if (pInstance->GetData(TYPE_TRASH_MAG) == DONE)
        {
            m_creature->setFaction(35);
            m_creature->SetVisibility(VISIBILITY_OFF);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiLavaBreath < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_LAVA_BREATH);
            uiLavaBreath = random(7000, 10000);
        }
		else uiLavaBreath -= diff;

        if (uiBiteTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BITE);
            uiBiteTimer = random(5000, 10000);
        }
		else uiBiteTimer -= diff;

        if (m_uiRandomDebuffTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_uiRandomDebuffId, CAST_TRIGGERED);
            m_uiRandomDebuffTimer = random(10000, 15000);
        }
        else
            m_uiRandomDebuffTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_core_hound(Creature *_Creature)
{
    return new npc_core_houndAI (_Creature);
}

/*
### Lava Woger in Molten Core - ID: 12101 ###
*/
#define SPELL_CHARGE	19196

struct MANGOS_DLL_DECL npc_lava_wogerAI : public ScriptedAI
{
    npc_lava_wogerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* pInstance;

    uint32 uiChargeTimer;

    void Reset()
    {
        uiChargeTimer = 5000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiChargeTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_CHARGE);

            uiChargeTimer = random(7000, 10000);
        }
		else uiChargeTimer -= diff;
		
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_lava_woger(Creature* pCreature)
{
    return new npc_lava_wogerAI (pCreature);
}

/*
### Lava Wichtel in Molten Core - ID: 11669 ###
*/
#define SPELL_FIRENOVA 20203

struct MANGOS_DLL_DECL npc_lava_wichtelAI : public ScriptedAI
{
    npc_lava_wichtelAI(Creature *c) : ScriptedAI(c)
	{
        Reset();
    }

	ScriptedInstance* pInstance;

	uint32 m_uiLavaNovaTimer;

    void Reset()
    {
        m_uiLavaNovaTimer = urand(10000, 35000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLavaNovaTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIRENOVA);
            m_uiLavaNovaTimer = random(8000, 25000);
        } else m_uiLavaNovaTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_lava_wichtel(Creature *_Creature)
{
    return new npc_lava_wichtelAI (_Creature);
}

/*
### Feuerlord in Molten Core - ID: 11668 ###
*/

#define SPELL_FIRE_BLAST	19393
#define SPELL_SPAWN_ADD		19392

struct MANGOS_DLL_DECL npc_feuer_lordAI : public ScriptedAI
{
    npc_feuer_lordAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
    }

	uint32 m_uiFireBlastTimer;
	uint32 m_uiAddSpawnTimer;

    void Reset()
    {
        m_uiFireBlastTimer	= 7000;
		m_uiAddSpawnTimer	= 5000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiAddSpawnTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SPAWN_ADD);
            m_uiAddSpawnTimer = random(6000, 8000);
        }
		else m_uiAddSpawnTimer -= diff;

        if (m_uiFireBlastTimer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_FIRE_BLAST);

            m_uiFireBlastTimer = random(4000, 8000);
        }
		else m_uiFireBlastTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_feuer_lord(Creature* pCreature)
{
    return new npc_feuer_lordAI (pCreature);
}

/*
### LavaSpawnAdd in Molten Core - ID: 12265 ###
*/

#define SPELL_FIREBALL		19391
#define SPELL_SPLITE		19569

struct MANGOS_DLL_DECL npc_lava_spawnAI : public ScriptedAI
{
    npc_lava_spawnAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
    }

	uint32 m_uiFireBallTimer;
	uint32 m_uiAddSpliteTimer;

    void Reset()
    {
        m_uiFireBallTimer	= 5000;
		m_uiAddSpliteTimer	= 15000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiAddSpliteTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SPLITE);
            m_uiAddSpliteTimer = 20000;
        }
		else m_uiAddSpliteTimer -= diff;

        if (m_uiFireBallTimer < diff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_FIREBALL);

            m_uiFireBallTimer = random(5000, 8000);
        }
		else m_uiFireBallTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_lava_spawn(Creature* pCreature)
{
    return new npc_lava_spawnAI (pCreature);
}

/*
### Kleiner Kernhund in Molten Core - ID: 11671 ###
*/
#define SPELL_KERNHUND_BISS		19771

struct MANGOS_DLL_DECL npc_kleiner_kernhundAI : public ScriptedAI
{
    npc_kleiner_kernhundAI(Creature *c) : ScriptedAI(c)
	{
        Reset();
		m_uiRespawnTimer = 0;
		m_RespawnMode = false;
    }

    Unit* target;

	WorldLocation mDeathPos;
	uint32 m_uiLavaHundBissTimer;
	uint32 m_uiRespawnTimer;
	std::list<uint64> mPackGuids;

	bool m_RespawnMode;

    void Reset()
    {
        m_uiLavaHundBissTimer = urand(3000,8000);
    }

	void Aggro(Unit* pWho)
    {
		m_creature->CallForHelp(5.0f);

		if(mPackGuids.size() == 0) {
			std::list<Creature*> packList;
			GetCreatureListWithEntryInGrid(packList, m_creature, NPC_KERNHUND, 30.0f);
			for(auto cr : packList) {
				std::list<Creature*> packListOther;
				GetCreatureListWithEntryInGrid(packListOther, cr, NPC_KERNHUND, 15.0f);
				bool hasFarHound = false;
				for(auto hound : packListOther) {
					if(hound->GetDistance(m_creature) > 25) {
						hasFarHound = true;
					}
				}

				if(hasFarHound)
					continue;

				mPackGuids.push_back(cr->GetGUID());
			}

			debug_log("Core hound %I64u has %u other hounds in pack", m_creature->GetGUID(), mPackGuids.size());
		}

		for(auto guid : mPackGuids) {
			Creature* cr = m_creature->GetMap()->GetCreature(guid);
			if(cr)
				cr->AI()->AttackStart(pWho);
		}
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void JustDied(Unit* pKiller)
    {
		m_creature->GetPosition(mDeathPos);
		m_uiRespawnTimer = 10000;
		m_RespawnMode = true;
    }

	void JustRespawned() {
		Reset();
		m_uiRespawnTimer = 0;
		m_RespawnMode = false;

		for(auto guid : mPackGuids) {
			Creature* cr = m_creature->GetMap()->GetCreature(guid);
			if(cr == nullptr)
				continue;

			if(!cr->isInCombat())
				continue;

			Unit* vic = cr->getVictim();
			if(vic != nullptr) {
				m_creature->AI()->AttackStart(vic);
				return;
			}
		}
	}

	void RequestRespawnCoords(float& x, float& y, float& z, float* orientation, float* radius) const {
		if(m_RespawnMode == false)
			return;

		bool isPackFighting = false;

		for(auto guid : mPackGuids) {
			Creature* cr = m_creature->GetMap()->GetCreature(guid);
			if(cr == nullptr)
				continue;

			if(cr->isInCombat())
				isPackFighting = true;
		}

		if(!isPackFighting)
			return;

		x = mDeathPos.coord_x;
		y = mDeathPos.coord_y;
		z = mDeathPos.coord_z;

		if(orientation != nullptr)
			*orientation = mDeathPos.orientation;

		if(radius != nullptr)
			*radius = 5;
	}

	void UpdateAIDead(const uint32 diff) {
		if (m_RespawnMode && diff >= m_uiRespawnTimer) {
			m_uiRespawnTimer = 0;

			bool hasOneAlive = std::find_if(mPackGuids.begin(), mPackGuids.end(),
				[this](uint64 guid) {
					Creature* cr = m_creature->GetMap()->GetCreature(guid);
					if(cr == nullptr)
						return false;

					return cr->isAlive();
				}) != mPackGuids.end();


			if(hasOneAlive) {
				m_creature->Respawn();
			}

			m_RespawnMode = false;

		} else if(m_uiRespawnTimer != 0) {
			m_uiRespawnTimer -= diff;
		}
	}

    void UpdateAI(const uint32 diff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiLavaHundBissTimer < diff)
		{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_KERNHUND_BISS);
				m_uiLavaHundBissTimer = random(4000, 8000);
		} else m_uiLavaHundBissTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_kleiner_kernhund(Creature *_Creature)
{
    return new npc_kleiner_kernhundAI (_Creature);
}

/*
### Molten Gigant in Molten Core - ID: 11658 ###
*/

#define SPELL_KICK		18945
#define SPELL_KRACHEN	18944

struct MANGOS_DLL_DECL npc_molten_gigantAI : public ScriptedAI
{
    npc_molten_gigantAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
    }

	uint32 m_uiKickSchlagTimer;
	uint32 m_uiKrachenTimer;

    void Reset()
    {
        m_uiKickSchlagTimer = urand(5000,7000);
		m_uiKrachenTimer	= urand(7000,10000);
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiKickSchlagTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_KICK);
            m_uiKickSchlagTimer = random(8000, 12000);
        }
		else m_uiKickSchlagTimer -= diff;

        if (m_uiKrachenTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_KRACHEN);
            m_uiKrachenTimer = random(5000, 9000);
        }
		else m_uiKrachenTimer -= diff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_molten_gigant(Creature *_Creature)
{
    return new npc_molten_gigantAI (_Creature);
}

bool GOUse_sulfuron_rune(Player* pPlayer, GameObject* pGo) //Sulfuron
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176951: //Sulfuron
            pInstance->SetData(TYPE_SULFURON, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Sulfuron gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Sulfuron clear!");
            break;
        }
    }
    return false;
}

bool GOUse_golemagg_rune(Player* pPlayer, GameObject* pGo) //Golemagg
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176954: //Golemagg
            pInstance->SetData(TYPE_GOLEMAGG, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Golemagg gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Golemagg clear!");
            break;
        }
    }
    return false;
}

bool GOUse_shazzrah_rune(Player* pPlayer, GameObject* pGo) //Shazzrah
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176953: //Shazzrah
            pInstance->SetData(TYPE_SHAZZRAH, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Shazzrah gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Shazzrah clear!");
            break;
        }
    }
    return false;
}

bool GOUse_garr_rune(Player* pPlayer, GameObject* pGo) //Garr
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176955: //Garr
            pInstance->SetData(TYPE_GARR, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Garr gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Garr clear!");
            break;
        }
    }
    return false;
}

bool GOUse_magmadar_rune(Player* pPlayer, GameObject* pGo) //Magmadar
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176956: //Magmadar
            pInstance->SetData(TYPE_MAGMADAR, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Magmadar gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Magmadar clear!");
            break;
        }
    }
    return false;
}

bool GOUse_gehennas_rune(Player* pPlayer, GameObject* pGo) //Gehennas
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176957: //Gehennas
            pInstance->SetData(TYPE_GEHENNAS, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Gehennas gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Gehennas clear!");
            break;
        }
    }
    return false;
}

bool GOUse_geddon_rune(Player* pPlayer, GameObject* pGo) //Geddon
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        switch (pGo->GetEntry())
        {
        case 176952: // Geddon
            pInstance->SetData(TYPE_GEDDON, SPECIAL);
			pPlayer->MonsterYell("Ich habe die Rune von Geddon gel\303\266scht!", LANG_UNIVERSAL, pPlayer);
			detail_log("Rune von Geddon clear!");
            break;
        }
    }
    return false;
}

void AddSC_molten_core()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_core_hound";
    newscript->GetAI = &GetAI_npc_core_hound;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_lava_woger";
    newscript->GetAI = &GetAI_npc_lava_woger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_feuer_wichtel";
    newscript->GetAI = &GetAI_npc_lava_wichtel;
    newscript->RegisterSelf();
	
	newscript = new Script;
    newscript->Name="npc_feuer_lord";
    newscript->GetAI = &GetAI_npc_feuer_lord;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="npc_lava_spawn";
    newscript->GetAI = &GetAI_npc_lava_spawn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_kleiner_kernhund";
    newscript->GetAI = &GetAI_npc_kleiner_kernhund;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_molten_gigant";
    newscript->GetAI = &GetAI_npc_molten_gigant;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_sulfuron_rune";
    newscript->pGOUse = &GOUse_sulfuron_rune;
    newscript->RegisterSelf();
	
	newscript = new Script;
    newscript->Name = "go_golemagg_rune";
    newscript->pGOUse = &GOUse_golemagg_rune;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_shazzrah_rune";
    newscript->pGOUse = &GOUse_shazzrah_rune;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_garr_rune";
    newscript->pGOUse = &GOUse_garr_rune;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_magmadar_rune";
    newscript->pGOUse = &GOUse_magmadar_rune;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_gehennas_rune";
    newscript->pGOUse = &GOUse_gehennas_rune;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name = "go_geddon_rune";
    newscript->pGOUse = &GOUse_geddon_rune;
    newscript->RegisterSelf();
}

