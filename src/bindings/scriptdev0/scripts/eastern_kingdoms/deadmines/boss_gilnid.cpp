/* boss_gilnid */

#include "precompiled.h"
#include "deadmines.h"

#define fluessiges_metall	5213


struct MANGOS_DLL_DECL boss_gilnidAI : public ScriptedAI
{
    boss_gilnidAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 fluessiges_metall_timer;

    void Reset()
    {
        fluessiges_metall_timer = 2000;
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*) m_creature->GetInstanceData();

        if (pInstance)
            pInstance->SetData(TYPE_GILNID, DONE);
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

        if (fluessiges_metall_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),fluessiges_metall);
            fluessiges_metall_timer = random(7000,12000);
        } else fluessiges_metall_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_gilnid(Creature *_Creature)
{
    return new boss_gilnidAI (_Creature);
}

void AddSC_boss_gilnid()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gilnid";
    newscript->GetAI = &GetAI_boss_gilnid;
    newscript->RegisterSelf();
}