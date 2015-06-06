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

ScriptName: Scholomance Instanz
Instanz: Scholomance
*/

#include "precompiled.h"
#include "scholomance.h"

/*######
## go_iron_gate
######*/

bool GOUse_go_iron_gate(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->HasStealthAura())
        pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

    if (pPlayer->HasInvisibilityAura())
        pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOD_INVISIBILITY);

    return false;
}

/*######
## go_fackel
######*/

bool GOHello_fackel(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if (pInstance)
    {
        pInstance->SetData(TYPE_FACKEL, DONE);

    }
    return false;
}


/*######
## mob_scholomance_student
######*/

enum eScholomanceStudent
{
    //SPELL_ALEXIS_GAMBIT                         = 17045,
    //SPELL_TRANSFORM_VIEWING_ROOM_STUDENTS       = 18110,
    //SPELL_VIEWING_ROOM_STUDENT_TRANSFORM_EFFECT = 18115,
    MODELID_SKELETAL_SCHOLOMANCE_STUDENT        = 9785,
};

struct MANGOS_DLL_DECL mob_scholomance_studentAI : public ScriptedAI
{
    mob_scholomance_studentAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 m_uiRandomMovementTimer;

    void Reset()
    {
        m_uiRandomMovementTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            // Random Movement
            if (m_creature->GetDisplayId() != MODELID_SKELETAL_SCHOLOMANCE_STUDENT)
                return;

            if (m_uiRandomMovementTimer <= uiDiff)
            {
				//m_creature->GetMotionMaster()->MoveRandomAroundPoint();
                m_uiRandomMovementTimer = urand(2500, 4500);
            }
            else
                m_uiRandomMovementTimer -= uiDiff;
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_scholomance_student(Creature* pCreature)
{
    return new mob_scholomance_studentAI(pCreature);
}

/*######
## mob_reanimated_corpse
######*/

enum eReanimatedCorpse
{
    SPELL_DARK_PLAGUE = 18270,
    REVIVE_TIMER      = 15000
};

struct MANGOS_DLL_DECL mob_reanimated_corpseAI : public ScriptedAI
{
    mob_reanimated_corpseAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 m_uiDarkPlague_Timer;
    uint32 m_uiRisen_Timer;
    bool CanReviveStart;
    bool WasRevived;

    void Reset()
    {
        m_uiDarkPlague_Timer	= urand(3500, 15000);
        m_uiRisen_Timer			= REVIVE_TIMER;
        CanReviveStart			= false;
        WasRevived				= false;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!WasRevived && uiDamage > m_creature->GetHealth())
        {
            m_creature->AttackStop();
            m_creature->StopMoving();
            m_creature->addUnitState(UNIT_STAT_CAN_NOT_MOVE);

            if (m_creature->getStandState() != UNIT_STAND_STATE_DEAD)
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

            CanReviveStart = true;
            m_creature->SetHealth(1);
            uiDamage = 0;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CanReviveStart && !WasRevived)
        {
            if (m_uiRisen_Timer <= uiDiff)
            {
                CanReviveStart = false;
                WasRevived = true;
                m_creature->clearUnitState(UNIT_STAT_CAN_NOT_MOVE);
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                m_creature->SetHealth(m_creature->GetMaxHealth());
            }
            else
                m_uiRisen_Timer -= uiDiff;
        }

        // Dark Plague
        if (!CanReviveStart && m_uiDarkPlague_Timer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARK_PLAGUE);
            m_uiDarkPlague_Timer = urand(10000, 18000);
        }
        else
            m_uiDarkPlague_Timer -= uiDiff;

        if (!CanReviveStart)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_reanimated_corpse(Creature* pCreature)
{
    return new mob_reanimated_corpseAI(pCreature);
}

/*######
## mob_scholomance_dark_summoner
######*/

enum eScholomanceDarkSummoner
{
    SPELL_CURSE_OF_BLOOD		= 12279,
    SPELL_DRAIN_LIFE			= 17620,
    SPELL_SUMMON_RISEN_LACKEY	= 17618,
    
    SAY1   = -1289002,
    SAY2   = -1289003,
    SAY3   = -1289004
};

struct MANGOS_DLL_DECL mob_scholomance_dark_summonerAI : public ScriptedAI
{
    mob_scholomance_dark_summonerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 m_uiCurseOfBlood_Timer;
    uint32 m_uiDrainLife_Timer;
    uint32 m_uiSummonRisenLackey_Timer;

    void Reset()
    {
        m_uiCurseOfBlood_Timer		= 1000;
        m_uiDrainLife_Timer			= urand(12500, 17000);
        m_uiSummonRisenLackey_Timer = urand(2000, 3000);
    }

    void Aggro(Unit* /*pWho*/)
    {
        switch(urand(1,3))
        {
            case 1: DoScriptText(SAY1, m_creature); break;
            case 2: DoScriptText(SAY2, m_creature); break;
            case 3: DoScriptText(SAY3, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Curse Of Blood
        if (m_uiCurseOfBlood_Timer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CURSE_OF_BLOOD);
            m_uiCurseOfBlood_Timer = urand(2000, 4000);
        }
        else
            m_uiCurseOfBlood_Timer -= uiDiff;

        // Drain Life
        if (m_uiDrainLife_Timer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DRAIN_LIFE);
            m_uiDrainLife_Timer = urand(15000, 30000);
        }
        else
            m_uiDrainLife_Timer -= uiDiff;

        // Summon Risen Lackey
        if (m_uiSummonRisenLackey_Timer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_RISEN_LACKEY);
            m_uiSummonRisenLackey_Timer = 3000;
        }
        else
            m_uiSummonRisenLackey_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_scholomance_dark_summoner(Creature* pCreature)
{
    return new mob_scholomance_dark_summonerAI(pCreature);
}

/*######
## mob_unstable_corpse
######*/

#define SPELL_DISEASE_BURST 17689

struct MANGOS_DLL_DECL mob_unstable_corpseAI : public ScriptedAI
{
    mob_unstable_corpseAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiKill_Timer;

    void Reset()
    {
        m_uiKill_Timer = 0;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_uiKill_Timer && uiDamage >= m_creature->GetHealth())
        {
            DoCastSpellIfCan(m_creature, SPELL_DISEASE_BURST);
            m_uiKill_Timer = 150;
            uiDamage = 0;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiKill_Timer)
        {
            if (m_uiKill_Timer <= uiDiff)
            {
                m_creature->SetHealth(0);
                m_creature->SetDeathState(JUST_DIED);
            }
            else
                m_uiKill_Timer -= uiDiff;
        }

        if (!m_uiKill_Timer)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_unstable_corpse(Creature* pCreature)
{
    return new mob_unstable_corpseAI(pCreature);
}

/*######
## mob_dark_shade
######*/

#define SPELL_PHYSICAL_IMMUNITY 17674

struct MANGOS_DLL_DECL mob_dark_shadeAI : public ScriptedAI
{
    mob_dark_shadeAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

    uint32 m_uiCheckImmunity_Timer;

    void Reset()
    {
        SetPhysicalImmunity();
        m_uiCheckImmunity_Timer = 2000;
    }

    void SetPhysicalImmunity()
    {
        if (!m_creature->HasAura(SPELL_PHYSICAL_IMMUNITY))
            DoCastSpellIfCan(m_creature, SPELL_PHYSICAL_IMMUNITY, CAST_FORCE_TARGET_SELF);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCheckImmunity_Timer <= uiDiff)
        {
            SetPhysicalImmunity();
            m_uiCheckImmunity_Timer = 2000;
        }
        else
            m_uiCheckImmunity_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_dark_shade(Creature* pCreature)
{
    return new mob_dark_shadeAI(pCreature);
}

void AddSC_scholomance()
{
    Script *pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "scholomance_fackel";
    pNewscript->pGOUse = &GOHello_fackel;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "go_iron_gate";
    pNewscript->pGOUse = &GOUse_go_iron_gate;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_scholomance_student";
    pNewscript->GetAI = &GetAI_mob_scholomance_student;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_reanimated_corpse";
    pNewscript->GetAI = &GetAI_mob_reanimated_corpse;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_scholomance_dark_summoner";
    pNewscript->GetAI = &GetAI_mob_scholomance_dark_summoner;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_unstable_corpse";
    pNewscript->GetAI = &GetAI_mob_unstable_corpse;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_dark_shade";
    pNewscript->GetAI = &GetAI_mob_dark_shade;
    pNewscript->RegisterSelf();
}
