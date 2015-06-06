/* halycon */

#include "precompiled.h"

#define hauen	3391
#define verwunden	13738


struct MANGOS_DLL_DECL halyconAI : public ScriptedAI
//struct TRINITY_DLL_DECL halyconAI : public ScriptedAI
{
    halyconAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 hauen_timer;
    uint32 verwunden_timer;

    void Reset()
    {
        hauen_timer = 3000;
        verwunden_timer = 6000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Grrrrr! ",LANG_UNIVERSAL,NULL);
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
            return;
        }

        if (hauen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hauen);
            hauen_timer = random(6000,10000);
        } else hauen_timer -= diff;

        if (verwunden_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),verwunden);
            verwunden_timer = random(10000,20000);
        } else verwunden_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_halycon(Creature *_Creature)
{
    return new halyconAI (_Creature);
}

void AddSC_halycon()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="halycon";
    newscript->GetAI = &GetAI_halycon;
    newscript->RegisterSelf();
}