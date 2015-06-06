/* Marduk */

#include "precompiled.h"

//	   | Zauberbezeichnung		SpellID
#define SPELL_ENTWEIHTE_AURA		17695
#define SPELL_SCHATTENBLITZSALVE	17228
#define SPELL_SCHATTENSCHILD		12040
#define SPELL_SPALTEN				15284

struct MANGOS_DLL_DECL mardukAI : public ScriptedAI
{
    mardukAI(Creature *c) : ScriptedAI(c) {}

    uint32 schattenblitzsalve_timer;
    uint32 schattenschild_timer;
    uint32 spalten_timer;

    Unit* target;

    void Reset()
    {//	ZauberName
        schattenblitzsalve_timer = 5000;
        schattenschild_timer = 32000;
        spalten_timer = 8000;

    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }



    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Die Schatten werden euch verzehren!",LANG_UNIVERSAL,NULL);
        DoCastSpellIfCan(m_creature,SPELL_ENTWEIHTE_AURA);
        DoCastSpellIfCan(m_creature,SPELL_SCHATTENSCHILD);

        m_creature->CallForHelp(VISIBLE_RANGE);
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
            return;

        if (schattenblitzsalve_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SCHATTENBLITZSALVE);
            schattenblitzsalve_timer = random(8000,12000);
        } else schattenblitzsalve_timer -= diff;

        if (schattenschild_timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SCHATTENSCHILD);
            schattenschild_timer = random(32000,35000);
        } else schattenschild_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SPALTEN);
            spalten_timer = random(5000,10000);
        } else spalten_timer -= diff;


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_marduk(Creature *_Creature)
{
    return new mardukAI (_Creature);
}

void AddSC_marduk()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="marduk";
    newscript->GetAI = &GetAI_marduk;
    newscript->RegisterSelf();
}