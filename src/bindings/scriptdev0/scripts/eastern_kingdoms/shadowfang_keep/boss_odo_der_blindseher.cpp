/* boss_odo_der_blindseher */

#include "precompiled.h"

#define heulende_wut_75	7481
#define heulende_wut_50	7483
#define heulende_wut_25	7484


struct MANGOS_DLL_DECL boss_odo_der_blindseherAI : public ScriptedAI
{
    boss_odo_der_blindseherAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    bool wut_75;
    bool wut_50;
    bool wut_25;

    void Reset()
    {
        wut_75=false;
        wut_50=false;
        wut_25=false;
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,522);
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.00f);
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

        if (m_creature->isAlive() && wut_75==false && m_creature->GetHealthPercent() < 75.0f)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,1955);
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.70f);
            DoCastSpellIfCan(m_creature,heulende_wut_75);
            wut_75=true;
        }

        if (m_creature->isAlive() && wut_50==false && m_creature->GetHealthPercent() < 50.0f)
        {
            DoCastSpellIfCan(m_creature,heulende_wut_50);
            wut_50=true;
        }

        if (m_creature->isAlive() && wut_25==false && m_creature->GetHealthPercent() < 25.0f)
        {
            DoCastSpellIfCan(m_creature,heulende_wut_25);
            wut_25=true;
        }


        DoMeleeAttackIfReady();

    }
};
CreatureAI* GetAI_boss_odo_der_blindseher(Creature *_Creature)
{
    return new boss_odo_der_blindseherAI (_Creature);
}

void AddSC_boss_odo_der_blindseher()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_odo_der_blindseher";
    newscript->GetAI = &GetAI_boss_odo_der_blindseher;
    newscript->RegisterSelf();
}

