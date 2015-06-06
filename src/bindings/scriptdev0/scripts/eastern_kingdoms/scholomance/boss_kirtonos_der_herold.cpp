/* Kirtonos der Herold */


#include "precompiled.h"
#include "scholomance.h"


//	   | Zauberbezeichnung				SpellID
#define SPELL_ENTWAFFNEN				8379
#define SPELL_FLUCH_DER_SPRACHEN		12889
#define SPELL_FLUEGELSCHLAG				12882
#define SPELL_GEDANKEN_BEHERRSCHEN		14515
#define SPELL_KIRTONOS_TRANSFORMIEREN	16467
#define SPELL_RUESTUNG_DURCHSTECHEN		6016
#define SPELL_SCHATTENBLITZSALVE		17228
#define SPELL_STURZFLUG					18144


struct MANGOS_DLL_DECL kirtonosAI : public ScriptedAI
{
    kirtonosAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* pInstance;

    uint32 entwaffnen_timer;
    uint32 fluch_der_sprachen_timer;
    uint32 fluegelschlag_timer;
    uint32 gedanken_beherrschen_timer;
    uint32 kirtonos_transformieren_timer;
    uint32 ruestung_durchstechen_timer;
    uint32 schattenblitzsalve_timer;
    uint32 sturzflug_timer;

    Unit* target;

    void Reset()
    {
        entwaffnen_timer = 8000;
        fluch_der_sprachen_timer = 15000;
        fluegelschlag_timer = 10000;
        gedanken_beherrschen_timer = 20000;
        kirtonos_transformieren_timer = 100000000;
        ruestung_durchstechen_timer = 5000;
        schattenblitzsalve_timer = 3000;
        sturzflug_timer = 20000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
			pInstance->SetData(TYPE_KIRTONOS, IN_PROGRESS);
		else
        m_creature->MonsterYell("nope!",LANG_UNIVERSAL,NULL);


        m_creature->MonsterYell("Ich war schon immer ein Blutsauger!",LANG_UNIVERSAL,NULL);
    }

    void JustDied(Unit* Killer)
    {
        if (pInstance)
			pInstance->SetData(TYPE_KIRTONOS, DONE);
    }

	void JustReachedHome()
	{
		if (pInstance)
			pInstance->SetData(TYPE_KIRTONOS, FAIL);
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (entwaffnen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ENTWAFFNEN);
            entwaffnen_timer = random(15000,20000);
        } else entwaffnen_timer -= diff;

        if (fluch_der_sprachen_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FLUCH_DER_SPRACHEN);
            fluch_der_sprachen_timer = random(25000,30000);
        } else fluch_der_sprachen_timer -= diff;

        if (fluegelschlag_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FLUEGELSCHLAG);
            fluegelschlag_timer = random(10000,15000);
        } else fluegelschlag_timer -= diff;

        if (gedanken_beherrschen_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_GEDANKEN_BEHERRSCHEN);
            gedanken_beherrschen_timer = random(30000,45000);
        } else gedanken_beherrschen_timer -= diff;

        /*		if (kirtonos_transformieren < diff)
                {
                //target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
        		DoCastSpellIfCan(m_creature->getVictim(),SPELL_KIRTONOS_TRANSFORMIEREN);
        		kirtonos_transformieren = random(2000,10000);
                }kirtonos_transformieren -= diff;
        */
        if (ruestung_durchstechen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_RUESTUNG_DURCHSTECHEN);
            ruestung_durchstechen_timer = random(40000,50000);
        } else ruestung_durchstechen_timer -= diff;

        if (schattenblitzsalve_timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_SCHATTENBLITZSALVE);
            schattenblitzsalve_timer = random(10000,15000);
        } else schattenblitzsalve_timer -= diff;

        if (sturzflug_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_STURZFLUG);
            sturzflug_timer = random(8000,20000);
        } else sturzflug_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_kirtonos(Creature *_Creature)
{
    return new kirtonosAI (_Creature);
}

void AddSC_kirtonos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="kirtonos";
    newscript->GetAI = &GetAI_kirtonos;
    newscript->RegisterSelf();
}