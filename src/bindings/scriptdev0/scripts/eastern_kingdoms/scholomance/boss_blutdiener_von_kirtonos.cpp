/* Blutdiener von Kirtonos */

#include "precompiled.h"
#include "scholomance.h"

#define SPELL_FLUCH_DER_MACHTLOSIGKEIT			22371
#define SPELL_FLUCH_DER_SCHWAECHE			12493
#define SPELL_LAEHMENDES_GIFT				3609

struct MANGOS_DLL_DECL blutdiener_von_kirtonosAI : public ScriptedAI
{
    blutdiener_von_kirtonosAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 fluch_der_machtlosigkeit_timer;
    uint32 fluch_der_schwaeche_timer;
    uint32 laehmendes_gift_timer;
    int i;

    void Reset()
    {
        fluch_der_machtlosigkeit_timer = 3000;
        fluch_der_schwaeche_timer = 1000;
        laehmendes_gift_timer = 5000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Mein Blut werdet ihr nie bekommen!",LANG_UNIVERSAL,NULL);

    }

    void JustDied(Unit *killer)
    {
        
    }

    void KilledUnit(Unit *victim)
    {
       
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //RavenousClaw
        if (fluch_der_machtlosigkeit_timer < diff)
        {
            i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)<5 && i<10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,SPELL_FLUCH_DER_MACHTLOSIGKEIT);

            fluch_der_machtlosigkeit_timer = random(30000,45000);

        } else fluch_der_machtlosigkeit_timer -= diff;

        if (fluch_der_schwaeche_timer < diff)
        {
            i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)>3 && i<10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,SPELL_FLUCH_DER_SCHWAECHE);

            fluch_der_schwaeche_timer = random(30000,45000);

        } else fluch_der_schwaeche_timer -=diff;

        if (laehmendes_gift_timer < diff)
        {
            i=0;
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)>5 && i<10)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            DoCastSpellIfCan(target,SPELL_LAEHMENDES_GIFT);
            laehmendes_gift_timer = random(10000,15000);

        } else laehmendes_gift_timer -=diff;


        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_blutdiener_von_kirtonos(Creature *_Creature)
{
    return new blutdiener_von_kirtonosAI (_Creature);
}


void AddSC_blutdiener_von_kirtonos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="blutdiener_von_kirtonos";
    newscript->GetAI = &GetAI_blutdiener_von_kirtonos;
    newscript->RegisterSelf();
}

