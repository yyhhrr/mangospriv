/* Der Unverziehene*/

//Standart - Pflicht!
#include "precompiled.h"

//Regestierliste:

//	   | Zauberbezeichnung			SpellID
#define SPELL_FROSTNOVA				14907


struct MANGOS_DLL_DECL der_unverzieheneAI : public ScriptedAI
{
    der_unverzieheneAI(Creature *c) : ScriptedAI(c) {}

    uint32 zauber1_timer;
    Unit* target;

    void Reset()
    {//	ZauberName
        zauber1_timer = 10000;//10 sec
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Ihr wollt doch nicht etwas weglaufen?",LANG_UNIVERSAL,NULL);
        DoCastSpellIfCan(m_creature,SPELL_FROSTNOVA);
    }

    void JustDied(Unit* Killer)
    {

    }

    void KilledUnit(Unit *victim)
    {

    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (zauber1_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_FROSTNOVA);
            zauber1_timer = random(6000,10000);
        }
        else
        {
            zauber1_timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_der_unverziehene(Creature *_Creature)
{
    return new der_unverzieheneAI (_Creature);
}

void AddSC_der_unverziehene()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="der_unverziehene";
    newscript->GetAI = &GetAI_der_unverziehene;
    newscript->RegisterSelf();
}