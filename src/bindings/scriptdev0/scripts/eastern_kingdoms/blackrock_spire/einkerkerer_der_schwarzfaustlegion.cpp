/* einkerkerer_der_schwarzfaustlegion */

#include "precompiled.h"
#include "blackrock_spire.h"


#define einsperren	16045
#define schlag	15580


struct MANGOS_DLL_DECL einkerkerer_der_schwarzfaustlegionAI : public ScriptedAI
{
    einkerkerer_der_schwarzfaustlegionAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    uint32 einsperren_timer;
    uint32 schlag_timer;
    uint32 feuerwache_glutseher_timer;
    bool free;

    void Reset()
    {
        einsperren_timer = 20000;
        schlag_timer = 3000;
        feuerwache_glutseher_timer = 15000;
        free=false;
    }

    void JustDied(Unit* Killer)
    {

    }

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_ALTAR_EVENT,FAIL);
            m_creature->setFaction(734);
            m_creature->SetVisibility(VISIBILITY_OFF);
        }

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();

        if (!pInstance->GetData(TYPE_ALTAR_EVENT)==IN_PROGRESS)
        {
            DoCastSpellIfCan(m_creature,15281,0,m_creature->GetGUID());
            m_creature->setFaction(734);
        }
        else
        {
            if (free!=true)
            {
                Player* pe;
                if (pe=pInstance->instance->GetPlayer(pInstance->GetData64(DATA_PLAYER)))
                    m_creature->AI()->AttackStart(pe);
                m_creature->setFaction(754);
                m_creature->CastStop();
                free = true;
            }
        }


        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (einsperren_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),einsperren);
            einsperren_timer = random(20000,25000);
        } else einsperren_timer -= diff;

        if (schlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),schlag);
            schlag_timer = random(3000,4000);
        } else schlag_timer -= diff;

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_einkerkerer_der_schwarzfaustlegion(Creature *_Creature)
{
    return new einkerkerer_der_schwarzfaustlegionAI (_Creature);
}

void AddSC_einkerkerer_der_schwarzfaustlegion()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="einkerkerer_der_schwarzfaustlegion";
    newscript->GetAI = &GetAI_einkerkerer_der_schwarzfaustlegion;
    newscript->RegisterSelf();
}