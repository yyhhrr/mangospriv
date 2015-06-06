/* general_drakkisath */

#include "precompiled.h"

#define donnerknall	15588
#define flammenstoss	16419
#define grossbrand	16805
#define ruestung_durchstechen	12097
#define spalten	15284
#define wut	16789


struct MANGOS_DLL_DECL general_drakkisathAI : public ScriptedAI
{
    general_drakkisathAI(Creature *c) : ScriptedAI(c) {
        Reset();
    }

    Unit* target;

    Creature* CDO;
    Creature* CDT;

    uint64 cdo_GUID;
    uint64 cdt_GUID;

    uint32 donnerknall_timer;
    uint32 flammenstoss_timer;
    uint32 grossbrand_timer;
    uint32 ruestung_durchstechen_timer;
    uint32 spalten_timer;
    uint32 wut_timer;

    void Reset()
    {
        donnerknall_timer = 5000;
        flammenstoss_timer = 8000;
        grossbrand_timer = 10000;
        ruestung_durchstechen_timer = 3000;
        spalten_timer = 12000;
        wut_timer = 15000;
    }

    void Aggro(Unit *who)
    {
        m_creature->MonsterYell("Das war ein Fehler von Euch!",LANG_UNIVERSAL,NULL);
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

    void JustReachedHome()
    {
        if ( CDO->isDead()) {
            CDO->Respawn();
        }
        if ( CDT->isDead()) {
            CDT->Respawn();
        }
    }


    void UpdateAI(const uint32 diff)
    {
        Map* pmap;
        pmap= m_creature->GetMap();


        if (CDO = pmap->GetCreature(cdo_GUID))
        {
            // ok !
        }
        else
        {
            CDO = (m_creature->SummonCreature(10814, 32.0f, -291.0f, 111.0f, 3.21f,TEMPSUMMON_MANUAL_DESPAWN, 0));
            cdo_GUID = CDO->GetGUID();
	    CDO->SetRespawnDelay(30000000);
        }

        if (CDT = pmap->GetCreature(cdt_GUID))
        {
            // ok !
        }
        else
        {
            CDT = (m_creature->SummonCreature(10814, 32.0f, -279.0f, 111.0f, 3.21f, TEMPSUMMON_MANUAL_DESPAWN, 0));
            cdt_GUID = CDT->GetGUID();
	    CDO->SetRespawnDelay(30000000);
        }



        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            return;
        }

        if (donnerknall_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),donnerknall);
            donnerknall_timer = random(20000,30000);
        } else donnerknall_timer -= diff;

        if (flammenstoss_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),flammenstoss);
            flammenstoss_timer = random(13000,20000);
        } else flammenstoss_timer -= diff;

        if (grossbrand_timer < diff)
        {
            DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),grossbrand);
            grossbrand_timer = random(20000,30000);
        } else grossbrand_timer -= diff;

        if (ruestung_durchstechen_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),ruestung_durchstechen);
            ruestung_durchstechen_timer = random(25000,30000);
        } else ruestung_durchstechen_timer -= diff;

        if (spalten_timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),spalten);
            spalten_timer = random(6000,10000);
        } else spalten_timer -= diff;

        if (wut_timer < diff)
        {
            DoCastSpellIfCan(m_creature,wut);
            wut_timer = random(30000,35000);
        } else wut_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_general_drakkisath(Creature *_Creature)
{
    return new general_drakkisathAI (_Creature);
}

void AddSC_general_drakkisath()
{

    Script *newscript;
    newscript = new Script;
    newscript->Name="general_drakkisath";
    newscript->GetAI = &GetAI_general_drakkisath;
    newscript->RegisterSelf();
}