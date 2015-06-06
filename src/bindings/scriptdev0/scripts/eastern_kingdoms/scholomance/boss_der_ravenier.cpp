/* Der Ravenier */


#include "precompiled.h"
#include "scholomance.h"


//	   | Zauberbezeichnung		SpellID
#define SPELL_SPALTEN				15284
#define SPELL_TRAMPELN				15550
#define SPELL_WEGSCHLAGEN			18670
#define SPELL_ZERREISSENDES_SPALTEN	17963

struct MANGOS_DLL_DECL der_ravenierAI : public ScriptedAI
{
    der_ravenierAI(Creature *c) : ScriptedAI(c) {}

    uint32 spalten_timer;
    uint32 trampeln_timer;
    uint32 wegschlagen_timer;
    uint32 zerreissendes_spalten_timer;

    Unit* target;

    void Reset()
    {
        spalten_timer = 5000;
        trampeln_timer = 8000;
        wegschlagen_timer = 10000;
        zerreissendes_spalten_timer = 3000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }



    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Zerrei�en und Spalten!",LANG_UNIVERSAL,NULL);
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_RAVENIAN,IN_PROGRESS);
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_RAVENIAN,DONE);
    }

	void JustReachedHome()
	{
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		if (pInstance)
			pInstance->SetData(TYPE_RAVENIAN, FAIL);
	}

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_RAVENIAN,FAIL);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SPALTEN);
            spalten_timer = random(8000,12000);
        } else spalten_timer -= diff;

        if (trampeln_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_TRAMPELN);
            trampeln_timer = random(5000,10000);
        } else trampeln_timer -= diff;

        if (wegschlagen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_WEGSCHLAGEN);
            wegschlagen_timer = random(10000,15000);
        } else wegschlagen_timer -= diff;

        if (zerreissendes_spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ZERREISSENDES_SPALTEN);
            zerreissendes_spalten_timer = random(15000,25000);
        } else zerreissendes_spalten_timer -= diff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_der_ravenier(Creature *_Creature)
{
    return new der_ravenierAI (_Creature);
}

void AddSC_der_ravenier()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="der_ravenier";
    newscript->GetAI = &GetAI_der_ravenier;
    newscript->RegisterSelf();
}