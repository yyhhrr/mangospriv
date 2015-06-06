/* boss_krümel */

#include "precompiled.h"

#define kruemels_kochkunst	5174
#define saeurespritzer	6306


struct MANGOS_DLL_DECL boss_kruemelAI : public ScriptedAI
{
    boss_kruemelAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 kruemels_kochkunst_timer;
    uint32 saeurespritzer_timer;

    void Reset()
    {
        kruemels_kochkunst_timer = 1000;
        saeurespritzer_timer = 4000;
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

        if (kruemels_kochkunst_timer < diff)
        {
            DoCastSpellIfCan(m_creature,kruemels_kochkunst);
            kruemels_kochkunst_timer = random(5000,10000);
        } else kruemels_kochkunst_timer -= diff;

        if (saeurespritzer_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),saeurespritzer);
            saeurespritzer_timer = random(15000,25000);
        } else saeurespritzer_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_kruemel(Creature *_Creature)
{
    return new boss_kruemelAI (_Creature);
}

void AddSC_boss_kruemel()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kruemel";
    newscript->GetAI = &GetAI_boss_kruemel;
    newscript->RegisterSelf();
}