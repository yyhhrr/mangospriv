/* boss_schattenpriester_sezz_ziz */

#include "precompiled.h"
#include "zulfarrak.h"

#define erneuerung	8362
#define heilen	12039
#define psychischer_schrei	13074
#define schattenblitz	15537


struct MANGOS_DLL_DECL boss_schattenpriester_sezz_zizAI : public ScriptedAI
{
    boss_schattenpriester_sezz_zizAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;
    Unit* target;

    uint32 erneuerung_timer;
    uint32 heilen_timer;
    uint32 psychischer_schrei_timer;
    uint32 schattenblitz_timer;

    void Reset()
    {
        erneuerung_timer = 6000;
        heilen_timer = 10000;
        psychischer_schrei_timer = 8000;
        schattenblitz_timer = 3000;
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            Reset();
            return;
        }

        if (erneuerung_timer < diff)
        {
            DoCastSpellIfCan(m_creature,erneuerung);
            erneuerung_timer = random(20000,30000);
        } else erneuerung_timer -= diff;

        if (heilen_timer < diff)
        {
            DoCastSpellIfCan(m_creature,heilen);
            heilen_timer = random(10000,20000);
        } else heilen_timer -= diff;

        if (psychischer_schrei_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),psychischer_schrei);
            psychischer_schrei_timer = random(20000,22000);
        } else psychischer_schrei_timer -= diff;

        if (schattenblitz_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),schattenblitz);
            schattenblitz_timer = random(5000,10000);
        } else schattenblitz_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_schattenpriester_sezz_ziz(Creature *_Creature)
{
    return new boss_schattenpriester_sezz_zizAI (_Creature);
}

void AddSC_boss_schattenpriester_sezz_ziz()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_schattenpriester_sezz_ziz";
    newscript->GetAI = &GetAI_boss_schattenpriester_sezz_ziz;
    newscript->RegisterSelf();
}

