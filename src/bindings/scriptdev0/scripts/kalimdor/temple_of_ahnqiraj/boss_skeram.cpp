/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Skeram
SD%Complete: 90
SDComment: Timers
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

enum
{
    SAY_AGGRO1                  = -1531000,
    SAY_AGGRO2                  = -1531001,
    SAY_AGGRO3                  = -1531002,
    SAY_SLAY1                   = -1531003,
    SAY_SLAY2                   = -1531004,
    SAY_SLAY3                   = -1531005,
    SAY_SPLIT                   = -1531006,
    SAY_DEATH                   = -1531007,

    SPELL_ARCANE_EXPLOSION      = 26192,
    SPELL_EARTH_SHOCK           = 26194,
    SPELL_TRUE_FULFILLMENT      = 785,
    SPELL_TELEPORT_1            = 4801,
    SPELL_TELEPORT_2            = 8195,
    SPELL_TELEPORT_3            = 20449,
    SPELL_INITIALIZE_IMAGE      = 3730,
    SPELL_SUMMON_IMAGES         = 747,
};

struct MANGOS_DLL_DECL boss_skeramAI : public ScriptedAI
{
    boss_skeramAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();

        // Check this for images, because the Initialize spell hits the target only after aggro
        if (m_creature->IsTemporarySummon())
            m_bIsImage = true;
        else
            m_bIsImage = false;
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiArcaneExplosionTimer;
    uint32 m_uiFullFillmentTimer;
    uint32 m_uiBlinkTimer;
    uint32 m_uiEarthShockTimer;

    float m_fHpCheck;

    bool m_bIsImage;
    bool m_bImagePosition;

    void Reset()
    {
        m_uiArcaneExplosionTimer = 15000;
        m_uiFullFillmentTimer    = 15000;
        m_uiBlinkTimer           = urand(30000, 45000);
        m_uiEarthShockTimer      = 1000;

        m_fHpCheck               = 75.0f;

        if (m_creature->GetVisibility() != VISIBILITY_ON)
            m_creature->SetVisibility(VISIBILITY_ON);
    }

    void KilledUnit(Unit* /*pVictim*/)
    {
        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* /*pKiller*/)
    {
        if (!m_bIsImage)
        {
            DoScriptText(SAY_DEATH, m_creature);

            if (m_pInstance)
                m_pInstance->SetData(TYPE_SKERAM, DONE);
            
            Map *m = m_creature->GetMap();
            if (m->IsDungeon())
                if (Player* plr = m_pInstance->GetPlayerInMap(false, false))
                    ((DungeonMap *)m)->PermBindAllPlayers(plr);
        }
        // Else despawn to avoid looting
        else
            m_creature->ForcedDespawn();
    }

    void Aggro(Unit*)
    {
        if (m_bIsImage)
            return;

        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SKERAM, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SKERAM, FAIL);

        if (m_bIsImage)
            m_creature->ForcedDespawn();
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);

        DoCastSpellIfCan(pSummoned, SPELL_INITIALIZE_IMAGE, CAST_TRIGGERED);

        if (m_bImagePosition)
        {
            pSummoned->NearTeleportTo(-8330.28f, 2123.08f, 133.06f, 0.4f);
            m_bImagePosition = false;
        }
        else
        {
            pSummoned->NearTeleportTo(-8306.22f, 2062.50f, 133.07f, 0.4f);
            m_bImagePosition = true;
        }
    }

    virtual void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell) 
    {
        if (pSpell->Id == 785)
        {
            pTarget->CastSpell(pTarget, 2313, true);
            pTarget->CastSpell(pTarget, 26525, true);
            pTarget->CastSpell(pTarget, 26526, true);
        }
    }

    // Wrapper to handle the image version initialize
    void DoInitializeImage()
    {
        if (!m_pInstance)
            return;

        // Initialize the health of the clone
        if (Creature* pProphet = m_pInstance->GetSingleCreatureFromStorage(NPC_SKERAM))
        {
            float fHealthPct = pProphet->GetHealthPercent();

            uint16 factor = 8;

            // The max health depends on the split phase. It's percent * original boss health
            if (fHealthPct < 25.0f)
                factor = 2;
            else if (fHealthPct < 50.0f)
                factor = 5;
            else
                factor = 10;

            m_creature->SetHealthPercent(pProphet->GetHealthPercent());
            m_creature->SetResilience(pProphet->GetResilience() * factor);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // ArcaneExplosion_Timer
        if (m_uiArcaneExplosionTimer < uiDiff)
        {
            int meleeCnt = 0;
            Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();

            for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            {
                Player* pPlayer = itr->getSource();
                if (pPlayer && pPlayer->CanReachWithMeleeAttack(m_creature))
                    meleeCnt++;
                if (meleeCnt >= 5)
                    break;
            }
            if (meleeCnt < 5)
                m_uiArcaneExplosionTimer = urand(3000, 6000);
            else if (DoCastSpellIfCan(m_creature, SPELL_ARCANE_EXPLOSION) == CAST_OK)
                m_uiArcaneExplosionTimer = 6000;
        }
        else
            m_uiArcaneExplosionTimer -= uiDiff;

        // True Fullfilment
        if (m_uiFullFillmentTimer < uiDiff)
        {
            Unit* pTarget = nullptr;
            float dist = 10000.f;

            Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();

            // Select nearst player
            for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            {
                Player* pPlayer = itr->getSource();
                if (pPlayer && dist > pPlayer->GetDistance(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()))
                {
                    dist = pPlayer->GetDistance(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                    pTarget = pPlayer;
                }
            }

            if (pTarget)
            {
                if (DoCastSpellIfCan(pTarget, SPELL_TRUE_FULFILLMENT) == CAST_OK)
                    m_uiFullFillmentTimer = urand(20000, 30000);
            }
        }
        else
            m_uiFullFillmentTimer -= uiDiff;

        // Blink_Timer
        if (m_uiBlinkTimer < uiDiff)
        {
            switch (urand(0, 2))
            {
                case 0: DoCastSpellIfCan(m_creature, SPELL_TELEPORT_1); break;
                case 1: DoCastSpellIfCan(m_creature, SPELL_TELEPORT_2); break;
                case 2: DoCastSpellIfCan(m_creature, SPELL_TELEPORT_3); break;
            }

            DoResetThreat();
            if (m_creature->GetVisibility() != VISIBILITY_ON)
                m_creature->SetVisibility(VISIBILITY_ON);

            m_uiBlinkTimer = urand(10000, 30000);
            m_uiEarthShockTimer = 1000;
        }
        else
            m_uiBlinkTimer -= uiDiff;

        // Summon images at 75%, 50% and 25%
        if (!m_bIsImage && m_creature->GetHealthPercent() < m_fHpCheck)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_IMAGES) == CAST_OK)
            {
                DoScriptText(SAY_SPLIT, m_creature);
                m_fHpCheck -= 25.0f;
                // Teleport shortly after the images are summoned and set invisible to clear the selection (Workaround alert!!!)
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->RemoveAllAurasOnEvade();
                m_uiBlinkTimer = 2000;
            }
        }

        // If we are within range melee the target
        if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            DoMeleeAttackIfReady();
            m_uiEarthShockTimer = 0;
        }
        else
        {
            if (m_uiEarthShockTimer <= uiDiff)
            {
                m_uiEarthShockTimer = 1000;
                if (!m_creature->IsNonMeleeSpellCasted(false))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_EARTH_SHOCK);
                }
            }
                m_uiEarthShockTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_skeram(Creature* pCreature)
{
    return new boss_skeramAI(pCreature);
}

bool EffectDummyCreature_prophet_skeram(Unit*, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    // always check spellid and effectindex
    if (uiSpellId == SPELL_INITIALIZE_IMAGE && uiEffIndex == EFFECT_INDEX_0)
    {
        // check for target == caster first
        if (instance_temple_of_ahnqiraj* pInstance = (instance_temple_of_ahnqiraj*)pCreatureTarget->GetInstanceData())
        {
            if (Creature* pProphet = pInstance->GetSingleCreatureFromStorage(NPC_SKERAM))
            {
                if (pProphet == pCreatureTarget)
                    return false;
            }
        }

        if (boss_skeramAI* pSkeramAI = dynamic_cast<boss_skeramAI*>(pCreatureTarget->AI()))
            pSkeramAI->DoInitializeImage();
    }

    return false;
}

void AddSC_boss_skeram()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_skeram";
    pNewScript->GetAI = &GetAI_boss_skeram;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_prophet_skeram;
    pNewScript->RegisterSelf();
}