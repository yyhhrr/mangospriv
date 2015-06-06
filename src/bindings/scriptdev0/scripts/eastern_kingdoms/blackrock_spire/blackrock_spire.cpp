#include "precompiled.h"
#include "blackrock_spire.h"

int i=0;
int solcounter=0;

#define ADD_X1	50.42583f
#define ADD_Y1	-313.180511f
#define ADD_Z1	106.435577f
#define ADD_O1	0

#define ADD_X2	35.793774f
#define ADD_Y2	-312.578308f
#define ADD_Z2	91.550636f
#define ADD_O2	0

bool GOHello_VaterFlamme(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if (pInstance)
    {
        solcounter=0;
        Creature* Add;
        //pInstance->SetData(TYPE_VATER_FLAMME, IN_PROGRESS);
        Add=pPlayer->SummonCreature(10683,ADD_X1,ADD_Y1,ADD_Z1,ADD_O1,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
        Add->AI()->AttackStart(pPlayer);
        solcounter++;
        Add=pPlayer->SummonCreature(10683,ADD_X2,ADD_Y2,ADD_Z2,ADD_O2,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
        Add->AI()->AttackStart(pPlayer);
        Add=pPlayer->SummonCreature(10683,ADD_X2,ADD_Y2,ADD_Z2,ADD_O2,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
        Add->AI()->AttackStart(pPlayer);
        Add=pPlayer->SummonCreature(10683,ADD_X2,ADD_Y2,ADD_Z2,ADD_O2,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
        Add->AI()->AttackStart(pPlayer);
        solcounter++;
    }

	if(solcounter==6)
	{
		Creature* Add;
		Add=pPlayer->SummonCreature(10264,ADD_X2,ADD_Y2,ADD_Z2,ADD_O2,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
       	Add->AI()->AttackStart(pPlayer);
		//return;
	}

    return false;
}

/* solakar_add */

struct MANGOS_DLL_DECL solakar_addAI : public ScriptedAI
{
    solakar_addAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    Creature* Add;

    uint32 spawn_timer;

    void Reset()
    {
        spawn_timer = 5000;
    }

    void JustDied(Unit*)
    {
//        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        //pInstance->SetData(TYPE_VATER_FLAMME, IN_PROGRESS);
    }

    void KilledUnit(Unit*)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(/*TYPE_VATER_FLAMME==IN_PROGRESS && */solcounter<6)
        {
        	if (spawn_timer < diff )
        	{
        		Add=m_creature->SummonCreature(10683,ADD_X2,ADD_Y2,ADD_Z2,ADD_O2,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
        		Add->AI()->AttackStart(target);
        		solcounter++;
        		spawn_timer = 7000;
        	}else spawn_timer -= diff;
        }
        
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_solakar_add(Creature *_Creature)
{
    return new solakar_addAI (_Creature);
}

void AddSC_blackrock_spire()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "vater_flamme";
    newscript->pGOUse = &GOHello_VaterFlamme;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="solakar_add";
    newscript->GetAI = &GetAI_solakar_add;
    newscript->RegisterSelf();

}