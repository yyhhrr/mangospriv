/* npc_teufelsross */

#include "precompiled.h"

#define teufelstampfen	7139


struct MANGOS_DLL_DECL npc_teufelsrossAI : public ScriptedAI
{
    npc_teufelsrossAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    Creature* teufelsross1;
    Creature* teufelsross2;

    uint32 teufelstampfen_timer;
    uint64 teufelsross1GUID;
    uint64 teufelsross2GUID;

    void Reset()
    {
        teufelstampfen_timer = 10000;
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
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (m_creature->isInCombat())
        {
            target=m_creature->getVictim();

            if (!teufelsross1->isInCombat())
                teufelsross1->AI()->AttackStart(target);

            if (!teufelsross2->isInCombat())
                teufelsross2->AI()->AttackStart(target);
        }
        else
        {
            Reset();
        }

        if (teufelsross1 = pInstance->instance->GetCreature(teufelsross1GUID))
        {
            if (teufelsross1->isInCombat())
            {
                target=teufelsross1->getVictim();

                if (!m_creature->isInCombat())
                    m_creature->AI()->AttackStart(target);

                if (!teufelsross2->isInCombat())
                    teufelsross2->AI()->AttackStart(target);
            }
        }
        else
        {
            teufelsross1=m_creature->SummonCreature(20008, -234.356979f, 2235.093262f, 79.778633f, 0.359692f, TEMPSUMMON_MANUAL_DESPAWN, 0);
            teufelsross1GUID=teufelsross1->GetGUID();
        }

        if (teufelsross2 = pInstance->instance->GetCreature(teufelsross2GUID))
        {
            if (teufelsross2->isInCombat())
            {
                target=teufelsross2->getVictim();

                if (!m_creature->isInCombat())
                    m_creature->AI()->AttackStart(target);

                if (!teufelsross1->isInCombat())
                    teufelsross1->AI()->AttackStart(target);
            }
        }
        else
        {
            teufelsross2=m_creature->SummonCreature(20008, -217.616943f, 2247.620605f, 79.774231f, 5.013832f, TEMPSUMMON_MANUAL_DESPAWN, 0);
            teufelsross2GUID=teufelsross2->GetGUID();
        }

        if (m_creature->SelectHostileTarget() || m_creature->getVictim())
        {
            if (teufelstampfen_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),teufelstampfen);
                teufelstampfen_timer = random(10000,15000);
            }
            else teufelstampfen_timer -= diff;

            DoMeleeAttackIfReady();
        }

    }
};

CreatureAI* GetAI_npc_teufelsross(Creature *_Creature)
{
    return new npc_teufelsrossAI (_Creature);
}


// Fake Teufelsross
struct MANGOS_DLL_DECL npc_fakerossAI : public ScriptedAI
{
    npc_fakerossAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;

    uint32 teufelstampfen_timer;

    void Reset()
    {
        teufelstampfen_timer = 10000;
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
        if (m_creature->SelectHostileTarget() || m_creature->getVictim())
        {
            if (teufelstampfen_timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),teufelstampfen);
                teufelstampfen_timer = random(8000,20000);
            }
            else teufelstampfen_timer -= diff;

            DoMeleeAttackIfReady();
        }

    }
};

CreatureAI* GetAI_npc_fakeross(Creature *_Creature)
{
    return new npc_fakerossAI (_Creature);
}

void AddSC_npc_teufelsross()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_teufelsross";
    newscript->GetAI = &GetAI_npc_teufelsross;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_fakeross";
    newscript->GetAI = &GetAI_npc_fakeross;
    newscript->RegisterSelf();
}

