/* Instrukteurin Malicia */


#include "precompiled.h"
#include "scholomance.h"

#define SPELL_BLITZHEILUNG				17843
#define SPELL_ERNEUERUNG				8362
#define SPELL_HEILEN					15586
#define SPELL_RUF_DES_GRABES			17831
#define SPELL_VERDERBNIS				18376
#define SPELL_VERLANGSAMEN				13747


struct MANGOS_DLL_DECL instrukteurin_maliciaAI : public ScriptedAI
{
    instrukteurin_maliciaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* m_pInstance;

    uint32 blitzheilung_timer;
    uint32 erneuerung_timer;
    uint32 heilen_timer;
    uint32 ruf_des_grabes_timer;
    uint32 verderbnis_timer;
    uint32 verlangsamen_timer;


    void Reset()
    {
        blitzheilung_timer = 8000;
        erneuerung_timer = 6000;
        heilen_timer = 7000;
        ruf_des_grabes_timer = 2000;
        verderbnis_timer = 5000;
        verlangsamen_timer = 6000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Heilen ist meine Kunst!",LANG_UNIVERSAL,NULL);
        m_pInstance->SetData(TYPE_MALICIA, IN_PROGRESS);
    }

    void JustDied(Unit* Killer)
    {
        m_pInstance->SetData(TYPE_MALICIA, DONE);
    }

	void JustReachedHome()
	{
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		if (pInstance)
			pInstance->SetData(TYPE_MALICIA, FAIL);
	}

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            m_pInstance->SetData(TYPE_MALICIA, FAIL);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (blitzheilung_timer < diff)
        {
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 70)
            {
                DoCastSpellIfCan(m_creature,SPELL_BLITZHEILUNG);
                blitzheilung_timer = random(10000,20000);
            }
        } else blitzheilung_timer -= diff;

        if (erneuerung_timer < diff)
        {
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 90)
            {
                DoCastSpellIfCan(m_creature,SPELL_ERNEUERUNG);
                erneuerung_timer = random(20000,25000);
            }
        } else erneuerung_timer -= diff;

        if (heilen_timer < diff)
        {
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 30)
            {
                DoCastSpellIfCan(m_creature,SPELL_HEILEN);
                heilen_timer = random(10000,20000);
            }
        } else heilen_timer -= diff;

        if (ruf_des_grabes_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_RUF_DES_GRABES);
            ruf_des_grabes_timer = random(60000,65000);
        } else ruf_des_grabes_timer -= diff;


        if (verderbnis_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_VERDERBNIS);
            verderbnis_timer = random(25000,30000);
        } else verderbnis_timer -= diff;

        if (verlangsamen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_VERLANGSAMEN);
            verlangsamen_timer = random(15000,20000);
        } else verlangsamen_timer -= diff;


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_instrukteurin_malicia(Creature *_Creature)
{
    return new instrukteurin_maliciaAI (_Creature);
}

void AddSC_instrukteurin_malicia()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="instrukteurin_malicia";
    newscript->GetAI = &GetAI_instrukteurin_malicia;
    newscript->RegisterSelf();
}