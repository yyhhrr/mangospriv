/* die_bestie */

#include "precompiled.h"

#define berserkerattacke	16636
#define erschreckendes_gebruell	14100
#define feuerball	16788
#define feuerbrand	15570
#define feuerschlag	16144
#define flammenbruch	16785
#define spieler_beschwoeren	20477


struct MANGOS_DLL_DECL die_bestieAI : public ScriptedAI
{
    die_bestieAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 berserkerattacke_timer;
    uint32 erschreckendes_gebruell_timer;
    uint32 feuerball_timer;
    uint32 feuerbrand_timer;
    uint32 feuerschlag_timer;
    uint32 flammenbruch_timer;
    uint32 spieler_beschwoeren_timer;
    uint32 i;
    bool port;


    void Reset()
    {
        berserkerattacke_timer = 10000;
        erschreckendes_gebruell_timer = 5000;
        feuerball_timer = 2000;
        feuerbrand_timer = 4000;
        feuerschlag_timer = 6000;
        flammenbruch_timer = 12000;
        spieler_beschwoeren_timer = 1;
        i=0;
        port = false;
    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature->getVictim(),berserkerattacke);
        m_creature->MonsterYell("Flieg und stirb!",LANG_UNIVERSAL,NULL);
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

        if (berserkerattacke_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),berserkerattacke);
            berserkerattacke_timer = random(20000,30000);
        } else berserkerattacke_timer -= diff;

        if (erschreckendes_gebruell_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),erschreckendes_gebruell);
            erschreckendes_gebruell_timer = random(20000,25000);
        } else erschreckendes_gebruell_timer -= diff;

        if (feuerball_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),feuerball);
            feuerball_timer = random(8000,15000);
        } else feuerball_timer -= diff;

        if (feuerbrand_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),feuerbrand);
            feuerbrand_timer = random(10000,20000);
        } else feuerbrand_timer -= diff;

        if (feuerschlag_timer < diff)
        {
            port = true;
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),feuerschlag);
            feuerschlag_timer = random(8000,15000);
        } else feuerschlag_timer -= diff;

        if (flammenbruch_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),flammenbruch);
            flammenbruch_timer = random(15000,20000);
        } else flammenbruch_timer -= diff;

        if (port == true)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            while (m_creature->GetDistance(target)<45 && i<15)
            {
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                i++;
            }
            if (i<15)
            {
                m_creature->MonsterYell("Weglaufen gibt es nicht!",LANG_UNIVERSAL,NULL);
                DoTeleportPlayer(target, m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation());
            }
            i=0;
        }
        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_die_bestie(Creature *_Creature)
{
    return new die_bestieAI (_Creature);
}

void AddSC_die_bestie()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="die_bestie";
    newscript->GetAI = &GetAI_die_bestie;
    newscript->RegisterSelf();
}