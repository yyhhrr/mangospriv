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
SDName: Eastern_Plaguelands
SD%Complete: 80
SDComment: Quest support: 5211, 5742, 7622. Special vendor Augustus the Touched
SDCategory: Eastern Plaguelands
EndScriptData */

/* ContentData
mobs_ghoul_flayer
npc_augustus_the_touched
npc_darrowshire_spirit
npc_tirion_fordring
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

//id8530 - cannibal ghoul
//id8531 - gibbering ghoul
//id8532 - diseased flayer

struct MANGOS_DLL_DECL mobs_ghoul_flayerAI : public ScriptedAI
{
    mobs_ghoul_flayerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            m_creature->SummonCreature(11064, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 60000);
    }

};

CreatureAI* GetAI_mobs_ghoul_flayer(Creature* pCreature)
{
    return new mobs_ghoul_flayerAI(pCreature);
}

/*######
## npc_augustus_the_touched
######*/

bool GossipHello_npc_augustus_the_touched(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pCreature->isVendor() && pPlayer->GetQuestRewardStatus(6164))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_augustus_the_touched(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());
    return true;
}

/*######
## npc_darrowshire_spirit
######*/

#define SPELL_SPIRIT_SPAWNIN    17321

struct MANGOS_DLL_DECL npc_darrowshire_spiritAI : public ScriptedAI
{
    npc_darrowshire_spiritAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        DoCastSpellIfCan(m_creature, SPELL_SPIRIT_SPAWNIN);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
};

CreatureAI* GetAI_npc_darrowshire_spirit(Creature* pCreature)
{
    return new npc_darrowshire_spiritAI(pCreature);
}

bool GossipHello_npc_darrowshire_spirit(Player* pPlayer, Creature* pCreature)
{
    pPlayer->SEND_GOSSIP_MENU(3873, pCreature->GetObjectGuid());
    pPlayer->TalkedToCreature(pCreature->GetEntry(), pCreature->GetObjectGuid());
    pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    return true;
}

/*######
## npc_tirion_fordring
######*/

enum
{
    QUEST_REDEMPTION             = 5742,

    TIRION_GOSSIP_ITEM_1         = -3000106,
    TIRION_GOSSIP_ITEM_2         = -3000107,
    TIRION_GOSSIP_ITEM_3         = -3000108,
    TIRION_GOSSIP_ITEM_4         = -3000109,
};

bool GossipHello_npc_tirion_fordring(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_REDEMPTION) == QUEST_STATUS_INCOMPLETE && pPlayer->getStandState() == UNIT_STAND_STATE_SIT)
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    else
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_tirion_fordring(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(4493, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(4494, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(4495, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(QUEST_REDEMPTION);
            break;
    }
    return true;
}
/*######
## npc_highlord_taelan_fordring
######*/

enum eHighLordTaelanFordring
{
    SPELL_CRUSADER_STRIKE         = 14518,
    //SPELL_DEVOTION_AURA           = 17232,
    SPELL_HOLY_CLEAVE             = 18819,
    SPELL_HOLY_LIGHT              = 15493,
    SPELL_HOLY_STRIKE             = 17143,
    SPELL_LAY_ON_HANDS            = 17233,
    SPELL_DEVOTION_AURA           = 8285,

    SPELL_MOUNT                   = 470,        // Taelan's horse
    SPELL_MOUNT2                  = 458,        // Tirion's horse
    SPELL_TAELAN_DEATH            = 18969,
    SPELL_TAELANS_SUFFERING       = 18810,

    QUEST_SCARLET_SUBTERFUGE      = 5862,
	QUEST_IN_DREAMS				  = 5944,
    NPC_SCARLET_CAVALIER          = 1836,
    NPC_GRAND_INQUISITOR_ISILLIEN = 1840,
    NPC_TIRION_FORDRING           = 1855,
    NPC_CRIMSON_ELITE             = 12128,
    
    FACTION_FRIENDLY              = 35,
    FACTION_FFP                   = 250,         // Friendly with players, but not with enviromental
    J_SUMMONED_DESPAWN_TIME       = 5*MINUTE*IN_MILLISECONDS,
    MAX_CREATURES                 = 7,           // This definition contains all summoned creatures which must be despawned at end of event

    SAY_GUARD1                    = -1000553,
    SAY_GUARD2                    = -1000554,
    SAY_GUARD3                    = -1000555,
    SAY_GUARD4                    = -1000556,
    SAY_GUARD5                    = -1000557,
    SAY_GUARD6                    = -1000558,
    SAY_GUARD7                    = -1000559,
    YELL_NOOO                     = -1000560,

    YELL_ISILLIEN                 = -1000561,
    YELL_TAELAN                   = -1000562,
    SAY_TAELAN_STRANGER           = -1000563,
    SAY_ISILLIEN1                 = -1000564,
    SAY_ISILLIEN2                 = -1000565,
    SAY_ISILLIEN3                 = -1000566,
    SAY_ISILLIEN4                 = -1000567,
    SAY_TIRION1                   = -1000568,
    SAY_ISILLIEN5                 = -1000569,
    SAY_TIRION2                   = -1000570,
    SAY_ISILLIEN6                 = -1000571,
    SAY_TIRION3                   = -1000572,
    SAY_TIRION4                   = -1000573,
    SAY_TIRION5                   = -1000574,
    SAY_TIRION6                   = -1000575,
    SAY_TIRION7                   = -1000576
};

struct MANGOS_DLL_DECL npc_highlord_taelan_fordringAI : public npc_escortAI
{
    npc_highlord_taelan_fordringAI(Creature* pCreature) : npc_escortAI(pCreature) {Reset();}
    
    uint32 m_uiCrusaderStrikeTimer;
    uint32 m_uiHolyCleaveTimer;
    uint32 m_uiHolyLightTimer;
    uint32 m_uiHolyStrikeTimer;
    uint32 m_uiMountTimer;

    uint32 EventPhase;
    uint32 EventTimer;
    uint32 DeathTimer;

    bool bCanMount;
    bool bUsedLayOnHands;
    bool CanWalk;

    std::list<Creature*> lTaelansGuards;
    Creature* pTowerCreatures[MAX_CREATURES];

    void Reset()
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            return;

        m_uiCrusaderStrikeTimer = 13000;
        m_uiHolyCleaveTimer = 10000;
        m_uiHolyLightTimer = 10000;
        m_uiHolyStrikeTimer = 5000;
        m_uiMountTimer = 0;

        EventPhase = 0;
        EventTimer = 0;
        DeathTimer = 0;

        bCanMount = false;
        bUsedLayOnHands = false;
        CanWalk = true;

        lTaelansGuards.clear();

        for(uint32 itr = 0; itr < MAX_CREATURES; ++itr)
            pTowerCreatures[itr] = 0;

        DoCastSpellIfCan(m_creature, SPELL_DEVOTION_AURA, CAST_AURA_NOT_PRESENT);

        DismountUnitIfCan();
    }
    
    void Aggro(Unit *)
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            DismountUnitIfCan();
        DoCastSpellIfCan(m_creature, SPELL_DEVOTION_AURA, CAST_AURA_NOT_PRESENT);
    }

    void JustDied(Unit* /*pKiller*/)
    {
        //DoCastSpellIfCan(m_creature, SPELL_TAELAN_DEATH, CAST_TRIGGERED);
    }

    void WaypointStart(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:
                m_creature->setFaction(FACTION_FFP);
                break;
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 19:
                bCanMount = true;
                SetRun(true);
                break;
            case 34:
                for(uint8 i = 0; i < 4; ++i)
                    if (Creature* pSummoned = m_creature->SummonCreature(NPC_CRIMSON_ELITE, 2710.37f+i, -1702.92f+i, 124.98f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                    {
                        pSummoned->GetMotionMaster()->MovePoint(NULL, 2761.21f, -1630.92f, 127.96f);
                        CreatureCreatePos pos(pSummoned->GetMap(), 2761.21f, -1630.92f, 127.96f, 0.0f);
						pSummoned->SetRespawnCoord(pos);
                    }
                break;
            case 43:
                CanWalk = false;
                bCanMount = false;
                DismountUnitIfCan();
                m_creature->SetFacingTo(5.21f);
                EventPhase = 1;
                EventTimer = 1;
                break;
        }
    }

    void DismountUnitIfCan(Unit* pUnit = 0)
    {
        if (!pUnit)
            pUnit = m_creature;

        if (pUnit && pUnit->isAlive() && pUnit->IsMounted())
        {
            pUnit->Unmount();
            pUnit->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
        }
    }

    void SpellHit(Unit *, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_TAELAN_DEATH && pTowerCreatures[0])
        {
            for(uint8 itr = 0; itr != 6; ++itr)
                if (pTowerCreatures[itr] && pTowerCreatures[itr]->isAlive())
                    if (pTowerCreatures[itr]->getVictim() && pTowerCreatures[itr]->getVictim()->GetEntry() == m_creature->GetEntry())
                        pTowerCreatures[itr]->AI()->EnterEvadeMode();

            m_creature->setFaction(FACTION_FRIENDLY);
            EnterEvadeMode();
            m_creature->StopMoving();
            m_creature->addUnitState(UNIT_STAT_CAN_NOT_MOVE);
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

            pTowerCreatures[0]->SetFacingToObject(m_creature);
            CreatureCreatePos pos(pTowerCreatures[0]->GetMap(), pTowerCreatures[0]->GetPositionX(), pTowerCreatures[0]->GetPositionY(), pTowerCreatures[0]->GetPositionZ(), pTowerCreatures[0]->GetOrientation());
			pTowerCreatures[0]->SetRespawnCoord(pos);
            pTowerCreatures[0]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pTowerCreatures[0]->AI()->EnterEvadeMode();
            EventTimer = 1500;
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        // Lay on Hands
        if (!bUsedLayOnHands && uiDamage >= m_creature->GetHealth())
        {
            bUsedLayOnHands = true;
            DoCastSpellIfCan(m_creature, SPELL_LAY_ON_HANDS, CAST_FORCE_CAST);
        }

        // Taelan's Death
        if ((((m_creature->GetHealth() - uiDamage)*100)/m_creature->GetMaxHealth()) <= 50)
            if (pTowerCreatures[0])
            {
                uiDamage = 0;
                pTowerCreatures[0]->InterruptNonMeleeSpells(false);
                pTowerCreatures[0]->CastSpell(m_creature, SPELL_TAELAN_DEATH, true);
            }
            else
                debug_log("SD0: Can't select Isillien in quest \"In Dreams!\" (phase Taelan's death)");
    }

    void DoStartEvent()
    {
        // "NOOOOOOOOOOOO!" Event
        EventPhase = 100;
        EventTimer = 1;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (CanWalk)
            npc_escortAI::UpdateAI(uiDiff);

        if (EventTimer)
        {
            if (EventTimer <= uiDiff)
            {
                switch(EventPhase)
                {
                    case 1:
                        m_creature->setFaction(FACTION_FRIENDLY);
                        if (pTowerCreatures[0] = m_creature->SummonCreature(NPC_GRAND_INQUISITOR_ISILLIEN, 2689.30f, -1936.92f, 72.13f, 2.10f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                        {
                            pTowerCreatures[0]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            DoScriptText(YELL_ISILLIEN, pTowerCreatures[0]);
                        }
                        EventTimer = 3000;
                        break;
                    case 2:
                        DoScriptText(YELL_TAELAN, m_creature);
                        EventTimer = 4000;
                        break;
                    case 3:
                        DoScriptText(SAY_TAELAN_STRANGER, m_creature);
                        if (pTowerCreatures[0])
                        {
                            pTowerCreatures[0]->GetMotionMaster()->MovePoint(NULL, 2677.28f, -1918.11f, 68.15f);
                            CreatureCreatePos pos(pTowerCreatures[0]->GetMap(), 2677.28f, -1918.11f, 68.15f, 2.10f);
							pTowerCreatures[0]->SetRespawnCoord(pos);
                        }
                        if (pTowerCreatures[1] = m_creature->SummonCreature(NPC_CRIMSON_ELITE, 2689.90f, -1937.98f, 72.13f, 2.10f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                        {
                            pTowerCreatures[1]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pTowerCreatures[1]->GetMotionMaster()->MovePoint(NULL, 2673.22f, -1920.70f, 68.21f);
                            CreatureCreatePos pos(pTowerCreatures[1]->GetMap(), 2673.22f, -1920.70f, 68.21f, 2.10f);
							pTowerCreatures[1]->SetRespawnCoord(pos);
                        }
                        if (pTowerCreatures[2] = m_creature->SummonCreature(NPC_CRIMSON_ELITE, 2689.90f, -1937.98f, 72.13f, 2.10f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                        {
                            pTowerCreatures[2]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pTowerCreatures[2]->GetMotionMaster()->MovePoint(NULL, 2682.19f, -1914.96f, 68.04f);
                            CreatureCreatePos pos(pTowerCreatures[2]->GetMap(), 2682.19f, -1914.96f, 68.04f, 2.10f);
							pTowerCreatures[2]->SetRespawnCoord(pos);
                        }
                        EventTimer = 8500;
                        break;
                    case 4:
                        if (pTowerCreatures[0])
                        {
                            pTowerCreatures[0]->HandleEmote(EMOTE_ONESHOT_TALK);
                            DoScriptText(SAY_ISILLIEN1, pTowerCreatures[0]);
                        }
                        EventTimer = 35000;
                        break;
                    case 5:
                        if (pTowerCreatures[0])
                            DoScriptText(SAY_ISILLIEN2, pTowerCreatures[0]);
                        EventTimer = 4000;
                        break;
                    case 6:
                        m_creature->setFaction(FACTION_FFP);
                        if (pTowerCreatures[0])
                        {
                            pTowerCreatures[0]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pTowerCreatures[0]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                            pTowerCreatures[0]->AI()->AttackStart(m_creature);
                            AttackStart(pTowerCreatures[0]);
                        }
                        for(uint8 itr = 1; itr < 3; ++itr)
                            if (pTowerCreatures[itr])
                            {
                                if (pTowerCreatures[itr]->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                                    pTowerCreatures[itr]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                if (!pTowerCreatures[itr]->getVictim())
                                    pTowerCreatures[itr]->AI()->AttackStart(m_creature);
                            }
                        for(uint8 itr = 3; itr < 6; ++itr)
                            if (pTowerCreatures[itr] = m_creature->SummonCreature(NPC_CRIMSON_ELITE, 2699.69f + rand()%3, -1862.04f + rand()%3, 67.51f, 4.06f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                            {
                                pTowerCreatures[itr]->AI()->AttackStart(m_creature);
                                switch(itr)
                                {
                                    case 3:
                                    {
                                        CreatureCreatePos pos(pTowerCreatures[itr]->GetMap(), 2669.17f, -1895.37f, 66.99f, 4.26f);
										pTowerCreatures[itr]->SetRespawnCoord(pos);
                                        break;
                                    }
                                    case 4:
                                    {
                                        CreatureCreatePos pos(pTowerCreatures[itr]->GetMap(), 2659.75f, -1901.16f, 68.04f, 6.23f);
										pTowerCreatures[itr]->SetRespawnCoord(pos);
                                        break;
                                    }
                                    case 5:
                                    {
                                        CreatureCreatePos pos(pTowerCreatures[itr]->GetMap(), 2676.84f, -1903.70f, 66.02f, 3.36f);
										pTowerCreatures[itr]->SetRespawnCoord(pos);
                                        break;
                                    }
                                }
                            }
                        EventTimer = 0;
                        break;
                    case 7:
                        if (pTowerCreatures[0])
                            DoScriptText(SAY_ISILLIEN3, pTowerCreatures[0]);
                        EventTimer = 2500;
                        break;
                    case 8:
                        if (pTowerCreatures[0])
                            DoScriptText(SAY_ISILLIEN4, pTowerCreatures[0]);
                        EventTimer = 8000;
                        break;
                    case 9:
                        if (pTowerCreatures[0] && GetPlayerForEscort())
                        {
                            pTowerCreatures[0]->SetTargetGuid(GetPlayerForEscort()->GetObjectGuid());
                            pTowerCreatures[0]->HandleEmote(EMOTE_ONESHOT_POINT);
                        }
                        EventTimer = 4000;
                        break;
                    case 10:
                        // Isillien start attacking escorter player.
                        if (pTowerCreatures[0] && pTowerCreatures[0]->isAlive() && GetPlayerForEscort() && GetPlayerForEscort()->isAlive())
                        {
                            CreatureCreatePos pos(pTowerCreatures[0]->GetMap(), 2677.28f, -1918.11f, 68.15f, 2.10f);
							pTowerCreatures[0]->SetRespawnCoord(pos);
                            pTowerCreatures[0]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_ATTACKABLE_1);
                            //pTowerCreatures[0]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                            pTowerCreatures[0]->AI()->AttackStart(GetPlayerForEscort());
                        }
                        EventTimer = 11000;
                        break;
                    case 11:
                        if (pTowerCreatures[6] = m_creature->SummonCreature(NPC_TIRION_FORDRING, 2634.07f, -1914.61f, 72.23f, 0.35f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                        {
                            pTowerCreatures[6]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							pTowerCreatures[6]->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
                            pTowerCreatures[6]->InterruptNonMeleeSpells(false);
                            pTowerCreatures[6]->CastSpell(pTowerCreatures[6], SPELL_MOUNT2, true);
                        }
                        EventTimer = 2000;
                        break;
                    case 12:
                        if (pTowerCreatures[6])
                        {
                            pTowerCreatures[6]->StopMoving();
                            pTowerCreatures[6]->GetMotionMaster()->Clear();
                            pTowerCreatures[6]->GetMotionMaster()->MovePoint(NULL, 2652.45f, -1908.32f, 69.64f);
                        }
                        EventTimer = 2000;
                        break;
                    case 13:
                        if (pTowerCreatures[6])
                        {
                            pTowerCreatures[6]->StopMoving();
                            //DismountUnitIfCan(pTowerCreatures[6]);
                            if (pTowerCreatures[6]->IsMounted())
                            {
                                pTowerCreatures[6]->Unmount();
                                pTowerCreatures[6]->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
                            }
                            pTowerCreatures[6]->SetFacingToObject(m_creature);
                            pTowerCreatures[6]->GetMotionMaster()->Clear();
                            pTowerCreatures[6]->GetMotionMaster()->MoveChase(m_creature, 3.0f, pTowerCreatures[6]->GetOrientation());
                            CreatureCreatePos pos(pTowerCreatures[6]->GetMap(), m_creature->GetPositionX()-2, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f);
							pTowerCreatures[6]->SetRespawnCoord(pos);
                            EventTimer = 2500;
                        }
                        break;
                    case 14:
                        if (pTowerCreatures[6])
                        {
                            if (pTowerCreatures[6]->IsWithinDist(m_creature, 4.0f))
                            {
                                CreatureCreatePos pos(pTowerCreatures[6]->GetMap(), pTowerCreatures[6]->GetPositionX(), pTowerCreatures[6]->GetPositionY(), pTowerCreatures[6]->GetPositionZ(), pTowerCreatures[6]->GetOrientation());
								pTowerCreatures[6]->SetRespawnCoord(pos);
                            }
                            else
                            {
                                pTowerCreatures[6]->Relocate(m_creature->GetPositionX()-2, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f);
                                --EventPhase;
                                EventTimer = 1;
                                break;
                            }
                            pTowerCreatures[6]->SetFacingToObject(m_creature);
                            pTowerCreatures[6]->SetStandState(UNIT_STAND_STATE_KNEEL);
                        }
                        EventTimer = 4000;
                        break;
                    case 15:
                        if (pTowerCreatures[6])
                        {
                            pTowerCreatures[6]->SetStandState(UNIT_STAND_STATE_STAND);
                            DoScriptText(SAY_TIRION1, pTowerCreatures[6]);
                        }
                        EventTimer = 10000;
                        break;
                    case 16:
                        if (pTowerCreatures[0] && pTowerCreatures[0]->isAlive())
                            DoScriptText(SAY_ISILLIEN5, pTowerCreatures[0]);
                        else if (pTowerCreatures[0] && pTowerCreatures[0]->isDead())
                        {
                            EventPhase = 20;
                            EventTimer = 1;
                            break;
                        }
                        EventTimer = 14000;
                        break;
                    case 17:
                        if (pTowerCreatures[0] && pTowerCreatures[0]->isAlive() && pTowerCreatures[6] && pTowerCreatures[6]->isAlive())
                        {
                            DoScriptText(SAY_TIRION2, pTowerCreatures[6]);
                            pTowerCreatures[6]->SetFacingToObject(pTowerCreatures[0]);
                            pTowerCreatures[6]->HandleEmote(EMOTE_ONESHOT_POINT);
                        }
                        EventTimer = 10000;
                        break;
                    case 18:
                        if (pTowerCreatures[0] && pTowerCreatures[0]->isAlive())
                            DoScriptText(SAY_ISILLIEN6, pTowerCreatures[0]);
                        EventTimer = 3500;
                        break;
                    case 19:
                        for(uint8 itr = 0; itr < MAX_CREATURES; ++itr)
                            if (pTowerCreatures[itr])
                                pTowerCreatures[itr]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_ATTACKABLE_1);
                        if (pTowerCreatures[0] && pTowerCreatures[6])
                        {
                            pTowerCreatures[6]->AI()->AttackStart(pTowerCreatures[0]);
                            pTowerCreatures[0]->DeleteThreatList();
                            pTowerCreatures[0]->AI()->AttackStart(pTowerCreatures[6]);
                        }
                        EventTimer = 0;
                        DeathTimer = 2500;
                        break;
                    case 20:
                        // Crimson Elites must be defeated
                        for(uint8 itr = 1; itr != 6; ++itr)
                            if (pTowerCreatures[itr] && pTowerCreatures[itr]->isAlive())
                                if (!pTowerCreatures[itr]->getVictim())
                                    if (pTowerCreatures[6] && pTowerCreatures[6]->isAlive() && pTowerCreatures[itr]->AI())
                                        pTowerCreatures[itr]->AI()->AttackStart(pTowerCreatures[6]);

                        if (pTowerCreatures[6])
                        {
                            if (!pTowerCreatures[6]->getVictim())
                            {
                                DoScriptText(SAY_TIRION3, pTowerCreatures[6]);
                                pTowerCreatures[6]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pTowerCreatures[6]->AI()->EnterEvadeMode();
                            }
                            else
                            {
                                --EventPhase;
                                EventTimer = 2500;
                            }
                        }
                        EventTimer = 8000;
                        break;
                    case 21:
                        if (pTowerCreatures[6] && m_creature)
                        {
                            pTowerCreatures[6]->SetFacingToObject(m_creature);
                            pTowerCreatures[6]->SetStandState(UNIT_STAND_STATE_KNEEL);
                        }
                        EventTimer = 2000;
                        break;
                    case 22:
                        if (pTowerCreatures[6])
                        {
                            DoScriptText(SAY_TIRION4, pTowerCreatures[6]);
                            pTowerCreatures[6]->SetStandState(UNIT_STAND_STATE_STAND);
                            pTowerCreatures[6]->HandleEmote(EMOTE_ONESHOT_EXCLAMATION);      // TODO: When core support Excited emote, use it!
                        }
                        EventTimer = 2000;
                        break;
                    case 23:
                        if (pTowerCreatures[6])
                            pTowerCreatures[6]->SetStandState(UNIT_STAND_STATE_KNEEL);
                        EventTimer = 4000;
                        break;
                    case 24:
                        if (pTowerCreatures[6])
                            DoScriptText(SAY_TIRION5, pTowerCreatures[6]);
                        EventTimer = 5000;
                        break;
                    case 25:
                        if (pTowerCreatures[6])
                            DoScriptText(SAY_TIRION6, pTowerCreatures[6]);
                        EventTimer = 5000;
                        break;
                    case 26:
                        if (GetPlayerForEscort())
                            GetPlayerForEscort()->GroupEventHappens(QUEST_IN_DREAMS, m_creature);
                        if (pTowerCreatures[6])
                        {
                            pTowerCreatures[6]->SetStandState(UNIT_STAND_STATE_STAND);
                            DoScriptText(SAY_TIRION7, pTowerCreatures[6]);
                            pTowerCreatures[6]->HandleEmote(EMOTE_ONESHOT_ROAR);
                        }
                        EventTimer = 45000;
                        break;
                    case 27:
                        // event done
                        /*for(uint8 itr = 0; itr < MAX_CREATURES-1; ++itr)
                            if (pTowerCreatures[itr])
                            {
                                pTowerCreatures[itr]->RemoveFromWorld();
                                pTowerCreatures[itr] = 0;
                            }*/
                        EventTimer = 0;
                        EventPhase = 0;
                        m_creature->ForcedDespawn();
                        break;

                    // NOOOOOOOOOOOO! event.
                    case 100:
                        m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);

                        GetCreatureListWithEntryInGrid(lTaelansGuards, m_creature, NPC_SCARLET_CAVALIER, 15.f);
                        if (!lTaelansGuards.empty())
                        {
                            uint8 pGuardNo = 0;
                            for(std::list<Creature*>::iterator itr = lTaelansGuards.begin(); itr != lTaelansGuards.end(); ++itr)
                                if ((*itr))
                                {
                                    if ((*itr)->isAlive())
                                    {
                                        ++pGuardNo;
                                        (*itr)->SetFacingToObject(m_creature);
                                        switch(pGuardNo)
                                        {
                                            case 1: DoScriptText(SAY_GUARD1, (*itr));
                                            case 2: DoScriptText(SAY_GUARD2, (*itr));
                                            case 3: DoScriptText(SAY_GUARD3, (*itr));
                                            case 4: DoScriptText(SAY_GUARD4, (*itr));
                                        }
                                    }
                                }
                        }
                        else
                            debug_log("SD0: Taelan cant find his guards around him.");

                        EventTimer = 3000;
                        break;
                    case 101:
                        if (!lTaelansGuards.empty())
                            for(std::list<Creature*>::iterator itr = lTaelansGuards.begin(); itr != lTaelansGuards.end(); ++itr)
                                if ((*itr))
                                    (*itr)->GetMotionMaster()->MoveFollow(m_creature, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
                        EventTimer = 7000;
                        break;
                    case 102:
                        DoScriptText(YELL_NOOO, m_creature);
                        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                        m_creature->HandleEmote(EMOTE_ONESHOT_ROAR);
                        if (!lTaelansGuards.empty())
                            for(std::list<Creature*>::iterator itr = lTaelansGuards.begin(); itr != lTaelansGuards.end(); ++itr)
                                if ((*itr))
                                    (*itr)->HandleEmote(EMOTE_ONESHOT_KICK);
                        EventTimer = 2000;
                        break;
                    case 103:
                        if (!lTaelansGuards.empty())
                            for(std::list<Creature*>::iterator itr = lTaelansGuards.begin(); itr != lTaelansGuards.end(); ++itr)
                                if ((*itr))
                                {
                                    DoScriptText(urand(0,1) == 1 ? SAY_GUARD5 : SAY_GUARD6, (*itr));
                                    (*itr)->AI()->AttackStart(m_creature);
                                    if (!m_creature->getVictim() && itr == lTaelansGuards.begin())
                                        AttackStart((*itr));
                                }
                        EventPhase = 0;
                        EventTimer = 0;
                        break;
                }
                ++EventPhase;
            }
            else
                EventTimer -= uiDiff;
        }

        if (DeathTimer)
        {
            if (DeathTimer <= uiDiff)
            {
                DeathTimer = 2500;
                if (pTowerCreatures[0])
                {
                    if (pTowerCreatures[0]->isDead())
                    {
                        EventTimer = 1;
                        DeathTimer = 0;
                    }
                }
                else
                    debug_log("SD0: Creature: Grand Inquisiton Isillien cant be selected for quest \"In Dream!\"");
            }
            else
                DeathTimer -= uiDiff;
        }

        // Mount
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            if (m_uiMountTimer <= uiDiff)
            {
                if (bCanMount && !m_creature->IsMounted() && !m_creature->getVictim())
                    DoCastSpellIfCan(m_creature, SPELL_MOUNT, CAST_TRIGGERED + CAST_FORCE_CAST + CAST_AURA_NOT_PRESENT);
                else if ((!bCanMount && m_creature->IsMounted()) || (m_creature->isInCombat()))
                    DismountUnitIfCan();
                m_uiMountTimer = 1000;
            }
            else
                m_uiMountTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCrusaderStrikeTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CRUSADER_STRIKE);
            m_uiCrusaderStrikeTimer = 10000;
        }
        else
            m_uiCrusaderStrikeTimer -= uiDiff;

        if (m_uiHolyCleaveTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HOLY_CLEAVE);
            m_uiHolyCleaveTimer = 10000;
        }
        else
            m_uiHolyCleaveTimer -= uiDiff;

        if (m_uiHolyLightTimer <= uiDiff)
        {
            Unit* pFriend = DoSelectLowestHpFriendly(15.f, 1500);
            if (pFriend)
                DoCastSpellIfCan(pFriend, SPELL_HOLY_LIGHT, CAST_FORCE_CAST);
            m_uiHolyLightTimer = urand(5000,15000);
        }
        else
            m_uiHolyLightTimer -= uiDiff;

        if (m_uiHolyStrikeTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HOLY_STRIKE);
            m_uiHolyStrikeTimer = 10000;
        }
        else
            m_uiHolyStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_highlord_taelan_fordring(Creature* pCreature)
{
    return new npc_highlord_taelan_fordringAI(pCreature);
}

bool QuestAccept_npc_highlord_taelan_fordring(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_IN_DREAMS)
        if (npc_highlord_taelan_fordringAI* pEscortAI = dynamic_cast<npc_highlord_taelan_fordringAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest);
    return true;
}

bool QuestRewarded_npc_highlord_taelan_fordring(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_SCARLET_SUBTERFUGE)
        if (npc_highlord_taelan_fordringAI* pTaelanAI = dynamic_cast<npc_highlord_taelan_fordringAI*>(pCreature->AI()))
            pTaelanAI->DoStartEvent();
    return true;
}

enum
{
	DISPLAY_ID_GHOST_MOUNT = 17328,
	NPC_GRYPHON = 9526,
	SAY_GUARDS = -549,
	SPELL_AURA_HORDE = 31309,
	SPELL_AURA_ALLIANCE = 31748,
	SPELL_AURA_REMOVEME = 17327,
	FACTION_HORDE = 83,
	FACTION_ALLIANCE = 84,

	PATH_Northpass_Tower = 494,
	PATH_Eastwall_Tower = 495,
	PATH_Crown_Guard = 496,

	TEXT_NORTHPASS = -3469205,
	TEXT_EASTWALL = -3469206,
	TEXT_CROWNGUARD = -3469207,
};

bool GossipHello_william_kielar(Player* pPlayer, Creature* pCreature)
{
	pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());
	pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_TAXI, TEXT_NORTHPASS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_TAXI, TEXT_EASTWALL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
	pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_TAXI, TEXT_CROWNGUARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_william_kielar(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
        pPlayer->GetSession()->SendDoFlight(17328, PATH_Northpass_Tower);
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2)
        pPlayer->GetSession()->SendDoFlight(17328, PATH_Eastwall_Tower);
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 3)
        pPlayer->GetSession()->SendDoFlight(17328, PATH_Crown_Guard);
    return true;
}

struct MANGOS_DLL_DECL mob_william_kielar : public ScriptedAI
{
	bool justSpawned;

    mob_william_kielar(Creature* pCreature) : ScriptedAI(pCreature)
    {
		justSpawned = true;
	}

	void Reset() {}

	void UpdateAI(uint32)
	{
		if(justSpawned)
		{
			m_creature->RemoveAura(SPELL_AURA_REMOVEME, EFFECT_INDEX_0);
			if(m_creature->getFaction() == FACTION_HORDE)
				m_creature->CastSpell(m_creature, SPELL_AURA_HORDE, true);
			else
				m_creature->CastSpell(m_creature, SPELL_AURA_ALLIANCE, true);
			justSpawned = false;
		}
	}
};


void AddSC_eastern_plaguelands()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mobs_ghoul_flayer";
    pNewScript->GetAI = &GetAI_mobs_ghoul_flayer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_augustus_the_touched";
    pNewScript->pGossipHello = &GossipHello_npc_augustus_the_touched;
    pNewScript->pGossipSelect = &GossipSelect_npc_augustus_the_touched;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_darrowshire_spirit";
    pNewScript->GetAI = &GetAI_npc_darrowshire_spirit;
    pNewScript->pGossipHello = &GossipHello_npc_darrowshire_spirit;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_tirion_fordring";
    pNewScript->pGossipHello = &GossipHello_npc_tirion_fordring;
    pNewScript->pGossipSelect = &GossipSelect_npc_tirion_fordring;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_highlord_taelan_fordring";
    pNewScript->GetAI = &GetAI_npc_highlord_taelan_fordring;
    pNewScript->pQuestAcceptNPC =   &QuestAccept_npc_highlord_taelan_fordring;
    pNewScript->pQuestRewardedNPC = &QuestRewarded_npc_highlord_taelan_fordring;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_william_kielar";
    pNewScript->GetAI = [](Creature* c) -> CreatureAI*
	{
		return new mob_william_kielar(c);
	};
    pNewScript->pGossipHello = &GossipHello_william_kielar;
    pNewScript->pGossipSelect = &GossipSelect_william_kielar;
    pNewScript->RegisterSelf();
}
