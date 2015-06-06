/* Brücken - Brandwache Event */

#include "precompiled.h"

// Gardist unten
#define add1x 739.893f
#define add1y -276.0469f
#define add1z -42.833f
#define add1o 4.395687f
// Gardist oben
#define add2x 644.592f
#define add2y -284.6379f
#define add2z -43.1851f
#define add2o 3.3f

#define SPELL_FEUERBALLSALVE 15285

struct MANGOS_DLL_DECL brandwache_brueckeAI : public ScriptedAI
{
    brandwache_brueckeAI(Creature *c) : ScriptedAI(c) {}

    uint32 feuerballsalve_timer;
    uint32 guardian_spawned;
    Unit* target;
    bool Guard;

    void Reset()
    {
        feuerballsalve_timer= 0;
        guardian_spawned = 0;
        Guard = false;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }
	
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Guard == false)
        {
            if (Creature* tmp = m_creature->SummonCreature(8891,add1x,add1y,add1z,add1o,TEMPSUMMON_DEAD_DESPAWN, 0))
            {
                tmp->AI()->AttackStart(m_creature->getVictim());
                //			tmp->Yell("Es ist noch nichts verloren!",LANG_UNIVERSAL,0);
            }
            if (Creature* tmp = m_creature->SummonCreature(8891,add2x,add2y,add2z,add2o,TEMPSUMMON_DEAD_DESPAWN, 0))
            {
                tmp->AI()->AttackStart(m_creature->getVictim());
                //			tmp->Yell("Ihr werdet schon sehen ...",LANG_UNIVERSAL,0);
            }
            Guard = true;
        }
        if (feuerballsalve_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FEUERBALLSALVE);

            feuerballsalve_timer = 6000;
        } else feuerballsalve_timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_brandwache_bruecke(Creature *_Creature)
{
    return new brandwache_brueckeAI (_Creature);
}

void AddSC_npc_brandwache_bruecke()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_brandwache_bruecke";
    newscript->GetAI = &GetAI_brandwache_bruecke;
    newscript->RegisterSelf();
}
