/* gyth */

#include "precompiled.h"
#include "blackrock_spire.h"

#define eiskaelte	16350
#define flammenatem	16390
#define rend_schwarzfaust_beschw�ren	16328
#define wegschlagen	10101
#define aetzender_saeureatem	16359

struct MANGOS_DLL_DECL gythAI : public ScriptedAI
{
    gythAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 eiskaelte_timer;
    uint32 flammenatem_timer;
    uint32 rend_schwarzfaust_beschwaeren_timer;
    uint32 wegschlagen_timer;
    uint32 aetzender_saeureatem_timer;
    bool res;
    bool bossspawned;


    void Reset()
    {
        eiskaelte_timer = 3000;
        flammenatem_timer = 6000;
        rend_schwarzfaust_beschwaeren_timer= 0;
        wegschlagen_timer = 7000;
        aetzender_saeureatem_timer = 9000;
        bossspawned=false;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("So, ihr habt noch nicht genug ?",LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit* Killer)
    {
        bossspawned=false;
    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_REND_SCHWARZFAUST,FAIL);
        }
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        if (pInstance->GetData(TYPE_REND_SCHWARZFAUST)!=IN_PROGRESS)
        {
            m_creature->ForcedDespawn(10000);
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || res!=true)
        {
            res=true;
            return;
        }

        if (eiskaelte_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),eiskaelte);
            eiskaelte_timer = random(12000,12000);
        } else eiskaelte_timer -= diff;

        if (flammenatem_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),flammenatem);
            flammenatem_timer = random(12000,12000);
        } else flammenatem_timer -= diff;

        if (wegschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),wegschlagen);
            wegschlagen_timer = random(12000,20000);
        } else wegschlagen_timer -= diff;

        if (aetzender_saeureatem_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),aetzender_saeureatem);
            aetzender_saeureatem_timer = random(12000,12000);
        } else aetzender_saeureatem_timer -= diff;

        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 20 && bossspawned ==false)
        {
	   Creature* tmp;
            tmp=m_creature->SummonCreature(10429,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,600000);
            tmp->SetRespawnDelay(60000000);
	    bossspawned = true;
        }


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_gyth(Creature *_Creature)
{
    return new gythAI (_Creature);
}

void AddSC_gyth()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="gyth";
    newscript->GetAI = &GetAI_gyth;
    newscript->RegisterSelf();
}