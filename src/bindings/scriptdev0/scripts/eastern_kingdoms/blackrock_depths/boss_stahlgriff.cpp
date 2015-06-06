/* Stahlgriff */

#include "precompiled.h"
#include "blackrock_depths.h"

#define SPELL_HEILTRANK_TRINKEN		15504
#define SPELL_RUESSTUNG_ZERREISSEN	11971

struct MANGOS_DLL_DECL stahlgriffAI : public ScriptedAI
{
    stahlgriffAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 zauber1_timer;
    uint32 zauber2_timer;
    bool kriegshetzer;

    void Reset()
    {
        zauber1_timer = 5000;
        zauber2_timer = 10000;
        kriegshetzer=false;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit*)
    {
        m_creature->MonsterYell("Was geht hier vor? Eindringlinge!!",LANG_UNIVERSAL,NULL);

    }

    void JustDied(Unit*)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_VAULT, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (zauber1_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_RUESSTUNG_ZERREISSEN);
            zauber1_timer = random(6000,8000);

            if (kriegshetzer==false)
            {
                Creature* tmp=m_creature->SummonCreature(8905,828.032288f,-338.029968f,-50.219379f,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                tmp->AI()->AttackStart(m_creature->getVictim());
                Creature* tmpa=m_creature->SummonCreature(8905,824.111023f,-334.081207f,-50.256893f,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                tmpa->AI()->AttackStart(m_creature->getVictim());
                Creature* tmpb=m_creature->SummonCreature(8905,828.032288f,-338.029968f,-50.219379f,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                tmpb->AI()->AttackStart(m_creature->getVictim());
                Creature* tmpc=m_creature->SummonCreature(8905,824.111023f,-334.081207f,-50.256893f,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                tmpc->AI()->AttackStart(m_creature->getVictim());

                kriegshetzer=true;
            }
        }
        else
        {
            zauber1_timer -= diff;
        }

        if (zauber2_timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_HEILTRANK_TRINKEN);
            zauber2_timer = random(10000, 15000);
        }
        else
        {
            zauber2_timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_stahlgriff(Creature *_Creature)
{
    return new stahlgriffAI (_Creature);
}

void AddSC_boss_stahlgriff()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_stahlgriff";
    newscript->GetAI = &GetAI_stahlgriff;
    newscript->RegisterSelf();
}
