/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Golemagg
SD%Complete: 75
SDComment: Timers need to be confirmed, Golemagg's Trust need to be checked
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"
#include "../system/ProgressMonitor.h"

enum
{
    SPELL_MAGMASPLASH       = 13879,
    SPELL_PYROBLAST         = 20228,
    SPELL_EARTHQUAKE        = 19798,
    SPELL_ENRAGE            = 19953,
    SPELL_GOLEMAGG_TRUST    = 20553,
	SPELL_THREAT			= 25063,

    // Core Rager
    EMOTE_LOWHP             = -1409002,
    SPELL_MANGLE            = 19820
};

struct MANGOS_DLL_DECL boss_golemaggAI : public ScriptedAI
{
    boss_golemaggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 m_uiPyroblastTimer;
    uint32 m_uiEarthquakeTimer;
    uint32 m_uiBuffTimer;
	uint32 m_uiMagmaSplashTimer;
    bool m_bEnraged;

    void Reset()
    {
        m_uiPyroblastTimer		= 5000;
        m_uiEarthquakeTimer		= 10000;
        m_uiBuffTimer			= 8000;
		m_uiMagmaSplashTimer	= 5000;
        m_bEnraged				= false;

		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
    }
    
    void Aggro(Unit*)
    {
		if (pInstance)
			pInstance->SetData(TYPE_GOLEMAGG, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_GOLEMAGG, NOT_STARTED);
	}
     
    void JustDied(Unit* pPlayer)
    {
		if (pInstance)
			pInstance->SetData(TYPE_GOLEMAGG, DONE);

		pPlayer->RemoveAura(SPELL_THREAT, SpellEffectIndex::EFFECT_INDEX_0);
		pPlayer->RemoveAura(SPELL_THREAT, SpellEffectIndex::EFFECT_INDEX_1);
		pPlayer->RemoveAura(SPELL_THREAT, SpellEffectIndex::EFFECT_INDEX_2);

		using namespace ProgressMonitor;
		if (pPlayer->GetTypeId() == TYPEID_PLAYER)
			Track(static_cast<Player*>(pPlayer), Raids::MoltenCore, Bosses::Golemagg);
    }

	int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
        // MagmaSplash
        if (m_uiMagmaSplashTimer < uiDiff)
        {
			if(m_creature->HasAura(SPELL_MAGMASPLASH) == false)
				DoCastSpellIfCan(m_creature, SPELL_MAGMASPLASH);
            m_uiMagmaSplashTimer = 5000;
        }
        else
            m_uiMagmaSplashTimer -= uiDiff;
        
		// Pyroblast
        if (m_uiPyroblastTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_PYROBLAST);

            m_uiPyroblastTimer = random(7000, 10000);
        }
        else
            m_uiPyroblastTimer -= uiDiff;

        // Enrage && Threat Spell on Raid
        if (!m_bEnraged && m_creature->GetHealthPercent() < 20.0f)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);

            Map::PlayerList const& players = pInstance->instance->GetPlayers();
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
					pPlayer->CastSpell(pPlayer, SPELL_THREAT, true);
            }

			if (Unit* pTank = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				pTank->RemoveAura(SPELL_THREAT, SpellEffectIndex::EFFECT_INDEX_0);
				pTank->RemoveAura(SPELL_THREAT, SpellEffectIndex::EFFECT_INDEX_1);
				pTank->RemoveAura(SPELL_THREAT, SpellEffectIndex::EFFECT_INDEX_2);
			}
            m_bEnraged = true;
        }

        // Earthquake
        if (m_bEnraged)
        {
            if (m_uiEarthquakeTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_EARTHQUAKE);
                m_uiEarthquakeTimer = random(5000, 10000);
            }
            else
                m_uiEarthquakeTimer -= uiDiff;
        }
		
        // Golemagg Trust
        if (m_uiBuffTimer < uiDiff)
        {
            Creature* target = NULL;
            std::list<Creature*> pList = DoFindFriendlyMissingBuff(30.0f, SPELL_GOLEMAGG_TRUST);
            if (!pList.empty())
            {
                std::list<Creature*>::iterator i = pList.begin();
                advance(i, (rand()%pList.size()));
                target = (*i);
            }

            if (target)
                DoCastSpellIfCan(target, SPELL_GOLEMAGG_TRUST);

            m_uiBuffTimer = 5000;
        } else m_uiBuffTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_golemagg(Creature* pCreature)
{
    return new boss_golemaggAI(pCreature);
}

struct MANGOS_DLL_DECL mob_core_ragerAI : public ScriptedAI
{
    mob_core_ragerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 m_uiMangleTimer;

    void Reset()
    {
        m_uiMangleTimer = 10000;
    }

	int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void DamageTaken(Unit*, uint32& uiDamage)
    { 			
        if (m_creature->GetHealthPercent() < 50.0f)
        {
            if (pInstance)
            {
                if (Creature* pGolemagg = pInstance->instance->GetCreature(pInstance->GetData64(DATA_GOLEMAGG)))
                {
                    if (pGolemagg->isAlive())
                    {
                        DoScriptText(EMOTE_LOWHP, m_creature);
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                    }
                    else
					{
                        uiDamage = m_creature->GetHealth();
					}
				}
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (Creature* pGolemagg = pInstance->instance->GetCreature(pInstance->GetData64(DATA_GOLEMAGG)))
        {
			if (pGolemagg->isDead())
			{
				m_creature->setFaction(734);
				m_creature->AI()->EnterEvadeMode();
				m_creature->SetVisibility(VISIBILITY_OFF);
		    }
		}

        // Mangle
        if (m_uiMangleTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MANGLE);
            m_uiMangleTimer = random(5000, 10000);
        }
        else
            m_uiMangleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_core_rager(Creature* pCreature)
{
    return new mob_core_ragerAI(pCreature);
}

void AddSC_boss_golemagg()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_golemagg";
    newscript->GetAI = &GetAI_boss_golemagg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_core_rager";
    newscript->GetAI = &GetAI_mob_core_rager;
    newscript->RegisterSelf();
}
