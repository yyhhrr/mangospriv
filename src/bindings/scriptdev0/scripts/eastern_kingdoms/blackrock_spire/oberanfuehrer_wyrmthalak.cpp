/* oberanfuehrer_wyrmthalak */

#include "precompiled.h"

#define demoralisierender_ruf	16244
#define spalten	15284
#define weitreichendes_zerschmettern	12887
bool addspawned=false;

#define berserkerx -38.641724f
#define berserkery -509.080170f
#define berserkerz 89.010513f
#define berserkero 4.679958f

#define kriegsherrx -49.890232f
#define kriegsherry -508.107727f
#define kriegsherrz 88.442253f
#define kriegsherro 4.694881f

struct MANGOS_DLL_DECL oberanfuehrer_wyrmthalakAI : public ScriptedAI
{
    oberanfuehrer_wyrmthalakAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;


    uint32 demoralisierender_ruf_timer;
    uint32 spalten_timer;
    uint32 weitreichendes_zerschmettern_timer;

    void Reset()
    {
        demoralisierender_ruf_timer = 3000;
        spalten_timer = 5000;
        weitreichendes_zerschmettern_timer = 10000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Ich liebe Besuch! Kommt nur her!",LANG_UNIVERSAL,NULL);
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

        if (demoralisierender_ruf_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),demoralisierender_ruf);
            demoralisierender_ruf_timer = random(25000,30000);
        } else demoralisierender_ruf_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(4000,8000);
        } else spalten_timer -= diff;

        if (weitreichendes_zerschmettern_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),weitreichendes_zerschmettern);
            weitreichendes_zerschmettern_timer = random(8000,15000);
        } else weitreichendes_zerschmettern_timer -= diff;

        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30 && addspawned ==false)
        {
            Creature* tmp=m_creature->SummonCreature(9216,kriegsherrx,kriegsherry,kriegsherrz,kriegsherro,TEMPSUMMON_CORPSE_TIMED_DESPAWN,120000);
            tmp->AI()->AttackStart(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
	    tmp->SetRespawnDelay(30000000);
            tmp=m_creature->SummonCreature(9268,berserkerx,berserkery,berserkerz,berserkero,TEMPSUMMON_CORPSE_TIMED_DESPAWN,120000);
            tmp->AI()->AttackStart(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
            tmp->SetRespawnDelay(30000000);
	    addspawned = true;
        }

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_oberanfuehrer_wyrmthalak(Creature *_Creature)
{
    return new oberanfuehrer_wyrmthalakAI (_Creature);
}

void AddSC_oberanfuehrer_wyrmthalak()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="oberanfuehrer_wyrmthalak";
    newscript->GetAI = &GetAI_oberanfuehrer_wyrmthalak;
    newscript->RegisterSelf();
}