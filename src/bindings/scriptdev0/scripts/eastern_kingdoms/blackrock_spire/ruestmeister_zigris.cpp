/* r�stmeister_zigris */

#include "precompiled.h"

#define betaeubungsbombe	16497
#define hakennetz	15609
#define heiltank_trinken	15504
#define schiessen	16496


struct MANGOS_DLL_DECL ruestmeister_zigrisAI : public ScriptedAI
{
    ruestmeister_zigrisAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 betaeubungsbombe_timer;
    uint32 hakennetz_timer;
    uint32 heiltank_trinken_timer;
    uint32 schiessen_timer;

    void Reset()
    {
        betaeubungsbombe_timer = 10000;
        hakennetz_timer = 5000;
        heiltank_trinken_timer = 12000;
        schiessen_timer = 8000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("An meinen Pfeilen kommt keiner vorbei!",LANG_UNIVERSAL,NULL);
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

        if (betaeubungsbombe_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),betaeubungsbombe);
            betaeubungsbombe_timer = random(10000,20000);
        } else betaeubungsbombe_timer -= diff;

        if (hakennetz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hakennetz);
            hakennetz_timer = random(12000,15000);
        } else hakennetz_timer -= diff;

        if (heiltank_trinken_timer < diff)
        {
            DoCastSpellIfCan(m_creature,heiltank_trinken);
            heiltank_trinken_timer = random(30000,35000);
        } else heiltank_trinken_timer -= diff;

        if (schiessen_timer < diff)
        {
            int i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)<5 && i>10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schiessen);
            schiessen_timer = random(6000,20000);
        } else schiessen_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_ruestmeister_zigris(Creature *_Creature)
{
    return new ruestmeister_zigrisAI (_Creature);
}

void AddSC_ruestmeister_zigris()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="ruestmeister_zigris";
    newscript->GetAI = &GetAI_ruestmeister_zigris;
    newscript->RegisterSelf();
}