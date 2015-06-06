/* boss_edwin_van_cleff */

#include "precompiled.h"

#define hauen	3391
#define van_cleffs_verbuendete	5200


struct MANGOS_DLL_DECL boss_edwin_van_cleffAI : public ScriptedAI
{
    boss_edwin_van_cleffAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    bool spawned;

    uint32 hauen_timer;
    uint32 van_cleffs_verbuendete_timer;

    void Reset()
    {
        hauen_timer = 2000;
        spawned = false;
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

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (hauen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hauen);
            hauen_timer = random(4000,6000);
        } else hauen_timer -= diff;

        if ((spawned == false) && (m_creature->GetHealthPercent() < 50.0f))
        {
            Creature* lump = m_creature->SummonCreature(636,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
            lump->AI()->AttackStart(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
            Creature* lump2 = m_creature->SummonCreature(636,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
            lump2->AI()->AttackStart(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
            spawned = true;
        }


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_edwin_van_cleff(Creature *_Creature)
{
    return new boss_edwin_van_cleffAI (_Creature);
}

void AddSC_boss_edwin_van_cleff()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_edwin_van_cleff";
    newscript->GetAI = &GetAI_boss_edwin_van_cleff;
    newscript->RegisterSelf();
}