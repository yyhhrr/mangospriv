/* H�ter des Wissens Polkelt */


#include "precompiled.h"
#include "scholomance.h"


//	   | Zauberbezeichnung			SpellID
#define SPELL_FLUECHTIGE_INFEKTION	18149
#define SPELL_GIFTIGER_KATALYSATOR	18151
#define SPELL_AETZENDE_SAEURE			8245


struct MANGOS_DLL_DECL hueter_des_wissens_polkeltAI : public ScriptedAI
{
    hueter_des_wissens_polkeltAI(Creature *c) : ScriptedAI(c) {}

//	uint32 fluechtige_infektion_timer;
    uint32 giftiger_katalysator_timer;
    uint32 aetzende_saeure_timer;

    Unit* target;

    void Reset()
    {
        giftiger_katalysator_timer = 2000;
        aetzende_saeure_timer = 4000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }



    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("So so!",LANG_UNIVERSAL,NULL);
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_POLKELT,IN_PROGRESS);
    }

    void JustDied(Unit* Killer)
    {
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        pInstance->SetData(TYPE_POLKELT,DONE);

    }

	void JustReachedHome()
	{
        ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
		if (pInstance)
			pInstance->SetData(TYPE_POLKELT, FAIL);
	}

    void KilledUnit(Unit *victim)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
            pInstance->SetData(TYPE_POLKELT,FAIL);
        }
    }


    void UpdateAI(const uint32 diff)
    {
        Unit* target;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;


        /*        if (fluechtige_infektion_timer < diff)
                {
        		DoCastSpellIfCan(m_creature, SPELL_RASEREI);
        		fluechtige_infektion_timer = random(125000,130000);
                }else fluechtige_infektion_timer -= diff;
        */
        if (giftiger_katalysator_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_GIFTIGER_KATALYSATOR);
            DoCastSpellIfCan(target,SPELL_FLUECHTIGE_INFEKTION);

            giftiger_katalysator_timer = random(8000,12000);
        } else giftiger_katalysator_timer -= diff;

        if (aetzende_saeure_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_AETZENDE_SAEURE);
            aetzende_saeure_timer = random(10000,15000);
        } else aetzende_saeure_timer -= diff;


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_hueter_des_wissens_polkelt(Creature *_Creature)
{
    return new hueter_des_wissens_polkeltAI (_Creature);
}

void AddSC_hueter_des_wissens_polkelt()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="hueter_des_wissens_polkelt";
    newscript->GetAI = &GetAI_hueter_des_wissens_polkelt;
    newscript->RegisterSelf();
}