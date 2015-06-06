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
SDName: Ruins of Ahn'Qiraj
SD%Complete: 40
SDComment:
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

/* ContentData
mob_anubisath_guardian
mob_hive_zara_stinger
EndContentData */

#include "precompiled.h"

/*######
## mob_anubisath_guardian
######*/
enum
{
    SPELL_METEOR                 = 24340,
    SPELL_PLAGUE                 = 22997,
    SPELL_SHADOW_STORM           = 26546,
    SPELL_THUNDER_CLAP           = 26554,
    SPELL_REFLECT_ARFR           = 13022,
    SPELL_REFLECT_FSSH           = 19595,
    SPELL_ENRAGE                 = 8599,
    SPELL_EXPLODE                = 25698,

    SPELL_SUMMON_ANUB_SWARMGUARD = 17430,
    SPELL_SUMMON_ANUB_WARRIOR    = 17431,

    EMOTE_FRENZY                 = -1000002,

    NPC_ANUB_WARRIOR             = 15537,
    NPC_ANUB_SWARM               = 15538
};


/*######
## mob_hive_zara_stinger
######*/
enum
{
    SPELL_POISION                = 25187,
    SPELL_CHARGE                 = 25190,
};

struct MANGOS_DLL_DECL mob_hive_zara_stinger : public ScriptedAI
{
    mob_hive_zara_stinger(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_chargeTimer;

    void Reset() 
    { 
        m_chargeTimer = urand(10000, 14000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_chargeTimer <= uiDiff)
        {
            Unit* pTarget = nullptr;

            
            Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            {
                if (itr->getSource() && itr->getSource()->HasAura(SPELL_POISION) && !m_creature->CanReachWithMeleeAttack(itr->getSource()))
                    pTarget = itr->getSource();
            }

            if (!pTarget)
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (SpellEntry *)0, SELECT_FLAG_NOT_IN_MELEE_RANGE);

            if (pTarget)
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CHARGE) == CAST_OK)
                    m_chargeTimer = urand(10000, 14000);
            }
        }
        else
            m_chargeTimer -= uiDiff;


        DoMeleeAttackIfReady();
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) 
    {
        if (!uiDamage || !pDoneTo->HasAura(SPELL_POISION))
            return;

        uiDamage *= 2;
    }
};

CreatureAI* GetAI_mob_hive_zara_stinger(Creature* pCreature)
{
    return new mob_hive_zara_stinger(pCreature);
}

struct MANGOS_DLL_DECL mob_anubisath_guardianAI : public ScriptedAI
{
    mob_anubisath_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSpell1;
    uint32 m_uiSpell2;
    uint32 m_uiSpell3;
    uint32 m_uiSpell4;
    uint32 m_uiSpell5;

    uint32 m_uiSpell1Timer;
    uint32 m_uiSpell2Timer;
    uint32 m_uiSpell5Timer;

    uint8 m_uiSummonCount;

    bool m_bIsEnraged;

    void Reset()
    {
        m_uiSpell1 = urand(0,1) ? SPELL_METEOR : SPELL_PLAGUE;
        m_uiSpell2 = urand(0,1) ? SPELL_SHADOW_STORM : SPELL_THUNDER_CLAP;
        m_uiSpell3 = urand(0,1) ? SPELL_REFLECT_ARFR : SPELL_REFLECT_FSSH;
        m_uiSpell4 = urand(0,1) ? SPELL_ENRAGE : SPELL_EXPLODE;
        m_uiSpell5 = urand(0,1) ? SPELL_SUMMON_ANUB_SWARMGUARD : SPELL_SUMMON_ANUB_WARRIOR;

        m_uiSpell1Timer = 10000;
        m_uiSpell2Timer = 20000;
        m_uiSpell5Timer = 10000;
        m_uiSummonCount = 0;
        m_bIsEnraged    = false;
    }

    void Aggro(Unit*)
    {
        // spell reflection
        DoCastSpellIfCan(m_creature, m_uiSpell3);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature->getVictim());
        ++m_uiSummonCount;
    }

    void SummonedCreatureDespawn(Creature*)
    {
        --m_uiSummonCount;
    }

    void DamageTaken(Unit*, uint32&)
    {
        // when we reach 10% of HP explode or enrage
        if (!m_bIsEnraged && m_creature->GetHealthPercent() < 10.0f)
        {
            if (m_uiSpell4 == SPELL_ENRAGE)
            {
                DoScriptText(EMOTE_FRENZY, m_creature);
                DoCastSpellIfCan(m_creature, m_uiSpell4);
                m_bIsEnraged = true;
            }
            else
                DoCastSpellIfCan(m_creature->getVictim(), m_uiSpell4);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Meteor or Plague
        if (m_uiSpell1Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_uiSpell1);
            m_uiSpell1Timer = 15000;
        }
        else
            m_uiSpell1Timer -= uiDiff;

        // Shadow Storm or Thunder Clap
        if (m_uiSpell2Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_uiSpell2);
            m_uiSpell2Timer = 15000;
        }
        else
            m_uiSpell2Timer -= uiDiff;

        // summon Anubisath Swarmguard or Anubisath Warrior
        if (m_uiSpell5Timer < uiDiff)
        {
            // change for summon spell
            if (m_uiSummonCount < 4)
                DoCastSpellIfCan(m_creature->getVictim(), m_uiSpell5);

            m_uiSpell5Timer = 15000;
        }
        else
            m_uiSpell5Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_anubisath_guardian(Creature* pCreature)
{
    return new mob_anubisath_guardianAI(pCreature);
}

/*######
## npc_obsidian_destroyer
######*/
enum
{
    SPELL_PURGE = 25756,
    SPELL_DRAIN_MANA = 25671,
};

struct npc_obsidian_destroyerAI : public ScriptedAI
{
    npc_obsidian_destroyerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uManaDrainTimer;

    void Reset()
    {
        m_creature->SetMaxPower(POWER_MANA, 0);
        m_uManaDrainTimer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetMaxPower(POWER_MANA, m_creature->GetCreatureInfo()->maxmana);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetPower(POWER_MANA) == m_creature->GetMaxPower(POWER_MANA))
            if (DoCastSpellIfCan(m_creature, SPELL_PURGE, CAST_TRIGGERED) == CAST_OK)
                m_creature->SetPower(POWER_MANA, 0);

        if (m_uManaDrainTimer <= diff)
        {
            m_uManaDrainTimer = 5000;

            std::vector<Unit*> lTargets;
            ThreatList const& threatlist = m_creature->getThreatManager().getThreatList();
            if (threatlist.empty())
                return;

            for (ThreatList::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
            {
                Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

                if (pUnit && pUnit->isAlive() && pUnit->GetPower(POWER_MANA))
                    lTargets.push_back(pUnit);
            }

            std::shuffle(lTargets.begin(), lTargets.end(), std::default_random_engine(time_t(NULL)));

            for (int i = 0; i < 6; i++)
            {
                if (i >= lTargets.size())
                    return;

                DoCastSpellIfCan(lTargets.at(i), SPELL_DRAIN_MANA, CAST_TRIGGERED);
            }
        }
        else
            m_uManaDrainTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_obsidian_destroyer(Creature* pCreature)
{
    return new npc_obsidian_destroyerAI(pCreature);
}

//Spell 25185 needs to be scripted
//15325 needs to cast 25187
//15335 needs to cast 25371 -> Fixme: trigger should heal caster & target should be teleported instead of caster
//15320 needs to cast 25497
//15336 needs to cast 25645 and 25654
//15527 needs to cast 25679
//15355 needs to cast 25698
//15338 need to cast 25756

void AddSC_ruins_of_ahnqiraj()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "mob_anubisath_guardian";
    newscript->GetAI = &GetAI_mob_anubisath_guardian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_hive_zara_stinger";
    newscript->GetAI = &GetAI_mob_hive_zara_stinger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_obsidian_destroyer";
    newscript->GetAI = &GetAI_npc_obsidian_destroyer;
    newscript->RegisterSelf();
}
