/* boss_sneeds */

#include "precompiled.h"
#include "deadmines.h"

#define entwaffnen	6713


struct MANGOS_DLL_DECL boss_sneedsAI : public ScriptedAI
{
    boss_sneedsAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 entwaffnen_timer;

    void Reset()
    {
        entwaffnen_timer = 2000;
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (pInstance)
            pInstance->SetData(TYPE_SNEED, DONE);
        //m_creature->SummonCreature(20004,-18.58,-379.98,60.94,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,20000);
        //m_creature->SummonCreature(20004,-18.58,-379.98,60.94,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,20000);
    }

    void KilledUnit(Unit *victim)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (entwaffnen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),entwaffnen);
            entwaffnen_timer = random(8000,12000);
        } else entwaffnen_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_sneeds(Creature *_Creature)
{
    return new boss_sneedsAI (_Creature);
}

void AddSC_boss_sneeds()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_sneeds";
    newscript->GetAI = &GetAI_boss_sneeds;
    newscript->RegisterSelf();
}