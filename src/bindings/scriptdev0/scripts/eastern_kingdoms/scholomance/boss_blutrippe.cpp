/* Blutrippe */


#include "precompiled.h"

#define SPELL_KRIEGSDONNER		16727
#define SPELL_STOSS				18368
#define SPELL_WEGSCHLAGEN		10101

struct MANGOS_DLL_DECL blutrippeAI : public ScriptedAI
{
    blutrippeAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    uint32 kriegsdonner_timer;
    uint32 stoss_timer;
    uint32 wegschlagen_timer;

    Unit* target;

    void Reset()
    {
        kriegsdonner_timer = 5000;
        stoss_timer = 8000;
        wegschlagen_timer = 10000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Wie ihr es verlangt!",LANG_UNIVERSAL,NULL);
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (kriegsdonner_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KRIEGSDONNER);

            kriegsdonner_timer = random(8000,12000);
        } else kriegsdonner_timer -= diff;

        if (stoss_timer < diff)
        {
            int i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)>10 && i<10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,SPELL_STOSS);
            stoss_timer = random(5000,10000);
        } else stoss_timer -= diff;

        if (wegschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_WEGSCHLAGEN);
            wegschlagen_timer = random(10000,15000);
        } else wegschlagen_timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_blutrippe(Creature *_Creature)
{
    return new blutrippeAI (_Creature);
}

void AddSC_blutrippe()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="blutrippe";
    newscript->GetAI = &GetAI_blutrippe;
    newscript->RegisterSelf();
}