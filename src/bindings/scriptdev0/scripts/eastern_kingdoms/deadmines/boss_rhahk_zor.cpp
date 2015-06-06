/* Rhahk'Zor */

#include "precompiled.h"
#include "deadmines.h"

#define zerschmettern	6304


struct MANGOS_DLL_DECL boss_rhahk_zorAI : public ScriptedAI
{
    boss_rhahk_zorAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 zerschmettern_timer;

    void Reset()
    {
        zerschmettern_timer = 4000;
    }
    void Aggro(Unit *who)
    {
		m_creature->MonsterYell("Van'Cleef gut zahlen fuer euren Kopfens!",LANG_UNIVERSAL,0);
    }
    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (pInstance)
            pInstance->SetData(TYPE_RHAHKZOR, DONE);
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

        if (zerschmettern_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),zerschmettern);
            zerschmettern_timer = random(6000,10000);
        } else zerschmettern_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_rhahk_zor(Creature *_Creature)
{
    return new boss_rhahk_zorAI (_Creature);
}

void AddSC_boss_rhahk_zor()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_rhahk_zor";
    newscript->GetAI = &GetAI_boss_rhahk_zor;
    newscript->RegisterSelf();
}