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
#include "escort_ai.h"
#include "follower_ai.h"
#include "dire_maul.h"

enum pus
{
    SAY_1   = -1901000,
    SAY_2   = -1901001,                     
    SAY_3   = -1901002,
    SAY_4   = -1901003,
    SAY_5   = -1901004,

    NPC_IMP = 13276
};

enum eSpells
{
    SPELL_BLAST_WAVE	= 17145,
    SPELL_FIRE_BLAST	= 14145,
    SPELL_FIREBALL		= 15228,
    SPELL_RUNN			= 22735
};

enum SpawnPosition
{
    POS_EINS	= 0,
    POS_ZWEI	= 1,
    POS_DREI	= 2,
	POS_VIER	= 3,
};

static const float afSpawnLocations1[4][4] =
{
    {23.019f,-703.442f,-12.642f,3.23f},		// 1
    {22.646f,-691.948f,-12.642f,4.13f},		// 2
    {12.277f,-676.333f,-12.642f,4.97f},		// 3
    {24.416f,-680.573f,-12.642f,4.31f}		// 4
};

#define GOSSIP_HELLO	"Ein Spiel? Bist du verr\303\274ckt?"
#define Gossip_2		"Warum so klein?..."
#define Gossip_3		"Ich werde dich fangen!"
#define Gossip_4		"STIRB!"
#define Gossip_5		"Bereite dich endlich auf einen Kampf vor!"

struct MANGOS_DLL_DECL npc_pusillinAI : public npc_escortAI
{
    npc_pusillinAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

		ScriptedInstance* pInstance;

        uint32 uiPhase;
        uint32 uiGossipStep;
        uint32 uiBlastWaveTimer;
        uint32 uiFireBlastTimer;
        uint32 uiFireBallTimer;
        uint32 uiSpiritOfRunnTimer;
        bool buffed;

        void Reset()
        {
            uiPhase 			= 0;
            uiGossipStep 		= 0;
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            uiBlastWaveTimer 	= urand(7000, 9000);
            uiFireBlastTimer 	= urand(3000, 6000);
            uiFireBallTimer  	= urand(11000, 13000);
            uiSpiritOfRunnTimer = urand(5000, 7000);
            buffed=false;
        }
        
        void SetHoldState(bool bOnHold)
        {
            SetEscortPaused(bOnHold);
        }
        
        void WaypointReached(uint32 uiPointId)
        {
            switch(uiPointId)
            {
            case 0:
                break;
            case 2:
                uiGossipStep = 1;
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                SetHoldState(true);
                break;
            case 4:
                uiGossipStep = 2;
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                SetHoldState(true);
                break;
            case 10:
                uiGossipStep = 3;
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                SetHoldState(true);
                break;
            case 16:
                uiGossipStep = 4;
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
				SetHoldState(true);
                break;
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);
                
            if (uiPhase)
            {
                switch(uiPhase)
                {
                case 1:
                    DoScriptText(SAY_1, m_creature);
                    uiPhase=0;
                    break;
                case 2:   
                    DoScriptText(SAY_2, m_creature);
                    uiPhase=0;
                    break;
                case 3:
                    DoScriptText(SAY_3, m_creature);
                    uiPhase=0;
                    break;
                case 4:
                    DoScriptText(SAY_4, m_creature);
                    uiPhase=0;
                    break;
                case 5:
                    DoScriptText(SAY_5, m_creature);
                    m_creature->setFaction(14);
                    float scale = m_creature->GetFloatValue(OBJECT_FIELD_SCALE_X);
                    m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, scale * 3);

                    Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                    m_creature->AI()->AttackStart(target);
                    for(uint8 i = 0; i<5; i++)
                    {
                        switch(i)
                        {
                        case 1:
                            if (Creature *pImp=m_creature->SummonCreature(NPC_IMP, afSpawnLocations1[POS_EINS][0], afSpawnLocations1[POS_EINS][1], afSpawnLocations1[POS_EINS][2], afSpawnLocations1[POS_EINS][3], TEMPSUMMON_CORPSE_DESPAWN, 120000))
                                {
                                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                    pImp->AI()->AttackStart(target);
                                }
                            break;
                        case 2:
                            if (Creature *pImp=m_creature->SummonCreature(NPC_IMP, afSpawnLocations1[POS_ZWEI][0], afSpawnLocations1[POS_ZWEI][1], afSpawnLocations1[POS_ZWEI][2], afSpawnLocations1[POS_ZWEI][3], TEMPSUMMON_CORPSE_DESPAWN, 120000))
                                {
                                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                    pImp->AI()->AttackStart(target);
                                }
                            break;
                        case 3:
                            if (Creature *pImp=m_creature->SummonCreature(NPC_IMP, afSpawnLocations1[POS_DREI][0], afSpawnLocations1[POS_DREI][1], afSpawnLocations1[POS_DREI][2], afSpawnLocations1[POS_DREI][3], TEMPSUMMON_CORPSE_DESPAWN, 120000))
                                {
                                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                    pImp->AI()->AttackStart(target);
                                }
                            break;
                        case 4:
                            if (Creature *pImp=m_creature->SummonCreature(NPC_IMP, afSpawnLocations1[POS_VIER][0], afSpawnLocations1[POS_VIER][1], afSpawnLocations1[POS_VIER][2], afSpawnLocations1[POS_VIER][3], TEMPSUMMON_CORPSE_DESPAWN, 120000))
                                {
                                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                    pImp->AI()->AttackStart(target);
                                }
                            break;
                        }
                    }
                    uiPhase=6;
                    break;
                }
  
			}
    
            if (uiPhase==6)
            {
			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

                if (uiBlastWaveTimer <= uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_BLAST_WAVE);
                    uiBlastWaveTimer = urand(7000, 9000);
                }
                else uiBlastWaveTimer -= uiDiff;

                if (uiFireBlastTimer <= uiDiff)
                {
                    DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), SPELL_FIRE_BLAST);
                    uiFireBlastTimer = urand(3000, 6000);
                }
                else uiFireBlastTimer -= uiDiff;

                if (uiFireBallTimer <= uiDiff)
                {
                    DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), SPELL_FIREBALL);
                    uiFireBallTimer  = urand(11000, 13000);
                }
                else uiFireBallTimer -= uiDiff;

                if (uiSpiritOfRunnTimer <= uiDiff && buffed==false)
                {
                    DoCast(m_creature, SPELL_RUNN);
                    buffed = true;
                }
                else uiSpiritOfRunnTimer -= uiDiff;         

                DoMeleeAttackIfReady();
            }
        }
    };
        
bool GossipHello_npc_pusillin(Player* pPlayer, Creature* pCreature)
{
	 npc_pusillinAI* pEscortAI = dynamic_cast<npc_pusillinAI*>(pCreature->AI());

	 switch (pEscortAI->uiGossipStep)
        {
		case 0:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(6877, pCreature->GetGUID());
			break;
		
		case 1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, Gossip_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(6878, pCreature->GetGUID());
			break;
	    
		case 2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, Gossip_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(6879, pCreature->GetGUID());
			break;
		
		case 3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, Gossip_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(6880, pCreature->GetGUID());
			break;
		
		case 4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, Gossip_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(6881, pCreature->GetGUID());
			break;
	 }
        return true;
}

bool GossipSelect_npc_pusillin(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
	//npc_pusillinAI* pEscortAI = dynamic_cast<npc_pusillinAI*>(pCreature->AI());

	if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
			if (npc_pusillinAI* pEscortAI = static_cast<npc_pusillinAI*>(pCreature->AI()))
			{
				pEscortAI->Start(true, pPlayer);
				pEscortAI->SetHoldState(false);
				pEscortAI->uiPhase = 1;
			}
        }

	if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
        {
			if (npc_pusillinAI* pEscortAI = static_cast<npc_pusillinAI*>(pCreature->AI()))
			{
				pEscortAI->uiPhase = 2;
				pEscortAI->SetHoldState(false);
			}
        }
	
	if (uiAction == GOSSIP_ACTION_INFO_DEF+3)
        {
			if (npc_pusillinAI* pEscortAI = static_cast<npc_pusillinAI*>(pCreature->AI()))
			{
				pEscortAI->uiPhase = 3;
				pEscortAI->SetHoldState(false);
			}
        }
        
        if (uiAction == GOSSIP_ACTION_INFO_DEF+4)
        {
			if (npc_pusillinAI* pEscortAI = static_cast<npc_pusillinAI*>(pCreature->AI()))
			{
				pEscortAI->uiPhase = 4;
				pEscortAI->SetHoldState(false);
			}
        }
        
        if (uiAction == GOSSIP_ACTION_INFO_DEF+5)
        {
			if (npc_pusillinAI* pEscortAI = static_cast<npc_pusillinAI*>(pCreature->AI()))
			{
				pEscortAI->uiPhase = 5;
			}
        }

        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        return true;
}

CreatureAI* GetAI_npc_pusillin(Creature* pCreature)
{
    return new npc_pusillinAI(pCreature);
}

void AddSC_npc_pusillin()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_pusillin";
    pNewScript->pGossipHello =  &GossipHello_npc_pusillin;
    pNewScript->pGossipSelect = &GossipSelect_npc_pusillin;
    pNewScript->GetAI = &GetAI_npc_pusillin;
    pNewScript->RegisterSelf();
}