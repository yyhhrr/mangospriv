/* kriegsmeister_voone */

#include "precompiled.h"

#define aufwaertshaken	10966
#define axt_werfen	16075
#define hauen	3391
#define schnapptritt	15618
#define spalten	15284
#define toedlicher_stoss	15708
#define zuschlagen	15615


struct MANGOS_DLL_DECL kriegsmeister_vooneAI : public ScriptedAI
{
    kriegsmeister_vooneAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 aufwaertshaken_timer;
    uint32 axt_werfen_timer;
    uint32 hauen_timer;
    uint32 schnapptritt_timer;
    uint32 spalten_timer;
    uint32 toedlicher_stoss_timer;
    uint32 zuschlagen_timer;

    void Reset()
    {
        aufwaertshaken_timer = 2000;
        axt_werfen_timer = 4000;
        hauen_timer = 6000;
        schnapptritt_timer = 10000;
        spalten_timer = 13000;
        toedlicher_stoss_timer = 15000;
        zuschlagen_timer = 18000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Seid ihr Euch sicher?",LANG_UNIVERSAL,NULL);
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

        if (aufwaertshaken_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),aufwaertshaken);
            aufwaertshaken_timer = random(10000,13000);
        } else aufwaertshaken_timer -= diff;

        if (axt_werfen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),axt_werfen);
            axt_werfen_timer = random(8000,12000);
        } else axt_werfen_timer -= diff;

        if (hauen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),hauen);
            hauen_timer = random(12000,15000);
        } else hauen_timer -= diff;

        if (schnapptritt_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schnapptritt);
            schnapptritt_timer = random(16000,20000);
        } else schnapptritt_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(10000,15000);
        } else spalten_timer -= diff;

        if (toedlicher_stoss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),toedlicher_stoss);
            toedlicher_stoss_timer = random(20000,25000);
        } else toedlicher_stoss_timer -= diff;

        if (zuschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),zuschlagen);
            zuschlagen_timer = random(25000,30000);
        } else zuschlagen_timer -= diff;


        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_kriegsmeister_voone(Creature *_Creature)
{
    return new kriegsmeister_vooneAI (_Creature);
}

void AddSC_kriegsmeister_voone()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="kriegsmeister_voone";
    newscript->GetAI = &GetAI_kriegsmeister_voone;
    newscript->RegisterSelf();
}

