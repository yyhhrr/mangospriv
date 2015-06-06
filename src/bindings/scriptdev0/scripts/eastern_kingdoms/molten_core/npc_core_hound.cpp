/* npc_core_hound */

#include "precompiled.h"
#include "molten_core.h"

#define spell_cone_of_fire	19630
#define spell_bite	19771
#define spell_ground_stomp	19364
#define spell_ancient_dread	19365
#define spell_cauterizing_flames	19366
#define spell_withering_heat	19367
#define spell_ancient_despair	19369
#define spell_ancient_hysteria	19372


struct MANGOS_DLL_DECL npc_core_houndAI : public ScriptedAI
{
    npc_core_houndAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 spell_cone_of_fire_timer;
    uint32 spell_bite_timer;
    uint32 spell_ground_stomp_timer;
    uint32 despawn_timer;
    ScriptedInstance* pInstance;

    uint32 hittimer;

    void Reset()
    {
        spell_cone_of_fire_timer = 6000;
        spell_bite_timer = 10000;
        spell_ground_stomp_timer = 12000;
	
	despawn_timer = 1800000;

        hittimer = 1200;

        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Wuff wuff!",LANG_UNIVERSAL,NULL);
        switch (rand()%5)
        {
        case 0:
            DoCastSpellIfCan(m_creature,spell_ancient_dread);
            break;
        case 1:
            DoCastSpellIfCan(m_creature,spell_cauterizing_flames);
            break;
        case 2:
            DoCastSpellIfCan(m_creature,spell_withering_heat);
            break;
        case 3:
            DoCastSpellIfCan(m_creature,spell_ancient_despair);
            break;
        case 4:
            DoCastSpellIfCan(m_creature,spell_ancient_hysteria);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {

    }

    void KilledUnit(Unit *victim)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void JustRespawned()
    {
        if (	pInstance->GetData(TYPE_MAGMADAR) == DONE)
        {
            m_creature->setFaction(734);
            m_creature->SetVisibility(VISIBILITY_OFF);
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
	{
	   if ( pInstance->GetData(TYPE_MAGMADAR) == DONE)
	   {
	     if( despawn_timer < diff)
	     {
             m_creature->setFaction(734);
             m_creature->SetVisibility(VISIBILITY_OFF);
	     despawn_timer = 100000;
	     }else despawn_timer-=diff;
	   }
	   return;
	}
           

        if (spell_cone_of_fire_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spell_cone_of_fire);
            spell_cone_of_fire_timer = random(7000,10000);
        } else spell_cone_of_fire_timer -= diff;

        if (spell_bite_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spell_bite);
            spell_bite_timer = random(5000,10000);
        } else spell_bite_timer -= diff;

        if (spell_ground_stomp_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spell_ground_stomp);
            spell_ground_stomp_timer = random(10000,15000);
        } else spell_ground_stomp_timer -= diff;

        if (hittimer < diff)
        {
            DoMeleeAttackIfReady();
            hittimer = 1200;
        } else hittimer -= diff;
    }
};

CreatureAI* GetAI_npc_core_hound(Creature *_Creature)
{
    return new npc_core_houndAI (_Creature);
}

void AddSC_npc_core_hound()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_core_hound";
    newscript->GetAI = &GetAI_npc_core_hound;
    newscript->RegisterSelf();
}

