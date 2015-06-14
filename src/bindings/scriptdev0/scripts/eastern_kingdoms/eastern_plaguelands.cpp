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
    mobs_ghoul_flayerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

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
    npc_darrowshire_spiritAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

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
    QUEST_REDEMPTION = 5742,

    TIRION_GOSSIP_ITEM_1 = -3000106,
    TIRION_GOSSIP_ITEM_2 = -3000107,
    TIRION_GOSSIP_ITEM_3 = -3000108,
    TIRION_GOSSIP_ITEM_4 = -3000109,
};

bool GossipHello_npc_tirion_fordring(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_REDEMPTION) == QUEST_STATUS_INCOMPLETE && pPlayer->getStandState() == UNIT_STAND_STATE_SIT)
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    else
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_tirion_fordring(Player* pPlayer, Creature* pCreature, uint32, uint32 uiAction)
{
    switch (uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF + 1:
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        pPlayer->SEND_GOSSIP_MENU(4493, pCreature->GetObjectGuid());
        break;
    case GOSSIP_ACTION_INFO_DEF + 2:
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        pPlayer->SEND_GOSSIP_MENU(4494, pCreature->GetObjectGuid());
        break;
    case GOSSIP_ACTION_INFO_DEF + 3:
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TIRION_GOSSIP_ITEM_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        pPlayer->SEND_GOSSIP_MENU(4495, pCreature->GetObjectGuid());
        break;
    case GOSSIP_ACTION_INFO_DEF + 4:
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->AreaExploredOrEventHappens(QUEST_REDEMPTION);
        break;
    }
    return true;
}
/*######
## npc_highlord_taelan_fordring
######*/

struct EventLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static EventLocations spawnPosition[] =
{
    { 2690.27f, -1730.42f, 117.072f }, // Pat 1/4
    { 2694.08f, -1732.34f, 116.889f }, // Pat 2/4
    { 2696.71f, -1733.64f, 117.148f }, // Pat 3/4
    { 2686.65f, -1728.84f, 118.004f }, // Pat 4/4
    { 2688.4f, -1871.67f, 66.7319f }, // Isill 1/3
    { 2693.5f, -1875.21f, 66.7839f }, // Isill 2/3
    { 2696.9f, -1878.29f, 66.7887f }, // Isill 3/3
};

enum eHighLordTaelanFordring
{
    SPELL_CRUSADER_STRIKE = 14518,
    SPELL_HOLY_CLEAVE = 18819,
    SPELL_HOLY_LIGHT = 15493,
    SPELL_HOLY_STRIKE = 17143,
    SPELL_LAY_ON_HANDS = 17233,
    SPELL_DEVOTION_AURA = 17232,

    SPELL_MOUNT = 470,        // Taelan's horse
    SPELL_MOUNT2 = 458,        // Tirion's horse
    SPELL_TAELAN_DEATH = 18969,
    SPELL_TAELANS_SUFFERING = 18810,

    QUEST_SCARLET_SUBTERFUGE = 5862,
    QUEST_IN_DREAMS = 5944,
    NPC_SCARLET_CAVALIER = 1836,
    NPC_GRAND_INQUISITOR_ISILLIEN = 1840,
    NPC_TIRION_FORDRING = 1855,
    NPC_CRIMSON_ELITE = 12128,

    FACTION_FRIENDLY = 35,
    FACTION_FFP = 250,         // Friendly with players, but not with enviromental
    J_SUMMONED_DESPAWN_TIME = 5 * MINUTE*IN_MILLISECONDS,
    MAX_CREATURES = 5,           // This definition contains all summoned creatures which must be despawned at end of event

    SAY_GUARD1 = -1001072,
    SAY_GUARD2 = -1001073,
    SAY_GUARD3 = -1001074,
    SAY_GUARD4 = -1001075,
    YELL_NOOO = -1001076,

    SAY_ESCORT_START = -1001077,
    SAY_ESCORT_START2 = -1001078,
    SAY_TAELAN_HALT = -1001079,
    EMOTE_TAELAN_MOUNT = -1001080,
    TAELAN_REACH_TOWER = -1001081,
    YELL_ISILLIEN = -1001082,
    YELL_TAELAN = -1001083,
    SAY_TAELAN_STRANGER = -1001084,
    SAY_ISILLIEN1 = -1001085,
    SAY_ISILLIEN2 = -1001086,
    SAY_ISILLIEN3 = -1001087,
    SAY_ISILLIEN4 = -1001088,
    SAY_ISILLIEN_ATTACK = -1001089,
    SAY_KILL_TAELAN_1 = -1001090,
    EMOTE_ISILLIEN_LAUGH = -1001091,
    SAY_KILL_TAELAN_2 = -1001092,
    EMOTE_ISILLIEN_FOCUS = -1001093,

    SAY_TIRION1 = -1001094,
    SAY_ISILLIEN5 = -1001095,
    SAY_TIRION2 = -1001096,
    SAY_TIRION3 = -1001097,
    SAY_ISILLIEN6 = -1001098,

    SAY_TIRION4 = -1001099,
    EMOTE_TIRION_KNEE = -1001100,
    SAY_TIRION5 = -1001101,
    EMOTE_TIRION_HOLD = -1001102,
    SAY_TIRION6 = -1001103,
    SAY_TIRION7 = -1001104,
    SAY_TIRION8 = -1001105,
};

struct MANGOS_DLL_DECL npc_highlord_taelan_fordringAI : public npc_escortAI
{
    npc_highlord_taelan_fordringAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    uint32 m_uiLayOnHandsTimer;
    uint32 m_uiCrusaderStrikeTimer;
    uint32 m_uiHolyCleaveTimer;
    uint32 m_uiHolyLightTimer;
    uint32 m_uiHolyStrikeTimer;
    uint32 m_uiMountTimer;
    uint32 m_HealthRegenTimer;

    uint32 EventPhase;
    uint32 EventTimer;
    uint32 DeathTimer;

    bool bCanMount;
    bool endFight;
    bool CanWalk;
    bool m_bTaelanDead;

    ObjectGuid isillien;
    ObjectGuid tirionFordring;

    std::list<ObjectGuid> pat;
    std::list<ObjectGuid> lTaelansGuards;
    ObjectGuid pTowerCreatures[MAX_CREATURES];

    void Reset()
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            return;

        m_uiLayOnHandsTimer = 20000;
        m_uiCrusaderStrikeTimer = 13000;
        m_uiHolyCleaveTimer = 10000;
        m_uiHolyLightTimer = 10000;
        m_uiHolyStrikeTimer = 5000;
        m_uiMountTimer = 0;
        m_bTaelanDead = false;
        m_HealthRegenTimer = 2000;

        EventPhase = 0;
        EventTimer = 0;
        DeathTimer = 0;

        bCanMount = false;
        endFight = false;
        CanWalk = true;

        pat.clear();
        lTaelansGuards.clear();
        isillien = ObjectGuid();
        tirionFordring = ObjectGuid();
        for (uint32 i = 0; i < MAX_CREATURES; ++i)
            pTowerCreatures[i] = ObjectGuid();

        DoCastSpellIfCan(m_creature, SPELL_DEVOTION_AURA, CAST_AURA_NOT_PRESENT);

        DismountUnitIfCan();
    }

    void Aggro(Unit *)
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            DismountUnitIfCan();
        DoCastSpellIfCan(m_creature, SPELL_DEVOTION_AURA, CAST_AURA_NOT_PRESENT);
    }

    void WaypointStart(uint32 uiPointId)
    {
        switch (uiPointId)
        {
        case 0:
            m_creature->setFaction(FACTION_FFP);
            break;
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch (uiPointId)
        {
        case 1:
            DoScriptText(SAY_ESCORT_START, m_creature);

            break;
        case 2:
            DoScriptText(SAY_ESCORT_START2, m_creature);
            break;

        case 16:
            bCanMount = true;


            DoScriptText(SAY_TAELAN_HALT, m_creature);
            DoScriptText(EMOTE_TAELAN_MOUNT, m_creature);

            break;
        case 35:
            for (uint8 i = 0; i < 4; ++i)
            {
                if (Creature* pSummoned = m_creature->SummonCreature(NPC_CRIMSON_ELITE, spawnPosition[i].m_fX, spawnPosition[i].m_fY, spawnPosition[i].m_fZ, 1.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, J_SUMMONED_DESPAWN_TIME))
                {
                    pSummoned->GetMotionMaster()->MovePoint(NULL, 2742.21f, -1656.92f, 126.96f);

                }
            }
            break;
        case 43:
            CanWalk = false;
            bCanMount = false;
            DismountUnitIfCan();
            m_creature->SetFacingToObject(GetPlayerForEscort());
            EventPhase = 1;
            EventTimer = 1;
            DoScriptText(TAELAN_REACH_TOWER, m_creature);
            SetEscortPaused(true);

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
        if (pSpell->Id == SPELL_TAELAN_DEATH && isillien)
        {
            for (uint8 itr = 0; itr < MAX_CREATURES; ++itr)
            {
                Creature* pCreature = m_creature->FindMap()->GetCreature(pTowerCreatures[itr]);
                if (!pCreature)
                    continue;
                if (pCreature->isAlive())
                if (pCreature->getVictim() && pCreature->getVictim()->GetEntry() == m_creature->GetEntry() && pCreature->AI())
                    pCreature->AI()->EnterEvadeMode();
            }

            // fake Death
            m_creature->RemoveAllAurasOnEvade();
            m_creature->DeleteThreatList();
            m_creature->CombatStop(true);
            m_creature->SetLootRecipient(NULL);

            m_creature->InterruptNonMeleeSpells(true);
            m_creature->SetHealth(0);
            m_creature->StopMoving();
            m_creature->ClearComboPointHolders();
            m_creature->RemoveAllAurasOnDeath();
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->ClearAllReactives();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->addUnitState(UNIT_STAT_CAN_NOT_MOVE);
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

            Reset();

            if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
            {
                pIsillien->SetFacingToObject(m_creature);
                CreatureCreatePos pos(pIsillien->GetMap(), pIsillien->GetPositionX(), pIsillien->GetPositionY(), pIsillien->GetPositionZ(), pIsillien->GetOrientation());
                pIsillien->SetRespawnCoord(pos);
                pIsillien->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pIsillien->AI()->EnterEvadeMode();
            }
            EventTimer = 1500;
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        // Taelan's Death (Fakedeath)
        if (m_creature->GetHealthPercent() > 70)
            return;

        if (!m_bTaelanDead)
        {
            if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
            {
                uiDamage = 0;
                pIsillien->InterruptNonMeleeSpells(false);
                pIsillien->CastSpell(m_creature, SPELL_TAELAN_DEATH, true);
                DoScriptText(SAY_KILL_TAELAN_1, pIsillien);
                DoScriptText(EMOTE_ISILLIEN_LAUGH, pIsillien);
                DoScriptText(SAY_KILL_TAELAN_2, pIsillien);
                m_bTaelanDead = true;
            }
            else
                debug_log("SD0: Can't select Isillien in quest \"In Dreams!\" (phase Taelan's death)");
        }
    }

    void DoStartEvent()
    {
        // "NOOOOOOOOOOOO!" Event
        EventPhase = 100;
        EventTimer = 1;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_HealthRegenTimer <= uiDiff)
        {
            if (!m_bTaelanDead && !m_creature->isInCombat())
            {
                m_creature->SetHealth(m_creature->GetHealth() + uint32(m_creature->GetMaxHealth() / 3));
                m_HealthRegenTimer = 2000;
            }
        }
        else
            m_HealthRegenTimer -= uiDiff;

        if (CanWalk)
            npc_escortAI::UpdateAI(uiDiff);

        if (EventTimer)
        {
            if (EventTimer <= uiDiff)
            {
                switch (EventPhase)
                {
                case 1:
                    m_creature->setFaction(FACTION_FRIENDLY);
                    if (Unit* pSummon = m_creature->SummonCreature(NPC_GRAND_INQUISITOR_ISILLIEN, 2689.30f, -1936.92f, 72.13f, 2.10f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                    {
                        isillien = pSummon->GetObjectGuid();
                        pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                    EventTimer = 2000;
                    break;
                case 2:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                        DoScriptText(YELL_ISILLIEN, pIsillien);
                    EventTimer = 2000;
                    break;
                case 3:
                    DoScriptText(YELL_TAELAN, m_creature);
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                        m_creature->SetFacingToObject(pIsillien);
                    EventTimer = 2000;
                    break;
                case 4:
                    DoScriptText(SAY_TAELAN_STRANGER, m_creature);
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                        pIsillien->GetMotionMaster()->MovePoint(NULL, 2677.28f, -1918.11f, 68.15f);
                    if (Creature* pSummon = m_creature->SummonCreature(NPC_CRIMSON_ELITE, 2689.90f, -1937.98f, 72.13f, 2.10f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                    {
                        pTowerCreatures[0] = pSummon->GetObjectGuid();
                        pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pSummon->GetMotionMaster()->MovePoint(NULL, 2673.22f, -1920.70f, 68.21f);
                        CreatureCreatePos pos(pSummon->GetMap(), 2673.22f, -1920.70f, 68.21f, 6.23f);
                        pSummon->SetRespawnCoord(pos);
                    }
                    if (Creature* pSummon = m_creature->SummonCreature(NPC_CRIMSON_ELITE, 2689.90f, -1937.98f, 72.13f, 2.10f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                    {
                        pTowerCreatures[1] = pSummon->GetObjectGuid();
                        pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pSummon->GetMotionMaster()->MovePoint(NULL, 2682.19f, -1914.96f, 68.04f);
                        CreatureCreatePos pos(pSummon->GetMap(), 2682.19f, -1914.96f, 68.04f, 6.23f);
                        pSummon->SetRespawnCoord(pos);
                    }
                    EventTimer = 8500;
                    break;
                case 5:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    {
                        m_creature->SetFacingToObject(pIsillien);
                        pIsillien->SetFacingToObject(m_creature);
                        pIsillien->HandleEmote(EMOTE_ONESHOT_TALK);
                        DoScriptText(SAY_ISILLIEN1, pIsillien);
                    }
                    EventTimer = 6500;
                    break;
                case 6:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                        DoScriptText(SAY_ISILLIEN2, pIsillien);
                    EventTimer = 6500;
                    break;

                case 7:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                        DoScriptText(SAY_ISILLIEN3, pIsillien);
                    EventTimer = 6500;
                    break;
                case 8:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    {
                        DoScriptText(SAY_ISILLIEN4, pIsillien);
                        DoScriptText(SAY_ISILLIEN_ATTACK, pIsillien);
                        pIsillien->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pIsillien->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                        pIsillien->AI()->AttackStart(m_creature);
                        AttackStart(pIsillien);
                        endFight = true;
                    }
                    for (uint8 itr = 0; itr < 2; ++itr)
                    if (Creature* pTemp = m_creature->FindMap()->GetCreature(pTowerCreatures[itr]))
                    {
                        pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        if (!pTemp->getVictim() && pTemp->AI())
                            pTemp->AI()->AttackStart(m_creature);
                    }
                    for (uint8 itr = 2; itr < MAX_CREATURES; ++itr)
                    if (Creature* pSummon = m_creature->SummonCreature(NPC_CRIMSON_ELITE, spawnPosition[itr + 2].m_fX, spawnPosition[itr + 2].m_fY, spawnPosition[itr + 2].m_fZ, 4.06f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                    {
                        pTowerCreatures[itr] = pSummon->GetObjectGuid();
                        if (pSummon->AI())
                            pSummon->AI()->AttackStart(m_creature);
                        switch (itr)
                        {
                        case 2:
                        {
                                  CreatureCreatePos pos(pSummon->GetMap(), 2680.14f, -1905.25f, 66.3613f, 6.23f);
                                  pSummon->SetRespawnCoord(pos);
                                  break;
                        }
                        case 3:
                        {
                                  CreatureCreatePos pos(pSummon->GetMap(), 2674.01f, -1909.43f, 66.1366f, 6.23f);
                                  pSummon->SetRespawnCoord(pos);
                                  break;
                        }
                        case 4:
                        {
                                  CreatureCreatePos pos(pSummon->GetMap(), 2667.25f, -1914.24f, 66.2629f, 3.36f);
                                  pSummon->SetRespawnCoord(pos);
                                  break;
                        }
                        }
                    }
                    EventTimer = 0;
                    break;
                case 9:
                    if (GetPlayerForEscort())
                    {
                        if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                        {
                            DoScriptText(EMOTE_ISILLIEN_FOCUS, pIsillien);
                            pIsillien->SetTargetGuid(GetPlayerForEscort()->GetObjectGuid());
                            pIsillien->HandleEmote(EMOTE_ONESHOT_POINT);
                        }
                    }
                    EventTimer = 4000;
                    break;
                case 10:
                    // Isillien start attacking escorter player.
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    if (pIsillien->isAlive() && GetPlayerForEscort() && GetPlayerForEscort()->isAlive())
                    {
                        CreatureCreatePos pos(pIsillien->GetMap(), 2677.28f, -1918.11f, 68.15f, 2.10f);
                        pIsillien->SetRespawnCoord(pos);
                        pIsillien->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_ATTACKABLE_1);
                        pIsillien->AI()->AttackStart(GetPlayerForEscort());
                    }
                    for (uint8 itr = 0; itr < MAX_CREATURES; ++itr)
                    if (Creature* pTemp = m_creature->FindMap()->GetCreature(pTowerCreatures[itr]))
                    {
                        pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        if (!pTemp->getVictim() && pTemp->AI())
                            pTemp->AI()->AttackStart(GetPlayerForEscort());
                    }
                    EventTimer = 25000;
                    break;
                case 11:
                    if (Creature* pSummon = m_creature->SummonCreature(NPC_TIRION_FORDRING, 2634.07f, -1914.61f, 72.23f, 0.35f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, J_SUMMONED_DESPAWN_TIME))
                    {
                        tirionFordring = pSummon->GetObjectGuid();
                        pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pSummon->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);
                        pSummon->InterruptNonMeleeSpells(false);
                        pSummon->CastSpell(pSummon, SPELL_MOUNT2, true);
                    }
                    EventTimer = 2000;
                    break;
                case 12:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->StopMoving();
                        pTirion->GetMotionMaster()->Clear();
                        pTirion->GetMotionMaster()->MovePoint(NULL, 2652.45f, -1908.32f, 69.64f);
                    }
                    EventTimer = 2000;
                    break;
                case 13:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->StopMoving();
                        if (pTirion->IsMounted())
                        {
                            pTirion->Unmount();
                            pTirion->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
                        }
                        pTirion->SetFacingToObject(m_creature);
                        pTirion->GetMotionMaster()->Clear();
                        pTirion->GetMotionMaster()->MoveChase(m_creature, 3.0f, pTirion->GetOrientation());
                        CreatureCreatePos pos(pTirion->GetMap(), m_creature->GetPositionX() - 2, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f);
                        pTirion->SetRespawnCoord(pos);
                        EventTimer = 2500;
                    }
                    break;
                case 14:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        if (pTirion->IsWithinDist(m_creature, 4.0f))
                        {
                            CreatureCreatePos pos(pTirion->GetMap(), pTirion->GetPositionX(), pTirion->GetPositionY(), pTirion->GetPositionZ(), pTirion->GetOrientation());
                            pTirion->SetRespawnCoord(pos);
                        }
                        else
                        {
                            pTirion->Relocate(m_creature->GetPositionX() - 2, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f);
                            --EventPhase;
                            EventTimer = 1;
                            break;
                        }
                        pTirion->SetFacingToObject(m_creature);
                        pTirion->SetStandState(UNIT_STAND_STATE_KNEEL);
                    }
                    EventTimer = 4000;
                    break;
                case 15:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->SetStandState(UNIT_STAND_STATE_STAND);
                        DoScriptText(SAY_TIRION1, pTirion);
                    }
                    EventTimer = 10000;
                    break;
                case 16:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    {
                        if (pIsillien->isAlive())
                            DoScriptText(SAY_ISILLIEN5, pIsillien);
                        else if (pIsillien->isDead())
                        {
                            EventPhase = 20;
                            EventTimer = 1;
                            break;
                        }
                    }
                    EventTimer = 14000;
                    break;
                case 17:
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        if (pIsillien->isAlive() && pTirion->isAlive())
                        {
                            DoScriptText(SAY_TIRION2, pTirion);
                            pTirion->SetFacingToObject(pIsillien);
                            pTirion->HandleEmote(EMOTE_ONESHOT_POINT);
                        }
                    }
                    EventTimer = 10000;
                    break;
                case 18:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    if (!pTirion->getVictim())
                        DoScriptText(SAY_TIRION3, pTirion);
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    if (pIsillien->isAlive())
                        DoScriptText(SAY_ISILLIEN6, pIsillien);
                    EventTimer = 3500;
                    break;
                case 19:
                    for (uint8 itr = 0; itr < MAX_CREATURES; ++itr)
                    if (Unit* pTownCreature = m_creature->FindMap()->GetCreature(pTowerCreatures[itr]))
                        pTownCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_ATTACKABLE_1);
                    if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_ATTACKABLE_1);
                        pIsillien->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_ATTACKABLE_1);
                        pTirion->AI()->AttackStart(pIsillien);
                        pIsillien->DeleteThreatList();
                        if (pIsillien->AI())
                            pIsillien->AI()->AttackStart(pTirion);

                    }
                    EventTimer = 0;
                    DeathTimer = 2500;
                    break;
                case 20:
                    // Crimson Elites must be defeated
                    for (uint8 itr = 0; itr < MAX_CREATURES; ++itr)
                    if (Creature* pTownCreature = m_creature->FindMap()->GetCreature(pTowerCreatures[itr]))
                    if (pTownCreature->isAlive() && !pTownCreature->getVictim())
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    if (pTirion->isAlive() && pTownCreature->AI())
                        pTownCreature->AI()->AttackStart(pTirion);

                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        if (!pTirion->getVictim())
                        {
                            pTirion->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pTirion->AI()->EnterEvadeMode();
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
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->SetFacingToObject(m_creature);
                        pTirion->SetStandState(UNIT_STAND_STATE_KNEEL);
                        DoScriptText(EMOTE_TIRION_KNEE, pTirion);
                    }
                    EventTimer = 2000;
                    break;
                case 22:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        DoScriptText(SAY_TIRION4, pTirion);
                        pTirion->SetStandState(UNIT_STAND_STATE_STAND);
                        pTirion->HandleEmote(EMOTE_ONESHOT_EXCLAMATION);      // TODO: When core support Excited emote, use it!
                    }
                    EventTimer = 2000;
                    break;
                case 23:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->SetStandState(UNIT_STAND_STATE_KNEEL);
                        DoScriptText(EMOTE_TIRION_HOLD, pTirion);
                    }
                    EventTimer = 4000;
                    break;
                case 24:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                        DoScriptText(SAY_TIRION5, pTirion);
                    EventTimer = 5000;
                    break;
                case 25:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                        DoScriptText(SAY_TIRION6, pTirion);
                    EventTimer = 5000;
                    break;
                case 26:
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                        DoScriptText(SAY_TIRION7, pTirion);
                    EventTimer = 8000;
                    break;
                case 27:
                    if (GetPlayerForEscort())
                        GetPlayerForEscort()->GroupEventHappens(QUEST_IN_DREAMS, m_creature);
                    if (Creature* pTirion = m_creature->FindMap()->GetCreature(tirionFordring))
                    {
                        pTirion->SetStandState(UNIT_STAND_STATE_STAND);
                        DoScriptText(SAY_TIRION8, pTirion);
                        pTirion->HandleEmote(EMOTE_ONESHOT_ROAR);
                    }

                    EventTimer = 45000;
                    break;
                case 28:
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
                {
                            m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);

                            std::list<Creature*> lTempGuardList;
                            GetCreatureListWithEntryInGrid(lTempGuardList, m_creature, NPC_SCARLET_CAVALIER, 10.f);
                            uint8 pGuardNo = 0;
                            for (std::list<Creature*>::iterator itr = lTempGuardList.begin(); itr != lTempGuardList.end(); ++itr)
                            if (*itr)
                            {
                                lTaelansGuards.push_back((*itr)->GetObjectGuid());
                                if ((*itr)->IsWithinLOSInMap(m_creature))
                                {
                                    if ((*itr)->isAlive())
                                    {
                                        ++pGuardNo;
                                        (*itr)->SetFacingToObject(m_creature);
                                        switch (pGuardNo)
                                        {
                                        case 1:
                                            DoScriptText(SAY_GUARD1, (*itr));
                                            break;
                                        case 2:
                                            DoScriptText(SAY_GUARD2, (*itr));
                                            break;
                                        case 3:
                                            DoScriptText(SAY_GUARD3, (*itr));
                                            break;
                                        case 4:
                                            DoScriptText(SAY_GUARD4, (*itr));
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                                debug_log("SD0: Taelan cant find his guards around him.");

                            EventTimer = 3000;
                            break;
                }
                case 101:
                    for (std::list<ObjectGuid>::iterator itr = lTaelansGuards.begin(); itr != lTaelansGuards.end(); ++itr)
                    {
                        if (Creature* pGuard = m_creature->FindMap()->GetCreature((*itr)))
                        if (pGuard->IsWithinLOSInMap(m_creature))
                        {
                            pGuard->GetMotionMaster()->MoveChase(m_creature, 0.25f, 0);
                            pGuard->SetFacingToObject(m_creature);
                        }
                    }
                    EventTimer = 5000;
                    break;
                case 102:
                    DoScriptText(YELL_NOOO, m_creature);
                    m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                    m_creature->HandleEmote(EMOTE_ONESHOT_ROAR);
                    if (!lTaelansGuards.empty())
                    for (std::list<ObjectGuid>::iterator itr = lTaelansGuards.begin(); itr != lTaelansGuards.end(); ++itr)
                    {
                        if (Creature* pGuard = m_creature->FindMap()->GetCreature((*itr)))
                        if (pGuard->IsWithinLOSInMap(m_creature))
                        {
                            if (pGuard->AI())
                                pGuard->AI()->AttackStart(m_creature);
                            if (!m_creature->getVictim() && itr == lTaelansGuards.begin())
                                AttackStart(pGuard);
                        }
                    }
                    DoCastSpellIfCan(m_creature, SPELL_TAELANS_SUFFERING);
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
                if (Creature* pIsillien = m_creature->FindMap()->GetCreature(isillien))
                {
                    if (pIsillien->isDead())
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

        if (m_bTaelanDead)
            return;

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
            m_uiCrusaderStrikeTimer = 8000;
        }
        else
            m_uiCrusaderStrikeTimer -= uiDiff;

        if (m_uiHolyCleaveTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HOLY_CLEAVE);
            m_uiHolyCleaveTimer = 8000;
        }
        else
            m_uiHolyCleaveTimer -= uiDiff;

        if (m_uiHolyLightTimer <= uiDiff)
        {
            Unit* pFriend = DoSelectLowestHpFriendly(15.f, 15000);
            if (pFriend)
                DoCastSpellIfCan(pFriend, SPELL_HOLY_LIGHT, CAST_FORCE_CAST);
            m_uiHolyLightTimer = urand(10000, 15000);
        }
        else
            m_uiHolyLightTimer -= uiDiff;

        if (m_uiHolyStrikeTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HOLY_STRIKE);
            m_uiHolyStrikeTimer = 5000;
        }
        else
            m_uiHolyStrikeTimer -= uiDiff;

        if (m_uiLayOnHandsTimer <= uiDiff)
        {
            if (!endFight && m_creature->GetHealthPercent() <= 15.0f)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_LAY_ON_HANDS) == CAST_OK)
                    m_uiLayOnHandsTimer = 180000;
            }
        }
        else
            m_uiLayOnHandsTimer -= uiDiff;

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
        if (justSpawned)
        {
            m_creature->RemoveAura(SPELL_AURA_REMOVEME, EFFECT_INDEX_0);
            if (m_creature->getFaction() == FACTION_HORDE)
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
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_highlord_taelan_fordring;
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