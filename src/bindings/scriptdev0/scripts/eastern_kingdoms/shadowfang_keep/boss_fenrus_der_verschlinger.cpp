/* boss_fenrus_der_verschlinger */

#include "precompiled.h"
#include "shadowfang_keep.h"

#define toxinspeichel	7125


struct MANGOS_DLL_DECL boss_fenrus_der_verschlingerAI : public ScriptedAI
{
    boss_fenrus_der_verschlingerAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 toxinspeichel_timer;

    void Reset()
    {
        toxinspeichel_timer = 8000;
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (pInstance)
            pInstance->SetData(TYPE_FENRUS, DONE);
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

        if (toxinspeichel_timer < diff)
        {
            int i=0;
            target=m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target) > 5 && i<15)
            {
                target=m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,toxinspeichel);
            toxinspeichel_timer = random(14000,17000);
        } else toxinspeichel_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_fenrus_der_verschlinger(Creature *_Creature)
{
    return new boss_fenrus_der_verschlingerAI (_Creature);
}

void AddSC_boss_fenrus_der_verschlinger()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_fenrus_der_verschlinger";
    newscript->GetAI = &GetAI_boss_fenrus_der_verschlinger;
    newscript->RegisterSelf();
}

