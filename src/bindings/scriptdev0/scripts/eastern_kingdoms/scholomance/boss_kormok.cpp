/* Kormok */
/* Knochendiener 16119 */
/* Knochenmagier 16120 */

#include "precompiled.h"

#define SPELL_SCHATTENBLITZSALVE	17228
#define SPELL_KNOCHENSCHILD         27688
#define SPELL_RASEREI				8269
#define SPELL_KAMPFRAUSCH			27689

struct MANGOS_DLL_DECL kormokAI : public ScriptedAI
{
    kormokAI(Creature *c) : ScriptedAI(c) {}

    uint32 schattenblitzsalve_Timer;
    uint32 knochenschild_Timer;
    uint32 skelettkrieger_Timer;
    uint32 skelettmagier_Timer;
    uint32 raserei_Timer;

    bool Mages;
    int Rand1;
    int Rand1X;
    int Rand1Y;
    int Rand2;
    int Rand2X;
    int Rand2Y;
    Creature* SummonedMinions;
    Creature* SummonedMages;
    Unit* mob;
    Unit* target;

    void Reset()
    {
        schattenblitzsalve_Timer = 10000;
        knochenschild_Timer = 2000;
        skelettkrieger_Timer = 15000;
        skelettmagier_Timer = 0;
        raserei_Timer = 20000;
        Mages = false;
    }

    void Aggro(Unit *who)
    {
    }

    void SummonMinion(Unit* victim)
    {
        Rand1 = rand()%8;
        switch (rand()%2)
        {
        case 0:
            Rand1X = 0 - Rand1;
            break;
        case 1:
            Rand1X = 0 + Rand1;
            break;
        }

        Rand1 = rand()%8;
        switch (rand()%2)
        {
        case 0:
            Rand1Y = 0 - Rand1;
            break;
        case 1:
            Rand1Y = 0 + Rand1;
            break;
        }
        Rand1 = 0;
        mob = SummonedMinions = DoSpawnCreature(16119, Rand1X, Rand1Y, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000);
        ((CreatureAI*)SummonedMinions->AI())->AttackStart(victim);
    }

    void SummonMages(Unit* victim)
    {
        Rand2 = rand()%10;
        switch (rand()%2)
        {
        case 0:
            Rand2X = 0 - Rand2;
            break;
        case 1:
            Rand2X = 0 + Rand2;
            break;
        }

        Rand2 = rand()%10;
        switch (rand()%2)
        {
        case 0:
            Rand2Y = 0 - Rand2;
            break;
        case 1:
            Rand2Y = 0 + Rand2;
            break;
        }
        Rand2 = 0;
        SummonedMages = DoSpawnCreature(16120, Rand2X, Rand2Y, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000);
        ((CreatureAI*)SummonedMages->AI())->AttackStart(victim);
    }


    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (schattenblitzsalve_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SCHATTENBLITZSALVE);
            schattenblitzsalve_Timer = 15000;
        } else schattenblitzsalve_Timer -= diff;


        if (knochenschild_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_KNOCHENSCHILD);
            knochenschild_Timer = 2600000;
        } else knochenschild_Timer -= diff;

        if (raserei_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_RASEREI);
            raserei_Timer = 130000;
        } else raserei_Timer -= diff;


        if (skelettkrieger_Timer < diff)
        {
            SummonMinion(m_creature->getVictim());
            SummonMinion(m_creature->getVictim());
            SummonMinion(m_creature->getVictim());
            SummonMinion(m_creature->getVictim());
            DoCastSpellIfCan(mob,SPELL_KAMPFRAUSCH);

            skelettkrieger_Timer = random(20000,25000);
        } else skelettkrieger_Timer -= diff;

        //Summon 2 Bone Mages
        if ( !Mages && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 26 )
        {
            //Cast
            SummonMages(m_creature->getVictim());
            SummonMages(m_creature->getVictim());
            Mages = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_kormok(Creature *_Creature)
{
    return new kormokAI (_Creature);
}

#define SPELL_FLAMMENSTOSS	16102
#define SPELL_FROSTBLITZ	15043
#define SPELL_FROSTNOVA		15063


struct MANGOS_DLL_DECL knochenmagierAI : public ScriptedAI
{
    knochenmagierAI(Creature *c) : ScriptedAI(c) {}

    Unit* target;
    uint32 flammenstoss_Timer;
    uint32 frostblitz_Timer;
    uint32 frostnova_Timer;

    void Reset()
    {
        flammenstoss_Timer = 10000;
        frostblitz_Timer = 2000;
        frostnova_Timer = 15000;

    }

    void Aggro(Unit *who)
    {
    }

    int random(int min, int max)
    {
        return (int)((max-min) * rand()/(float)RAND_MAX +min);
    }


    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;


        if (flammenstoss_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FLAMMENSTOSS);
            flammenstoss_Timer = random(10000,15000);
        } else flammenstoss_Timer -= diff;


        if (frostblitz_Timer < diff)
        {
            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            DoCastSpellIfCan(target,SPELL_FROSTBLITZ);
            frostblitz_Timer = random (3000,5000);
        } else frostblitz_Timer -= diff;

        if (frostnova_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTNOVA);
            frostnova_Timer = random(12000,17000);
        } else frostnova_Timer -= diff;


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_knochenmagier(Creature *_Creature)
{
    return new knochenmagierAI (_Creature);
}


void AddSC_kormok()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="kormok";
    newscript->GetAI = &GetAI_kormok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="knochenmagier";
    newscript->GetAI = &GetAI_knochenmagier;
    newscript->RegisterSelf();
}