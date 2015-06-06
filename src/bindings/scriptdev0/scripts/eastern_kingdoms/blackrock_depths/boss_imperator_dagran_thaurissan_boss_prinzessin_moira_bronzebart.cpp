/*
#################################################################################
#   ____  _ _   _        ____                 _                          _		#
#  / ___|(_) \ | |      / ___| __ _ _ __ ___ (_)_ __   __ _   _ __   ___| |_	#
#  \___ \| |  \| |_____| |  _ / _` | '_ ` _ \| | '_ \ / _` | | '_ \ / _ \ __|	#
#   ___) | | |\  |_____| |_| | (_| | | | | | | | | | | (_| |_| | | |  __/ |_	#
#  |____/|_|_| \_|      \____|\__,_|_| |_| |_|_|_| |_|\__, (_)_| |_|\___|\__|	#
#                                                     |___/						#
#																				#
#													 Schwarzfelstiefen			#
#												   			...					#
#################################################################################

ScriptName:
Instanz: Schwarzfelstiefen
Staus in %: 100%
Abgeschlossen?: Ja

*/
/* Imperator Dagran Thaurissan */

#include "precompiled.h"
#include "blackrock_depths.h"

#define  FACTION_NEUTRAL	734


#define SPELL_HAND_VON_THAURISSAN		17492
#define SPELL_AVATAR_DER_FLAMME			15636

#define SAY_AGGRO                       "Kommt und kaempft um den Thron!"
#define SAY_SLAY                        "Gelobt sei unser Koenig!"

#define SPELL_HEILEN				15586
#define SPELL_ERNEUERUNG			8362
#define SPELL_GEDANKENSCHLAG        15587
#define SPELL_SCHATTENWORT_SCHMERZ  15654
#define SPELL_SCHATTENBLITZ         15537
#define SPELL_OPEN_PORTAL           13912



int p=0;

struct MANGOS_DLL_DECL imperator_dagran_thaurissanAI : public ScriptedAI
{
    imperator_dagran_thaurissanAI(Creature *c) : ScriptedAI(c)
    {
        m_pInstance = (ScriptedInstance*)c->GetInstanceData();
    }
    ScriptedInstance* m_pInstance;

    uint32 hand_von_thaurissan_timer;
    uint32 avatar_der_flame_timer;

    Creature* prinzessin;

    //uint32 Counter;

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void Reset()
    {
        hand_von_thaurissan_timer = 4000;
        avatar_der_flame_timer = 10000;

    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
        m_creature->CallForHelp(VISIBLE_RANGE);
    }

    void KilledUnit(Unit* victim)
    {
        m_creature->MonsterYell(SAY_SLAY, LANG_UNIVERSAL, NULL);
    }

    void JustDied(Unit* Victim)
    {

        if (!m_pInstance)
            return;

        if (Creature* pPrincess = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_PRINCESS)))
        {
            if (pPrincess->isAlive())
            {
                pPrincess->setFaction(FACTION_NEUTRAL);
                pPrincess->AI()->EnterEvadeMode();
            }
        }



        //prinzessin = Unit::GetUnit(m_creature, m_pInstance->GetData64(DATA_PRINCESS));
        //prinzessin = Creature::GetUnit(m_creature, m_pInstance->GetData64(DATA_PRINCESS));
        //prinzessin = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_PRINCESS));
        //if(prinzessin)
        //{
        //	prinzessin->setFaction(734);
        //prinzessin->CombatStop(false);
        //prinzessin->CombatStop();

        //prinzessin->AttackStop(false);
        //}


    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()  )
        {
            Reset();
            return;
        }

        if (hand_von_thaurissan_timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            if (target) DoCastSpellIfCan(target,SPELL_HAND_VON_THAURISSAN);

            hand_von_thaurissan_timer = 5000;

        } else hand_von_thaurissan_timer -= diff;


        if (avatar_der_flame_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_AVATAR_DER_FLAMME);
            avatar_der_flame_timer = 14000;
        } else avatar_der_flame_timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_imperator_dagran_thaurissan(Creature *_Creature)
{
    return new imperator_dagran_thaurissanAI (_Creature);
}



struct MANGOS_DLL_DECL prinzessin_moira_bronzebeardAI : public ScriptedAI
{
    prinzessin_moira_bronzebeardAI(Creature *c) : ScriptedAI(c)
    {
        m_pInstance = (ScriptedInstance*)c->GetInstanceData();
    }
    ScriptedInstance* m_pInstance;

    uint32 heilen_timer;
    uint32 gedankenschlag_timer;
    uint32 schattenwort_schmerz_timer;
    uint32 erneuerung_timer;
    uint32 schattenblitz_timer;
    Unit* target;
    Creature* m_imperator;

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void Reset()
    {
        target = NULL;
        heilen_timer = 6000;
        gedankenschlag_timer = 10000;
        schattenwort_schmerz_timer = 2000;
        erneuerung_timer = 4000;
        schattenblitz_timer = 3000;
    }

    void Aggro(Unit *who)
    {
        //m_imperator = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_EMPEROR)); //Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_EMPEROR));

        if (m_creature->Attack(who, false))
        {
            m_creature->AddThreat(who);
            m_creature->SetInCombatWith(who);
            who->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(who, 25.0f);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (Creature* pEmperor = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_EMPEROR)))
            {
                // if evade, then check if he is alive. If not, start make portal
                if (!pEmperor->isAlive())
                    m_creature->CastSpell(m_creature, SPELL_OPEN_PORTAL, false);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        m_creature->getFaction();
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()  )
            return;


        if (gedankenschlag_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_GEDANKENSCHLAG);
            gedankenschlag_timer = random(8000,15000);
        } else gedankenschlag_timer -= diff;


        if (schattenwort_schmerz_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_SCHATTENWORT_SCHMERZ);
            schattenwort_schmerz_timer = random(8000,18000);
        } else schattenwort_schmerz_timer -= diff;

        if (schattenblitz_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_SCHATTENBLITZ);
            schattenblitz_timer = random(10000,14000);
        } else schattenblitz_timer -= diff;


        if (heilen_timer <diff)
        {



            switch (rand()%2)
            {

            case 0:
                if (Creature* pEmperor = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_EMPEROR)))
                {
                    if (pEmperor->isAlive() && pEmperor->GetHealthPercent() != 100.0f)
                        DoCastSpellIfCan(pEmperor,SPELL_HEILEN);
                }
                break;

            case 1:
                DoCastSpellIfCan(m_creature,SPELL_HEILEN);
                break;
            }
            heilen_timer = random(3000,5000);
        } else heilen_timer -= diff;

        if (erneuerung_timer <diff)
        {
            switch (rand()%2)
            {

            case 0:
                if (Creature* pEmperor = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_EMPEROR)))
                {
                    if (pEmperor->isAlive() && pEmperor->GetHealthPercent() != 100.0f)
                        DoCastSpellIfCan(pEmperor,SPELL_ERNEUERUNG);
                }
                break;

            case 1:
                DoCastSpellIfCan(m_creature,SPELL_ERNEUERUNG);
                break;
            }

            erneuerung_timer = random(5000,10000);
        } else erneuerung_timer -= diff;

    }

};
CreatureAI* GetAI_prinzessin_moira_bronzebeard(Creature *_Creature)
{
    return new prinzessin_moira_bronzebeardAI (_Creature);
}

void AddSC_prinzessin_moira_bronzebeard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="prinzessin_moira_bronzebeard";
    newscript->GetAI = &GetAI_prinzessin_moira_bronzebeard;
    newscript->RegisterSelf();
}


void AddSC_imperator_dagran_thaurissan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="imperator_dagran_thaurissan";
    newscript->GetAI = &GetAI_imperator_dagran_thaurissan;
    newscript->RegisterSelf();
}

